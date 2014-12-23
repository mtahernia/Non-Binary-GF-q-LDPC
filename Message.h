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
	// If all q components of this message is smaller than message 2 returns true
	BOOLEAN operator<(message &m2);
	// Returns the maximum component of the message
	double Maximum();
	// Operator overloading for getting message values
	double &operator[](int i) {	return Probs[i];}
	double &operator[](GFq i) {	return Probs[i.val];}

	message &operator=(message &M);
	message &operator=(double d);
	message &operator*=(message &M);
	message &operator*(message &M2);
	message &operator*(double d);

	// TODO: Not compatible with complex
	BOOLEAN operator==(message &m);
	// TODO: Not compatible with complex
	// FIXME: this might make a problem because of decimation error
	BOOLEAN operator==(double d);
	message &operator+=(message &M);
	message &operator+(message &M);
	message &operator/=(double d);
	double sum();
	void Normalize();
	void Clear();

	// Normal convolution! TODO: this can be implemented using FFTW
	// This convolves this message whith M2 and stores the result in this! this is a somehow circular convolution
	message &Convolve(message &M2);

	//These two are not defined and not used TODO: Define and use them for fast implementation
	message &ApproxConvolve(message &M2);
	void Approximate();

	//TODO : This function is not used and it is not compatible for complex message
	message &MaxConvolve(message &M2) {     // max-prod version
		message M1(*this);   // Auxiliary

		// Clear this
		Clear();
		double max;
		double candidate;

		for (GFq i(0); i.val < q; i.val++) {
			max = -1;
			for (GFq j(0); j.val < q; j.val++) {
				candidate = M1[j] * M2[i - j];
				if (candidate > max)
					max = candidate;
			}
			Probs[i.val] = max;
		}

		return *this;
	}




	void PermutePlus(GFq &g) {
		message Aux;
		GFq t;
		Aux = *this;
		for (GFq i(0); i.val < q; i.val++){
			t = i+g;
			Probs[i.val] = Aux.Probs[t.val];
			ProbsI[i.val] = Aux.ProbsI[t.val];
//			Probs[i.val] = Aux[i + g];
		}
	}

	void PermuteTimes(GFq &g) {
		message Aux;
		GFq t;
		Aux = *this;
		for (GFq i(0); i.val < q; i.val++){
			t = i * g;
			Probs[i.val] = Aux.Probs[t.val];
			ProbsI[i.val] = Aux.ProbsI[t.val];
		}
	}

	message &Reverse() {
		message Aux;
		GFq t;
		Aux = *this;
		for (GFq i(0); i.val < q; i.val++){
			t = i.Minus();
			Probs[i.val] = Aux.Probs[t.val];
			ProbsI[i.val] = Aux.ProbsI[t.val];
		}


		return *this;
	}

	// Used in Rightbound messages and thus does not need to support complex
	GFq &Decision() {
		static GFq Candidate(0);
		double max = -1;
		int count_max = 0;

		// find maximum components of message and number of them if there is no unique maximum
		for (GFq i(0); i.val < q; i.val++) {
			if (Probs[i.val] > max) {
				max = Probs[i.val];
				Candidate = i;
				count_max = 1;
			} else if (Probs[i.val] == max)
				count_max++;
		}

		if (count_max > 1) {     // If more than one maximum - randomly select
			int selection = uniform_random(count_max) + 1; // Between 1 and count_max
			int found_so_far = 0;
			// TODO: this part can be written more efficiently
			for (GFq i(0); i.val < q; i.val++)
				if (Probs[i.val] == max) {
					Candidate = i;
					found_so_far++;
					if (found_so_far == selection)
						break;
				}
		}

		return Candidate;
	}

	// FIXME: Where and why this function is used, it is not called anywhere
	double ProbCorrect() {
		int count_zero = 1;

		for (int i = 1; i < q; i++) {
			if (Probs[i] > Probs[0]) {
				return 0;
			} else if (Probs[i] == Probs[0]) {
				count_zero++;
			}
		}

		return 1. / count_zero;
	}

	// Gives the entropy of message
	double Entropy() {
		double aux = 0;
		double aux2;

		for (int i = 0; i < q; i++) {
			if (Probs[i] != 0) {
				aux2 = Probs[i] * log(Probs[i]);
				aux += -clip(aux2);
			}
		}

		return aux;
	}

	// Left shift the message
	message &operator<<(int l) {
		static message Aux;

		Aux.q = q;
		for (int i = 0; i < q; i++){
			Aux.Probs[i] = Probs[(i + l) % q];
			Aux.ProbsI[i] = ProbsI[(i + l) % q];
		}
		return Aux;
	}

	// FIXME:
	message &LLRShift(int k) {
		message Aux = *this;

		for (int i = 0; i < q; i++){
			Probs[i] = Aux[(i + k) % q] - Aux[k % q];
			ProbsI[i] = Aux.ProbsI[(i + k) % q] - Aux.ProbsI[k % q];
		}
		return *this;
	}

	// Divide message to m[0] and set m[0] to zero
	double AverageD() {
		message m = *this;

		m.Clip();
		m /= m[0];
		m.Clip();

		m[0] = 0;     // Don't count zero
		return m.sum() / (GFq::q - 1);
	}

	// Left shift assignment for message
	void operator<<=(int l) {
		*this = *this << l;
	}

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

	int RandomSelectIndex(); // Select index at random based on probabilities in message

	//   ~message()
	//      { if (Probs != NULL) delete Probs; }

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
