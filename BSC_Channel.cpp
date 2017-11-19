/*
 * BSC_Channel.cpp
 *
 *  Created on: 23 Dec, 2014
 \*      Author: Mehrdad Tahernia
 */
#include <cstdlib>
#include "Definitions.h"
#include "LDPC.h" //LDPC Code
#include "Mapping.h" //Mapping (also was included in LDPC.h)
#include "GFq.h" // included from LDPC->Matrix->GFq
#include "BSC_Channel.h"



/*********************************************************************************
 *
 * BSC Channel
 *
 *********************************************************************************/

void BSC_Channel::ProcessMapping(LDPC_Code &Code) {
	mapping &MapInUse = Code.MapInUse;

	for (GFq i(0); i.val < MapInUse.q; i.val++)
		if ((MapInUse.map(i) != 0) && (MapInUse.map(i) != 1)) {
			cout << "mapping levels must be (0,1)\n";
			exit(1);
		}
}

double BSC_Channel::CapacityInBits() {
	return 1 - Hb(channel_p);
}

void BSC_Channel::PrintChannelData(LDPC_Code &Code) {
	double BitRate;

	BitRate = Code.Calc_Bit_Rate();
	cout << "channel_p = " << channel_p
			<< "\nCapacity (bits per channel use) = " << 1 - Hb(channel_p)
			<< " Max channel_p for Bit rate: " << HbInverse(1. - BitRate);
}

double BSC_Channel::SimulateOutput(double ChannelInput)
{
	return (double) (int(ChannelInput) ^ Flip(channel_p));
}

double BSC_Channel::CalcProbForInput(double ChannelOutput, double ChannelInput) {
	if (ChannelOutput == ChannelInput)
		return 1. - channel_p;
	else
		return channel_p;
}

double BSC_Channel::CalcProbForOutput(double ChannelOutput) {
	if (ChannelOutput == 1)
		return source_p;
	else if (ChannelOutput == 0)
		return 1 - source_p;
	else {
		cout << "BSC_Channel.CalcProbForOutput: Invalid parameter\n";
		exit(1);
	}
}
