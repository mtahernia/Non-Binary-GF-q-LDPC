/*
 * Message.cpp
 *
 *  Created on: 23 Dec, 2014
 *      Author: mehrdad
 */
#include <fftw3.h> // This is a try to fix fft problem.
#include "Definitions.h"
#include "Message.h"
/*********************************************************************************
 *
 * MESSAGE
 *
 *********************************************************************************/
// FIXME: Need to rewrite convolve and this function, currently I'm ignoring the complex part and the whole convolution will be inaccurate
void message::DFT()          // A real-valued DFT - also IDFT
{
	static message Aux;
	GFq mask, n0_index, n1_index;
	BYTE j_bit;


	if (!GFq::IsPrimeQ) {
		for (int i = 0; i < GFq::log_2_q; i++) {
			Aux = *this;			// Initialize
			mask.val = 1 << i;  //Shift left i times to get powers of 2
			//			cout << "mask.val("<<i<<")="<<mask.val<<std::endl; // Just for debugging
			for (GFq j(0); j.val < q; j.val++) {
				j_bit = (j.val & mask.val) >> i;// obtain value of j which is i th bit of j
				n0_index.val = j.val & (~mask.val);	// turn bit off
				n1_index.val = j.val | mask.val;    // turn bit on
				//				cout << (int)j_bit;
				if (j_bit == 0)
					Probs[j.val] = Aux[n0_index] + Aux[n1_index];
				else
					Probs[j.val] = Aux[n0_index] - Aux[n1_index];
				//				cout << "Aux(" << n0_index << ")= " << Aux[n0_index] << std::endl;
			}    //end for j
		}    //end for i
	}    //end if
	else if (GFq::IsPrimeQ) // FIXME: There is lots of memory copying and redundant fft generation(plan generation fixed by defining static plan).
	{
		Aux = *this;

		// Create fft variables and a plan
		static fftw_complex *in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * q);
		static fftw_complex *out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * q);
		static fftw_plan p = fftw_plan_dft_1d(q, in, out, FFTW_FORWARD, FFTW_PATIENT);

		// Copy the data to input of FFT TODO: make use of copy or other faster method
		for(int i=0;i<q;i++)
		{
			in[i][0] = Aux[i];
			in[i][1] = Aux.ProbsI[i];
		}

		// Execute FFT
		fftw_execute(p); /* repeat as needed */


		for(int i=0;i<q;i++)
		{
			Probs[i] = out[i][0] ;
			ProbsI[i] = out[i][1] ;
		}

		// These lines are commented because we don't want to delete our plan and we want to use it over and over
		//		fftw_destroy_plan(p);
		//		fftw_free(in); fftw_free(out);


	} // Else if prime Q
}

void message::IDFT()          // A real-valued DFT - also IDFT
{
	static message Aux;
	GFq mask, n0_index, n1_index;
	BYTE j_bit;


	if (!GFq::IsPrimeQ) {
		for (int i = 0; i < GFq::log_2_q; i++) {
			Aux = *this;			// Initialize
			mask.val = 1 << i;  //Shift left i times to get powers of 2

			//			cout << "mask.val("<<i<<")="<<mask.val<<std::endl; // Just for debugging

			for (GFq j(0); j.val < q; j.val++) {
				j_bit = (j.val & mask.val) >> i;// obtain value of j which is i th bit of j
				n0_index.val = j.val & (~mask.val);	// turn bit off
				n1_index.val = j.val | mask.val;    // turn bit on

				//				cout << (int)j_bit;

				if (j_bit == 0)
					Probs[j.val] = Aux[n0_index] + Aux[n1_index];
				else
					Probs[j.val] = Aux[n0_index] - Aux[n1_index];
				//				cout << "Aux(" << n0_index << ")= " << Aux[n0_index] << std::endl;
			}    //end for j
		}    //end for i
	}    //end if

	else if (GFq::IsPrimeQ) // FIXME: There is lots of memory copying and redundant fft generation(plan generation fixed by defining static plan).
	{
		Aux = *this;


		// Create fft variables and a plan
		static fftw_complex *in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * q);
		static fftw_complex *out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * q);
		static fftw_plan p = fftw_plan_dft_1d(q, in, out, FFTW_BACKWARD, FFTW_PATIENT);

		// Copy the data to input of FFT
		for(int i=0;i<q;i++)
		{
			in[i][0] = Aux[i];
			in[i][1] = Aux.ProbsI[i];
		}

		// Execute FFT
		fftw_execute(p); /* repeat as needed */

		// Copy output values Ignoring complex part!
		for(int i=0;i<q;i++)
		{
			Probs[i] = out[i][0] ;
			ProbsI[i] = out[i][1] ;
		}

		// These lines are commented because we don't want to delete our plan and we want to use it over and over
		//		fftw_destroy_plan(p);
		//		fftw_free(in); fftw_free(out);


	} // Else if prime Q
}

double message::ABS(int i){
	return sqrt(Probs[i]*Probs[i]+ProbsI[i]*ProbsI[i]);
}

void message::HardMessage(GFq &g){
	*this = 0; (*this)[g] = 1;
}

BOOLEAN message::operator<(message &m2) {
	message &m1 = *this;
	for (int i = 0; i <= m1.q; i++)
		if (m1[i] >= m2[i])
			return FALSE;
	return TRUE;
}

