/*
 * Matrix.cpp
 *
 *  Created on: 22 Dec, 2014
 *      Author: Mehrdad Tahernia
 *		  User: mehrdad
 */

#include "Definitions.h"
#include "Functions.h"
#include "Check_Node.h"
#include "Variable_Node.h"
#include "Edge.h"
#include "Matrix.h"
/************************************************************************************
 *
 * GF(q) Matrix
 *
 ************************************************************************************/
/// Returns Identity matrix of size N
matrix &Identity(int N) {
	static matrix I;

	I.Init(N, N);
	for (int i = 0; i < N; i++)
		I.Element(i, i) = 1;

	return I;
}


/// Returns the inverse of this matrix using Gauss-Jordan Method in GF(q)
matrix &matrix::Inverse() {
	static matrix Result;

	if (N != M) {
		cout << "Attempt to invert a nonsquare matrix\n";
		exit(1);
	}

	matrix Aux(*this);         // temporary storage for this
	Result = Identity(N);      // temporary result

	//------------------------------------------------------------
	// Go through each column and eliminate unwanted rows
	//------------------------------------------------------------
	for (int j = 0; j < N; j++) {		// counter for columns --->
		// find a remaining row where column is non zero
		int NonZeroRow = -1;
		for (int i = j; i < N; i++) {
			if (!Aux.Element(i, j).IsZero()) {
				NonZeroRow = i;
				break;
			}
		}

		if (NonZeroRow == -1) {
			Result.SetNull();          // if matrix is singular
			return Result;
		}

		if (NonZeroRow != j)          // Move row to desired position
				{
			Aux.SwitchRows(j, NonZeroRow);
			Result.SwitchRows(j, NonZeroRow);
		}

		// Divide so that leading value is a one
		GFq Multiplier = Aux.Element(j, j).Inverse();
		Aux.MultRow(j, Multiplier);
		Result.MultRow(j, Multiplier);

		// Eliminate column at all other rows
		for (int i = 0; i < N; i++) {
			if (i == j)
				continue;         // skip row at current column
			if (!Aux.Element(i, j).IsZero()) {
				GFq Eliminator = Aux.Element(i, j).Minus();
				Aux.AddRow(i, j, Eliminator);
				Result.AddRow(i, j, Eliminator);
			}
		}

	}

	return Result;
}

// FIXME: The following functions work on check nodes, the role is not clear to me yet
void matrix::Add(int row, check_node &Check, GFq Mult) {
	for (int i = 0; i < Check.GetDegree(); i++) {
		int VarID = Check.GetEdge(i).LeftNode().GetID();
		GFq label = Check.GetEdge(i).label;
		Element(row, VarID) += Mult * label;
	}
}



void matrix::Set(int row, check_node &Check) {
	for (int i = 0; i < Check.GetDegree(); i++) {
		int VarID = Check.GetEdge(i).LeftNode().GetID();
		GFq label = Check.GetEdge(i).label;
		Element(row, VarID) = label;
	}
}




