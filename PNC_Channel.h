/*
 * PNC_Channel.h
 *
 *  Created on: 23 Dec, 2014
 *      Author: mehrdad
 */

#ifndef PNC_CHANNEL_H_
#define PNC_CHANNEL_H_
//#include <cstdlib>
#include "Channel.h"
class LDPC_Code; //defined in LDPC.h, but header is not needed so we use forward declerations


/****************************************************************************
 *
 * PNC Channel
 *
 ****************************************************************************/
class PNC_Channel: public channel {
private:
	double noise_sigma;
	double h_A, h_B;
	int alpha, beta;
public:
	// Specialized functions ----------------------------
	PNC_Channel(double p_noise_sigma = -1) :
			noise_sigma(p_noise_sigma),
			h_A(1),
			h_B(1),
			alpha(1),
			beta(1){
	}

	double NoiseVariance(); // Returns noise_sigma^2
	double NoiseStdDev();	// Returns noise_sigma

	void SetNoiseSigma(double p_noise_sigma) {
		noise_sigma = p_noise_sigma;
	}

	// General functions --------------------------------
	const char *GetChannelName() {
		return "PNC_Channel";
	}

	virtual void PrintChannelData(LDPC_Code &Code);
	virtual void ProcessMapping(LDPC_Code &Code);

	// Channel coding functions -------------------------
//	virtual double SimulateOutput(double ChannelInput);
//	virtual double CalcProbForInput(double ChannelOutput, double ChannelInput);
//
//	// Statistical data ---------------------------------
//	virtual double CapacityInBits();
//
};



#endif /* PNC_CHANNEL_H_ */
