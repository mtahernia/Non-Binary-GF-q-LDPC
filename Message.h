/*
 * Message.h
 *
 *  Created on: 23 Dec, 2014
 *      Author: mehrdad
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_
#include <cstring>  // bzero
#include "GFq.h"

/***************************************************************************
 *
 * Messages
 *
 ***************************************************************************/
class message {
public:
	signed int q;
	double Probs[MAX_Q];
	double ProbsI[MAX_Q];
public:
	message(int p_q = -1) :	q(p_q) {}
	message(message &M)   :	q(M.q) {*this = M;}

	double ABS(int i);
	void HardMessage(GFq &g);
	void Set_q(int p_q) {q = p_q;}
	void DFT();
	void IDFT();
	double Maximum();// Returns the maximum component of the message


	// Operator overloading for getting message values
	double &operator[](int i) {	return Probs[i];}
	double &operator[](GFq i) {	return Probs[i.val];}
	message &operator=(message &M);
	message &operator=(double d);
	message &operator*=(message &M);
	message &operator*(message &M2);
	message &operator*(double d);
	BOOLEAN operator==(message &m); // TODO: Not compatible with complex
	BOOLEAN operator==(double d);// TODO: Not compatible with complex	// FIXME: this might make a problem because of decimation error
	BOOLEAN operator<(message &m2);	// If all q components of this message is smaller than message 2 returns true
	message &operator+=(message &M);
	message &operator+(message &M);
	message &operator/=(double d);

	double sum();
	void Normalize();
	void Clear();
	// Clip large and small values to
	void Clip(double minval = EPSILON, double maxval = INF) {
		for (int i = 0; i < q; i++) {
			clip(Probs[i], maxval);
			clip(ProbsI[i], maxval);
			if (Probs[i] < minval)
				Probs[i] = minval;
			if (ProbsI[i] < minval)
				ProbsI[i] = minval;
		}
	}
	// Normal convolution! TODO: this can be implemented using FFTW
	// This convolves this message whith M2 and stores the result in this! this is a somehow circular convolution
	message &Convolve(message &M2);

	message &ApproxConvolve(message &M2);// TODO:These two are not defined and not used
	void Approximate(); // TODO: Define and use them for fast implementation
	message &MaxConvolve(message &M2); //TODO : This function is not used and it is not compatible for complex message
	void PermutePlus(GFq &g);
	void PermuteTimes(GFq &g);
	message &Reverse();

	GFq &Decision();// Used in Rightbound messages and thus does not need to support complex
	double ProbCorrect();// FIXME: Where and why this function is used, it is not called anywhere
	double Entropy();// Gives the entropy of message

	message &operator<<(int l);// Left shift the message l
	message &LLRShift(int k);// FIXME:
	double AverageD();// Divide message to m[0] and set m[0] to zero
	void operator<<=(int l);// Left shift assignment for message
	int RandomSelectIndex(); // Select index at random based on probabilities in message
//   ~message() { if (Probs != NULL) delete[] Probs; }
};

// Convolve two messages using traditional convolve function
inline message &Convolve(message &M1, message &M2) {
	static message Aux;

	Aux = M1;
	Aux.Convolve(M2);
	return Aux;
}

// output message to ostream
inline std::ostream &operator<<(std::ostream &s, message &m) {
	s << ceil(m[0] * 1000.) / 1000.;
	for (int i = 1; i < m.q; i++)
		s << " " << ceil(m[i] * 1000.) / 1000.;

	return s;
}

// subtract two messages
inline message &operator-(message &m1, message &m2) {
	static message aux;

	aux.q = m1.q;

	for (int i = 0; i < m1.q; i++){
		aux.Probs[i]  = m1.Probs[i]  - m2.Probs[i];
		aux.ProbsI[i] = m1.ProbsI[i] - m2.ProbsI[i];
	}

	return aux;
}

// Devide message by d
inline message &operator/(message &m, double d) {
	static message aux;

	aux.q = m.q;

	for (int i = 0; i < m.q; i++){
		aux.Probs[i] = m.Probs[i] / d;
		aux.ProbsI[i] = m.ProbsI[i] / d;
	}
	return aux;
}

// Raise message components to power l
// FIXME,TODO: This is not used and does not support complex message
inline double pow(message &m, int l) {
	double f = 0;

	for (int i = 0; i < m.q; i++)
		f += pow(m[i], l);

	return f;
}


// returns sum of absolute values of message component
inline double fabs(message &m) {
	double f = 0;

	for (int i = 0; i < m.q; i++)
		f += fabs(m.ABS(i));

	return f;
}

// returns the log of message components
inline message &log(message &m) {
	static message aux;

	aux.q = m.q;
	for (int i = 0; i < m.q; i++){
		aux[i] = mylog(m.ABS(i));
		aux.ProbsI[i] = 0;
	}

	return aux;
}

// returns a message which is e^m
inline message &exp(message &m) {
	static message aux;

	aux.q = m.q;
	for (int i = 0; i < m.q; i++){
		aux[i] = exp(m.ABS(i));
		aux.ProbsI[i] = 0;
	}

	return aux;
}


// Returns message in LLR form
inline message &LLR(message &m) {
	static message aux;

	aux.q = m.q;
	double log_m0 = mylog(m.ABS(0));

	aux[0] = 0;
	aux.ProbsI[0] = 0;

	for (int i = 1; i < m.q; i++){
		aux[i] = log_m0 - mylog(m.ABS(i));
		aux.ProbsI[i] = 0;
	}

	return aux;
}

// Does the reverse of LLR
inline message &unLLR(message &m) {
	static message aux;

	aux.q = m.q;

	aux[0] = 1;
	for (int i = 1; i < m.q; i++)
		aux[i] = exp(-m.ABS(i));

	aux.Clip();
	aux.Normalize();
	aux.Clip();

	return aux;
}


/*****************************************************************************
 *
 * ArrayOfMessage
 *
 *****************************************************************************/

class ArrayOfMessage {
	message *Messages;
	int size;
public:

	int GetSize() {
		return size;
	}

	ArrayOfMessage() :
			Messages(NULL), size(0) {
	}

	void Allocate(int p_Size) {
		if (size != p_Size) {
			deAllocate();
			size = p_Size;
			Messages = new message[size];
		}
	}

	void deAllocate() {
		if (Messages != NULL)
			delete Messages;

		Messages = NULL;
		size = 0;
	}

	~ArrayOfMessage() {
		deAllocate();
	}

	message &operator[](int i) {
		return Messages[i];
	}
};


#endif /* MESSAGE_H_ */
