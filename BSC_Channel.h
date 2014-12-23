/*
 * BSC_Channel.h
 *
 *  Created on: 23 Dec, 2014
 *      Author: mehrdad
 */

#ifndef BSC_CHANNEL_H_
#define BSC_CHANNEL_H_

#include <cstdlib>
#include "Channel.h"
class LDPC_Code; //defined in LDPC.h, but header is not needed so we use forward declerations

/****************************************************************************
 *
 * BSC Channel
 *
 ****************************************************************************/

/**
 * BSC Channel, which is a child of Channel.
 * It is only useful for binary channels
 */
class BSC_Channel: public channel {
private:
	double channel_p;			///< Bit flip probability for BSC
	double source_p;				///< Input source
	double InterferenceProb; ///< Dirty paper, I guess this value is unused here

public:
	// General functions ------------------------------------------

	BSC_Channel() :
			channel_p(0), InterferenceProb(0.5) {
	}
	void SetChannel_p(double p_channel_p) {
		channel_p = p_channel_p;
	}
	int Flip(double prob) {
		return (my_rand() < prob);
	}
	const char *GetChannelName() {
		return "BSC_Channel";
	}

	virtual void PrintChannelData(LDPC_Code &Code);
	virtual void ProcessMapping(LDPC_Code &Code);

	// Channel coding functions -----------------------------------
	virtual double SimulateOutput(double ChannelInput) {
		return (double) (int(ChannelInput) ^ Flip(channel_p));
	}
	virtual double CalcProbForInput(double ChannelOutput, double ChannelInput) {
		if (ChannelOutput == ChannelInput)
			return 1. - channel_p;
		else
			return channel_p;
	}

	// Statistical functions -------------------------------------
	virtual double CapacityInBits();
	virtual double CalcProbForOutput(double ChannelOutput) {
		if (ChannelOutput == 1)
			return source_p;
		else if (ChannelOutput == 0)
			return 1 - source_p;
		else {
			cout << "BSC_Channel.CalcProbForOutput: Invalid parameter\n";
			exit(1);
		}
	}
};


#endif /* BSC_CHANNEL_H_ */
