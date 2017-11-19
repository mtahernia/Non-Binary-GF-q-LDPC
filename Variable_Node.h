/*
 * Variable_Node.h
 *
 *  Created on: 23 Dec, 2014
 \*      Author: Mehrdad Tahernia
 */

#ifndef VARIABLE_NODE_H_
#define VARIABLE_NODE_H_

#include "Mapping.h"
#include "Message.h"
#include "Node.h" //Parent

class channel;


message &GenerateChannelMessage(GFq v, channel &TransmitChannel, mapping &MapInUse, double ChannelOut);
message &GenerateChannelMessage_PNC(channel &TransmitChannel, mapping &MapInUse, double ChannelOut);

// ----------------- Variable Node ---------------
class variable_node: public node {
public:
	GFq Symbol;     // Value - for encoding
	GFq DecSymbol;     // Value - for decoding
	GFq v;          // coset vector - randomly selected at variable node NOTE: Set to all zero for now
	double SourceData;
	mapping *MapInUse;

	// Auxiliary for Linear Programming method
	int *LCLP_Constraints;
	int CountConstraints;

public:
	message InitialMessage;
	message FinalEstimate;
	message AllImprovementsForChange;       // For use in greedy source coding

public:
	variable_node():SourceData(0),MapInUse(NULL),LCLP_Constraints(NULL),CountConstraints(-1) {
		v.val = 0;
//		uniform_random(GFq::q); //FIXME: Here I have set Coset Vector to zero, previously it was set to a random number
	}
	~variable_node(){/*cout<<"variable_node destructor called\n";*/	}

	void Allocate_LCLP_Constraints(int **ConstraintsStack);
	int Count_LCLP_Constraints();
	void Add_LCLP_Constraint(int variable_index);

	// TODO
	message &CalcRightboundMessage(int rightbound_index);
	void CalcAllRightboundMessages();
	void CalcFinalMessage();
	void Initialize(channel &TransmitChannel, double ChannelOut);
	void SetMapInUse(mapping &p_MapInUse) {MapInUse = &p_MapInUse;}

	double GetZeroSignal() {return MapInUse->map(v);}
	double GetSignal() {return MapInUse->map(v+Symbol);	}
	// For use in greedy source coding

	bool IsRightConnectedTo(node *n);
	bool IsPath3ConnectedTo(node *n);  // Is connected by a path of at most
	virtual node &AdjacentNode(int index);
};


#endif /* VARIABLE_NODE_H_ */
