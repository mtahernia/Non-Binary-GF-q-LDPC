/*
 * Variable_Node.cpp
 *
 *  Created on: 23 Dec, 2014
 *      Author: mehrdad
 */
#include "Message.h"
#include "LDPC_2.h"
#include "Variable_Node.h"



/*************************************************************************
 *
 * Variable node
 *
 *************************************************************************/

node &variable_node::AdjacentNode(int index) {
	return GetEdge(index).RightNode();
}

message &variable_node::CalcRightboundMessage(int rightbound_index)
// rightbound_index is -1 if calculation is meant for final estimate
{
	static message Aux;

	Aux = InitialMessage;

	for (int i = 0; i < GetDegree(); i++) {
		// ignore intrinsic information
		if (i == rightbound_index)
			continue;

		Aux *= GetEdge(i).LeftBoundMessage;
	}

	Aux.Normalize();

	return Aux;
}

void variable_node::CalcFinalMessage() {
	FinalEstimate = CalcRightboundMessage( /* NO rightbound index */-1);
	DecSymbol = FinalEstimate.Decision();
}

void variable_node::CalcAllRightboundMessages() {
	for (int i = 0; i < GetDegree(); i++)
		GetEdge(i).RightBoundMessage = CalcRightboundMessage(/* rightbound index */i);

	CalcFinalMessage();
}

// Generates channel message for zero codeword transmission
message &GenerateChannelMessage(GFq v, channel &TransmitChannel, mapping &MapInUse, double ChannelOut) {
	static message InitialMessage;
	int q = MapInUse.GetQ();
	double CandidateIn;
//	double ChannelIn;

//	ChannelIn = MapInUse.map(v);        // mapping of (0 + v) % q; FIXME: This is not used

	// Generate InitialMessage
	InitialMessage.Set_q(q);
	for (GFq i(0); i.val < q; i.val++) {
		CandidateIn = MapInUse.map(i + v);
		InitialMessage[i.val] = TransmitChannel.CalcProbForInput(ChannelOut, CandidateIn);
	}

	InitialMessage.Normalize();    // Make valid probability vector

	return InitialMessage;
}

void variable_node::Initialize(channel &TransmitChannel, double ChannelOut) {
	InitialMessage = GenerateChannelMessage(v, TransmitChannel, *MapInUse,ChannelOut);
	FinalEstimate = InitialMessage;

	// Generate Rightbound messages
	for (int j = 0; j < degree; j++)
		GetEdge(j).RightBoundMessage = InitialMessage;

}

BOOLEAN variable_node::IsRightConnectedTo(node *n) {
	for (int i = 0; i < degree; i++)
		if (&edges[i]->RightNode() == n)
			return TRUE;

	return FALSE;
}
