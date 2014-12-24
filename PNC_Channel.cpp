/*
 * PNC_Channel.cpp
 *
 *  Created on: 23 Dec, 2014
 *      Author: mehrdad
 */
#include <cstdlib>
#include "Definitions.h"
#include "LDPC.h" //LDPC Code
#include "PNC_Channel.h"


/*********************************************************************************
 *
 * PNC Channel
 *
 ********************************************************************************/

void PNC_Channel::ProcessMapping(LDPC_Code &Code) {
	Code.MapInUse.Normalize();
}

// I use AWGN Channel Capacity formula
double PNC_Channel::CapacityInBits() {
	double No = pow(noise_sigma, 2.);
	double SNR = (h_A*h_A+h_B*h_B)*1.0 / No;
	return 0.5 * log(1. + SNR) / log(2.);
}

void PNC_Channel::PrintChannelData(LDPC_Code &Code) {
	cout << "FIXME: Channel Data Calculation and Printing Not Implemented!!!!";
	double BitRate, No, SNR, SNR_dB;

	BitRate = Code.Calc_Bit_Rate();
	No = pow(noise_sigma, 2.);
	SNR = (h_A*h_A+h_B*h_B)*1.0 / No;
	SNR_dB = 10. * log10(SNR);

	cout << "SNR(dB) = " << SNR_dB
			<< " SNR = " << SNR
			<< " Noise Sigma = "<< noise_sigma
			<< "\nCapacity at SNR (symbols per channel use) = "<< 0.5 * log(1.0 + SNR) / log((double) GFq::q)
			<< "\nCapacity at SNR (bits per channel use) = "   << 0.5 * log(1.0 + SNR) / log(2.0)
			<< "\nMinimum SNR for rate (dB) = "<< 10.0 * log(pow(2.0, 2.0 * BitRate) - 1.) / log(10.0)
			<< " (absolute value) = " << pow(2.0, 2.0 * BitRate) - 1;
}

// override virtual functions
// Gaussian distribution P(Y|X)
double PNC_Channel::CalcProbForInput(double ChannelOutput, double ChannelInput) {
	static const double sqrt_2pi = sqrt(2 * 3.141592653);

	double noise_prob = (1 / (sqrt_2pi * noise_sigma)
			* exp( -pow(ChannelOutput - ChannelInput, 2.)/ (2. * NoiseVariance())) );

	return noise_prob;
}

double PNC_Channel::SimulateOutput(double ChannelInput)
// Simulate the result of passing the input through the AWGN
// Actually pass ChannelInput from a gaussian channel
{
	return ChannelInput + GaussGenerate(noise_sigma);
}

double PNC_Channel::NoiseVariance() {
	return pow(noise_sigma, 2);
}

double PNC_Channel::NoiseStdDev() {
	return noise_sigma;
}
