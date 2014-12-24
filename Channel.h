/*
 * Channel.h
 *
 *  Created on: 18 Dec, 2014
 *      Author: Mehrdad Tahernia
 */

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <cstdlib>

class GFq;
class vector;
class LDPC_Code; //defined in LDPC.h, but header is not needed so we use forward declerations
class mapping; // defined down in this file

/// Channel Class
/**
 * General Channel Class
 * More specific channel classes will be defined as children of this classes
 */
class channel {
public:
	// Virtual destructor
	virtual ~channel(void) {/*cout << "channel destructor called\n"; */}

	virtual const char *GetChannelName() = 0;
	// General functions ------------------------------------------
	GFq &MaxProbableForOutput(mapping &MapInUse); // FIXME: Not used or defined in this program
	double ErrorUndefined(char *FuncName){cout << GetChannelName() << "::" << FuncName << " undefined.\n";exit(1);}
	virtual void PrintChannelData(LDPC_Code &Code) = 0; /*!<This is a pure virtual function, meaning the child classes should define it or it will retun zero if not defined */
	virtual void ProcessMapping(LDPC_Code &Code) = 0;
	// Channel functions-------------------------------------------
	void SimulateOutputVector(vector &InVector, vector &OutVector);
	virtual double SimulateOutput(double ChannelInput) = 0;
	virtual double CalcProbForInput(double ChannelOutput, double ChannelInput) = 0;
	// Statistical data -------------------------------------------
	virtual double CapacityInBits() = 0;
};

#endif /* CHANNEL_H_ */
