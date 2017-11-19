/*
 * AWGN_Channel.cpp
 *
 *  Created on: 23 Dec, 2014
 *      Author: Mehrdad Tahernia
 */
#include <cstdlib>
#include "Definitions.h"
#include "LDPC.h" //LDPC Code
#include "AWGN_Channel.h"

/*********************************************************************************
 *
 * AWGN Channel
 *
 ********************************************************************************/

void AWGN_Channel::ProcessMapping(LDPC_Code &Code) {
	Code.MapInUse.Normalize();
}

double AWGN_Channel::CapacityInBits() {
	double No = pow(noise_sigma, 2.);
	double SNR = 1. / No;
	return 0.5 * log(1. + SNR) / log(2.);
}

void AWGN_Channel::PrintChannelData(LDPC_Code &Code) {
	double BitRate, No, SNR, SNR_dB;

	BitRate = Code.Calc_Bit_Rate();
	No = pow(noise_sigma, 2.);
	SNR = 1. / No;
	SNR_dB = 10. * log10(SNR);

	cout << "SNR(dB) = " << SNR_dB
			<< " SNR = " << SNR
			<< " Noise Sigma = "<< noise_sigma
			<< "\nCapacity at SNR (symbols per channel use) = "<< 0.5 * log(1. + SNR) / log((double) GFq::q)
			<< "\nCapacity at SNR (bits per channel use) = "   << 0.5 * log(1. + SNR) / log(2.)
			<< "\nMinimum SNR for rate (dB) = "<< 10. * log(pow(2., 2. * BitRate) - 1.) / log(10.)
			<< " (absolute value) = " << pow(2., 2. * BitRate) - 1;
}

// override virtual functions
// Gaussian distribution P(Y|X)
double AWGN_Channel::CalcProbForInput(double ChannelOutput, double ChannelInput) {
	static const double sqrt_2pi = sqrt(2 * 3.141592653);

	double noise_prob = (1 / (sqrt_2pi * noise_sigma)
			* exp( -pow(ChannelOutput - ChannelInput, 2.)/ (2. * NoiseVariance())) );

	return noise_prob;
}

double AWGN_Channel::SimulateOutput(double ChannelInput)
// Simulate the result of passing the zero vector through the AWGN
// Actually pass ChannelInput from a gaussian channel
{
	return ChannelInput + GaussGenerate(noise_sigma);
}

double AWGN_Channel::NoiseVariance() {
	return pow(noise_sigma, 2);
}

double AWGN_Channel::NoiseStdDev() {
	return noise_sigma;
}


