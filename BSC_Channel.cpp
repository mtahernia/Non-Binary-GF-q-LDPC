/*
 * BSC_Channel.cpp
 *
 *  Created on: 23 Dec, 2014
 *      Author: mehrdad
 */
#include "LDPC.h" //LDPC Code
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

double H2(double p) {
	return -p * log2(p) - (1 - p) * log2(1 - p);
}

double H2Reverse(double H) {
	double pmin = 0, pmax = 0.5, tol = 0.0000001, pmid;

	if (H == 0)
		return 0;
	else if (H == 1.)
		return 0.5;

	while ((pmax - pmin) > tol) {
		pmid = (pmin + pmax) / 2.;
		if (H2(pmid) > H)
			pmax = pmid;
		else
			pmin = pmid;
	}

	return pmid;
}

double BSC_Channel::CapacityInBits() {
	return 1 - H2(channel_p);
}

void BSC_Channel::PrintChannelData(LDPC_Code &Code) {
	double BitRate;

	BitRate = Code.Calc_Bit_Rate();
	cout << "channel_p = " << channel_p
			<< "\nCapacity (bits per channel use) = " << 1 - H2(channel_p)
			<< " Max channel_p for Bit rate: " << H2Reverse(1. - BitRate);
}