message& message::operator=(message &M) {
	if (q != M.q)
		Set_q(M.q);
	//======================================================================
	// memcpy and bcopy cause some overflow apparently. and copying this way is very slow
	//		for (int i = 0; i < q; i++)
	//		{
	//			Probs[i] = M.Probs[i];
	//			ProbsI[i] = M.ProbsI[i];
	//		}
	//======================================================================
	//		bcopy(/* from */M.Probs, /* to */Probs, sizeof(double) * q); // This transition might make some problem if the source and destination are overlapping
	//		bcopy(/* from */M.ProbsI, /* to */ProbsI, sizeof(double) * q);
	//======================================================================
	//		memcpy(/* to */Probs,/* from */M.Probs, sizeof(double) * q);
	//		memcpy(/* to */ProbsI,/* from */M.ProbsI, sizeof(double) * q);
	//======================================================================
	//		memmove(/* to */Probs,/* from */M.Probs, sizeof(double) * q);
	//		memmove(/* to */ProbsI,/* from */M.ProbsI, sizeof(double) * q);
	//======================================================================
	// This finally works like a charm!
	std::copy(M.Probs, M.Probs+q, Probs);
	std::copy(M.ProbsI, M.ProbsI+q, ProbsI);
	return *this;
}

message& message::operator=(double d) {
	for (int i = 0; i < q; i++){
		Probs[i] = d;
		ProbsI[i] = 0;
	}
	return *this;
}

message& message::operator*=(message &M) {
	double x,y;
	for (int i = 0; i < q; i++){

		x = Probs[i] * M.Probs[i] - ProbsI[i] * M.ProbsI[i];
		y = Probs[i] * M.ProbsI[i]+ ProbsI[i] * M.Probs[i];
		Probs[i] = x;
		ProbsI[i] = y;
	}
	return *this;
}

message& message::operator*(message &M2) {
	static message Aux(q);
	message &M1 = *this;

	for (int i = 0; i < q; i++){
		Aux.Probs[i] = M1.Probs[i] * M2.Probs[i]-M1.ProbsI[i] * M2.ProbsI[i];
		Aux.ProbsI[i] = M1.Probs[i] * M2.ProbsI[i]+M1.ProbsI[i] * M2.Probs[i];
	}
	return Aux;
}

message& message::operator*(double d) {
	static message Aux(q);

	for (int i = 0; i < q; i++){
		Aux[i] = Probs[i] * d;
		Aux.ProbsI[i] = ProbsI[i] * d;
	}
	return Aux;
}

BOOLEAN message::operator==(message &m) {
	if (q != m.q)
		return FALSE;
	for (int i = 0; i < q; i++)
		//         if (Probs[i] != m[i])
		if (fabs(Probs[i] - m[i]) > EPSILON)
			return FALSE;
	return TRUE;
}

BOOLEAN message::operator==(double d) {
	for (int i = 0; i < q; i++)
		//if (fabs(Probs[i] - d) > EPSILON)
		if (Probs[i] != d)
			return FALSE;

	return TRUE;
}

message& message::operator+=(message &M) {
	for (int i = 0; i < q; i++){
		Probs[i] += M.Probs[i];
		ProbsI[i] += M.ProbsI[i];
	}
	return *this;
}

message& message::operator+(message &M) {
	static message Aux(q);

	for (int i = 0; i < q; i++){
		Aux[i] = Probs[i] + M.Probs[i];
		Aux.ProbsI[i] = ProbsI[i] + M.ProbsI[i]; //FIXME
	}
	return Aux;
}

message& message::operator/=(double d) {
	for (int i = 0; i < q; i++){
		Probs[i] /= d;
		ProbsI[i] /= d;
	}

	return *this;
}

double message::Maximum() {
	double maximum = -INF;
	for (int i = 0; i < q; i++)
		if (Probs[i] > maximum)
			maximum = Probs[i];
	return maximum;
}

double message::sum() {
	double aux = 0;
	for (int i = 0; i < q; i++){
		aux += sqrt(Probs[i]*Probs[i]+ProbsI[i]*ProbsI[i]);
	}

	return aux;
}

void message::Normalize() {
	double aux = sum();

	if (aux > 0) {
		for (int i = 0; i < q; i++){
			Probs[i] = sqrt(Probs[i]*Probs[i]+ProbsI[i]*ProbsI[i])/aux;
			ProbsI[i] = 0;
		}
	}
	else {	// If the message does not sum to something positive, give uniform values to each component TODO: Check why?
		for (int i = 0; i < q; i++){
			Probs[i] = 1. / (double) q;
			ProbsI[i] = 0;
		}
	}
}

void message::Clear() {
	bzero(Probs, sizeof(double) * q);
	bzero(ProbsI, sizeof(double) * q);
}

// Normal convolution! TODO: this can be implemented using FFTW
// This convolves this message whith M2 and stores the result in this! this is a somehow circular convolution
message& message::Convolve(message &M2) {
	message M1(*this);   // Auxiliary
	GFq t;
	// Clear this
	Clear();

	for (GFq i(0); i.val < q; i.val++)
		for (GFq j(0); j.val < q; j.val++){
			t = i - j;
			Probs[i.val] += M1.Probs[j.val] * M2.Probs[t.val] - M1.ProbsI[j.val] * M2.ProbsI[t.val];
			ProbsI[i.val] += M1.Probs[j.val] * M2.ProbsI[t.val] + M1.ProbsI[j.val] * M2.Probs[t.val];
		}
//				Probs[i.val] += M1[j] * M2[i - j];

	return *this;
}
