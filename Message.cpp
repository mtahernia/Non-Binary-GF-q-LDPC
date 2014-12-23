/*
 * Message.cpp
 *
 *  Created on: 23 Dec, 2014
 *      Author: mehrdad
 */
#include <fftw3.h> // This is a try to fix fft problem.
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




