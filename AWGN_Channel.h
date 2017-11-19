/*
 * AWGN_Channel.h
 *
 *  Created on: 23 Dec, 2014
  *      Author: Mehrdad Tahernia
 */
#ifndef AWGN_CHANNEL_H_
#define AWGN_CHANNEL_H_

#include "Channel.h"
class LDPC_Code; //defined in LDPC.h, but header is not needed so we use forward declerations

/****************************************************************************
 *
 * AWGN Channel
 *
 ****************************************************************************/
class AWGN_Channel: public channel {
private:
//	double source_sigma;
	double noise_sigma;
public:
	// Specialized functions ----------------------------
	AWGN_Channel(double p_noise_sigma = -1) :noise_sigma(p_noise_sigma) { }
	~AWGN_Channel(){cout << "AWGN_Channel Destructor Called\n";}
	double NoiseVariance(); // Returns noise_sigma^2
	double NoiseStdDev();	// Returns noise_sigma
	void SetNoiseSigma(double p_noise_sigma) { noise_sigma = p_noise_sigma; }
	// General functions --------------------------------
	const char *GetChannelName() { return "AWGN_Channel"; }
	virtual void PrintChannelData(LDPC_Code &Code);
	virtual void ProcessMapping(LDPC_Code &Code);
	// Channel coding functions -------------------------
	virtual double SimulateOutput(double ChannelInput);
	virtual double CalcProbForInput(double ChannelOutput, double ChannelInput);
	// Statistical data ---------------------------------
	virtual double CapacityInBits();
};

#endif /* AWGN_CHANNEL_H_ */
