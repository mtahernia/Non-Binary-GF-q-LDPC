/*
 * Message.h
 *
 *  Created on: 23 Dec, 2014
 \*      Author: Mehrdad Tahernia
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_
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
   ~message() {}

	double ABS(int i);
	void HardMessage(GFq &g);
	void Set_q(int p_q) {q = p_q;}
	void DFT();
	void IDFT();
	double Maximum();// Returns the maximum component of the message
	double &operator[](int i) {	return Probs[i];} 	// Operator overloading for getting message values
	double &operator[](GFq i) {	return Probs[i.val];}	// Operator overloading for getting message values
	message &operator=(message &M);
	message &operator=(double d);
	message &operator*=(message &M);
	message &operator*(message &M2);
	message &operator*(double d);
	bool operator==(message &m); // TODO: Not compatible with complex
	bool operator==(double d);// TODO: Not compatible with complex	// FIXME: this might make a problem because of decimation error
	bool operator<(message &m2);	// If all q components of this message is smaller than message 2 returns true
	message &operator+=(message &M);
	message &operator+(message &M);
	message &operator/=(double d);

	double sum();
	void Normalize();
	void Clear();
	void Clip(double minval = EPSILON, double maxval = INF);	// Clip large and small values to

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
};

message &Convolve(message &M1, message &M2); // Convolve two messages using traditional convolve function
std::ostream &operator<<(std::ostream &s, message &m);// output message to ostream
message &operator-(message &m1, message &m2);// subtract two messages
message &operator/(message &m, double d);// Devide message by d
double pow(message &m, int l);// Raise message components to power l // FIXME,TODO: This is not used and does not support complex message
double fabs(message &m);// returns sum of absolute values of message component
message &log(message &m);// returns the log of message components
message &exp(message &m);// returns a message which is e^m
message &LLR(message &m);// Returns message in LLR form
message &unLLR(message &m);// Does the reverse of LLR
/*****************************************************************************
 *
 * ArrayOfMessage
 *
 *****************************************************************************/

class ArrayOfMessage {
	message *Messages;
	int size;
public:
	ArrayOfMessage() :Messages(NULL), size(0) {	}
	~ArrayOfMessage() {	deAllocate();}

	int GetSize() {	return size;}
	void Allocate(int p_Size) {
		if (size != p_Size) {
			deAllocate();
			size = p_Size;
			Messages = new message[size];
		}
	}
	void deAllocate() {
		if (Messages != NULL)
			delete[] Messages;
		Messages = NULL;
		size = 0;
	}
	message &operator[](int i) {return Messages[i];	}
};

#endif /* MESSAGE_H_ */
