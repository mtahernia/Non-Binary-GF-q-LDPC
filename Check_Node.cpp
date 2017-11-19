/*
 * Check_Node.cpp
 *
 *  Created on: 23 Dec, 2014
 \*      Author: Mehrdad Tahernia
 */
#include "Message.h"
#include "Edge.h"
#include "Variable_Node.h"
#include "Check_Node.h"

/****************************************************************************
 *
 * Check node
 *
 ****************************************************************************/

GFq &check_node::Value() {
	static GFq Aux;

	Aux = 0;
	for (int i = 0; i < GetDegree(); i++)
		Aux += GetEdge(i).label * GetEdge(i).LeftNode().Symbol;

	return Aux;
}

// returns the variable node connected to edge specified with index
node &check_node::AdjacentNode(int index) {
	return GetEdge(index).LeftNode();
}

message &FastCalcLeftboundMessage(message AuxLeft[], message AuxRight[],int left_index, int degree)
{
	static message Aux;

	// Init to delta pulse
	Aux.Set_q(GFq::q);

	Aux = AuxLeft[left_index];
	Aux *= AuxRight[left_index];

// FIXME: Why is this normalization not important?
//	if (GFq::IsPrimeQ)
//		Aux /= (double) GFq::q;

	Aux.IDFT();
	Aux.Reverse();	// estimate of symbol value = (- sum of others)

	return Aux;
}

void check_node::CalcAllLeftboundMessages() {
	static message Vectors[MAX_DEGREE];

	static message AuxLeft[MAX_DEGREE];
	static message AuxRight[MAX_DEGREE];
	static GFq ZERO(0);

	// Calc auxiliary DFTs
	for (int i = 0; i < GetDegree(); i++) {
		Vectors[i] = GetEdge(i).RightBoundMessage;
		Vectors[i].PermuteTimes(GetEdge(i).label.Inverse());
	}

	//-------------------------------------------------------------
	// If power of two - use DFT2
	//-------------------------------------------------------------
//	if (!GFq::IsPrimeQ)
	if (true)
	{
		for (int i = 0; i < GetDegree(); i++) {
			Vectors[i].DFT();
		}

		// Calc auxiliary values
		AuxLeft[0].Set_q(GFq::q);
		AuxLeft[0] = 1.0;
		for (int i = 1; i < GetDegree(); i++) {
			AuxLeft[i] = AuxLeft[i - 1];
			AuxLeft[i] *= Vectors[i - 1];
		}

		AuxRight[GetDegree() - 1].Set_q(GFq::q);
		AuxRight[GetDegree() - 1] = 1.0;
		for (int i = GetDegree() - 2; i >= 0; i--) {
			AuxRight[i] = AuxRight[i + 1];
			AuxRight[i] *= Vectors[i + 1];
		}

		// Calc leftbound messages
		for (int i = 0; i < GetDegree(); i++) {
			GetEdge(i).LeftBoundMessage = FastCalcLeftboundMessage(AuxLeft,	AuxRight, i, GetDegree());
			GetEdge(i).LeftBoundMessage.PermuteTimes(GetEdge(i).label);
		}
	}
/*
	else {
//		cout << "FIXME!!!" << std::endl;
		for (int i = 0; i < GetDegree(); i++) {
			Vectors[i].DFT2();
		}

		// Calc auxiliary values
		AuxLeft[0].Set_q(GFq::q);
		AuxLeft[0] = 1.0;
		for (int i = 1; i < GetDegree(); i++) {
			AuxLeft[i] = AuxLeft[i - 1];
			AuxLeft[i] *= Vectors[i - 1];
		}

		AuxRight[GetDegree() - 1].Set_q(GFq::q);
		AuxRight[GetDegree() - 1] = 1.0;
		for (int i = GetDegree() - 2; i >= 0; i--) {
			AuxRight[i] = AuxRight[i + 1];
			AuxRight[i] *= Vectors[i + 1];
		}

		// Calc leftbound messages
		for (int i = 0; i < GetDegree(); i++) {
			GetEdge(i).LeftBoundMessage = FastCalcLeftboundMessage(AuxLeft,	AuxRight, i, GetDegree());
			GetEdge(i).LeftBoundMessage.PermuteTimes(GetEdge(i).label);


		}

	} */
}
// Used for encoding
GFq &check_node::Element(int index)
// Treats check node as a sparse matrix column
		{
	static GFq ReturnVal(0);                    // Value of check at column

	for (int j = 0; j < GetDegree(); j++) {
		if (GetEdge(j).LeftNode().GetID() == index) {
			ReturnVal = GetEdge(j).label;
			break;
		}
	}

	return ReturnVal;
}

