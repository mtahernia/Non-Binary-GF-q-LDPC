/*********************************************************************************
 * The encoder in this file is based on:
 *
 * T. Richardson and R.Urbanke, "Efficient encoding of low-density parity-check codes," IEEE Trans. Inform. Theory, vol. 47,
 * pp. 638--656, February 2001.
 *
 *********************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <wchar.h>
#include "LDPC_1.h"
#include "LDPC_2.h"
#include "Utils_1.h"
#include "Encoding.h"

/*********************************************************************************
 *
 * Utility
 *
 *********************************************************************************/

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

/*********************************************************************************
 *
 * Node lists
 *
 *********************************************************************************/

void NodeListWithID::SwitchNodes(int i1, int i2) {
	node *Aux;

	Aux = Nodes[i1];
	Nodes[i1] = Nodes[i2];
	Nodes[i2] = Aux;

	// Update ID
	Nodes[i1]->SetID(i1);
	Nodes[i2]->SetID(i2);
}

void VariableNodeList::Init(variable_node VariableNodeArray[], int p_Length) {
	Allocate(p_Length);
	for (int i = 0; i < p_Length; i++) {
		Nodes[i] = &(VariableNodeArray[i]);
		Nodes[i]->SetID(i);
	}
	CurrentLength = p_Length;
}

void CheckNodeList::Init(check_node CheckNodeArray[], int p_Length) {
	Allocate(p_Length);
	int index = 0;

	for (int i = 0; i < p_Length; i++) {
		if (CheckNodeArray[i].GetDegree() != 0) // Ignore the rare case of degree = 0
				{
			Nodes[index] = &(CheckNodeArray[i]);
			Nodes[index]->SetID(index);
			index++;
		}
	}
	CurrentLength = index;
}

void EraseKnown(node &KnownColumn, NodeListWithoutID &DegreeOneRows)
// "Erase" a known column
		{
	for (int i = 0; i < KnownColumn.degree; i++) {
		node &AdjacentRow = KnownColumn.AdjacentNode(i);
		AdjacentRow.AuxDegree--;

		if (AdjacentRow.AuxDegree == 1)
			DegreeOneRows.Add(AdjacentRow);
	}
}

/*********************************************************************************
 *
 * Urbanke's Algorithm AH
 *
 *********************************************************************************/

// If gap ends up too big, it might be necessary to modify the following 3 to 300 or larger
#define MIN_GAP   3
#define MAX_GAP   10

void UrbankeAlgorithmAH(NodeListWithID &Columns, NodeListWithID &Rows) {
	//------------------------------------------------------
	// Init degrees
	//------------------------------------------------------
	for (int i = 0; i < Rows.GetLength(); i++)
		Rows[i].AuxDegree = Rows[i].degree;

	// data structure for degree one nodes
	NodeListWithoutID DegreeOneRows;
	DegreeOneRows.Allocate(Rows.GetLength());

	//------------------------------------------------------
	// Initialization: select known columns
	//-----------------------------------------------------
	int SystematicColumns = max(Columns.GetLength() - Rows.GetLength(),
			0) + MIN_GAP;

	for (int i = 0; i < SystematicColumns; i++) {
		// Select a known column among remaining columns
		int Known = uniform_random(Columns.GetLength() - i) + i;
		Columns.SwitchNodes(Known, i);          // Move known column to start 
		EraseKnown(Columns[i], DegreeOneRows);  // "Erase" known column
	}

	int FirstNonSystematic = SystematicColumns;

	//------------------------------------------------------
	// Diagonal extension steps
	//------------------------------------------------------
	int DegreeOneIndex = 0;
	int StartUnknown = FirstNonSystematic;
	for (; StartUnknown < Columns.GetLength(); StartUnknown++) {
		// Find next degree one row
		// Skip degree one rows which may have turned to zero
		for (; DegreeOneIndex < DegreeOneRows.GetLength(); DegreeOneIndex++)
			if (DegreeOneRows[DegreeOneIndex].AuxDegree == 1)
				break;

		if (DegreeOneIndex == DegreeOneRows.GetLength())
			break;       // If not finished and no degree one nodes remain

		node &CurrentDegreeOneRow = DegreeOneRows[DegreeOneIndex++];
		int CurrentRowNumber = CurrentDegreeOneRow.GetID();

		// Find the single unknown column (degree 1 refers to unknown connected nodes)
		int NewColumnToBeKnown = -1;
		for (int i = 0; i < CurrentDegreeOneRow.GetDegree(); i++) {
			int ColumnNumber = CurrentDegreeOneRow.AdjacentNode(i).GetID();
			if (ColumnNumber >= StartUnknown) {
				NewColumnToBeKnown = ColumnNumber;
				break;
			}
		}

		if (NewColumnToBeKnown == -1) // Assertion - an unknown column must be found
				{
			cout
					<< "LDPC_Code::GenerateEncoder(): No unknown column was found, while "
					<< "because degree of row = 1, by definition one should have been found\n";
			exit(1);
		}

		Columns.SwitchNodes(NewColumnToBeKnown, StartUnknown); // Make new known column StartUnknown 
		Rows.SwitchNodes(CurrentRowNumber, StartUnknown - FirstNonSystematic);
		EraseKnown(Columns[StartUnknown], DegreeOneRows); // "Erase" new known node
	}

	//-------------------------------------------------------------
	// Generate gap by moving remaining unknowns down
	//-------------------------------------------------------------
	int LowerTriangularLength = StartUnknown - FirstNonSystematic;
	for (int i1 = Columns.GetLength() - 1, i2 = StartUnknown - 1;
			i2 >= FirstNonSystematic; i1--, i2--) {
		Columns.SwitchNodes(i1, i2);
	}

	//-------------------------------------------------------------
	// Record gap
	//-------------------------------------------------------------
	Columns.Gap = Columns.GetLength() - LowerTriangularLength;
	Rows.Gap = Rows.GetLength() - LowerTriangularLength;

//   cout << "SystematicColumns = " << SystematicColumns << "\n"
//      << "StartUnknown = " << StartUnknown << "\n"
//      << "Columns.GetLength() = " << Columns.GetLength() << "\n"
//      << "Columns gap = " << Columns.Gap << "\n"
//      << "Rows.GetLength() = " << Rows.GetLength() << "\n"
//      << "Rows gap = " << Rows.Gap << "\n";
}

/*********************************************************************************
 *
 * Encoder
 *
 *********************************************************************************/

void LDPC_Code::GenerateRandomSystematic() {
	//------------------------------------------------------
	// Randomly select values for systematic digits
	//------------------------------------------------------
	for (int i = 0; i < Systematic; i++)
		Variables[i].Symbol.val = uniform_random(GFq::q);
}

void LDPC_Code::Encode() {
	int FirstVarOfTriangle = Systematic + Gap;

	//------------------------------------------------------
	// Determine gap symbols
	//------------------------------------------------------
	if (Gap > 0) {
		// Multiply systematic vector multiplied by GapMatrix
		matrix Vals(Gap);          // a column vector for results
		for (int i = 0; i < Gap; i++) {
			Vals[i] = 0;
			for (int j = 0; j < Systematic; j++)
				Vals[i] += Variables[j].Symbol * GapMatrix.Element(i, j);
		}

		matrix GapVars(Gap);
		GapVars = MinusPhiInverse * Vals;

		// place vals
		for (int i = 0; i < Gap; i++)
			Variables[Systematic + i].Symbol = GapVars[i];
	}

	//------------------------------------------------------
	// Determine all the rest
	//------------------------------------------------------
	for (int i = 0; i < Triangle; i++) {
		// Calculate value of check node without symbol
		Variables[FirstVarOfTriangle + i].Symbol = 0;
		GFq label = Checks[i].Element(FirstVarOfTriangle + i);
		Variables[FirstVarOfTriangle + i].Symbol =
				(Checks[i].Value() / label).Minus();
	}
}

void LDPC_Code::GenerateEncoder_WithoutGap() {
	//------------------------------------------------------
	// Approximate lower triangularize
	//------------------------------------------------------
	// Init 
	Variables.Init(Graph.variable_nodes, Graph.N);
	Checks.Init(Graph.check_nodes, Graph.M);

	UrbankeAlgorithmAH(/* columns */Checks, /* rows */Variables);

	// Reverse because we have made checks the columns and variables the rows
	Variables.Reverse();
	Checks.Reverse();

	// Determine values
	Systematic = Variables.GetLength() - Checks.GetLength();
	Gap = Checks.Gap;
	Triangle = Variables.GetLength() - Systematic - Gap;

	int FirstCheckOfGap = Checks.GetLength() - Gap;

	//------------------------------------------------------
	// Eliminate gap check nodes
	//------------------------------------------------------
	if (Gap > MAX_GAP) {
		cout << "GenerateEncoder_WithoutGap: Gap = " << Gap << " too big\n";
		exit(1);
	}

	for (int i = FirstCheckOfGap; i < Checks.GetLength(); i++) {
		Checks[i].Disconnect();
	}

	// Update variables
	Systematic += Gap;
	Gap = 0;
	GapMatrix.deAllocate();
	MinusPhiInverse.deAllocate();
}

void LDPC_Code::GenerateEncoder() {
	//------------------------------------------------------
	// Approximate lower triangularize
	//------------------------------------------------------
	// Init
	Variables.Init(Graph.variable_nodes, Graph.N);
	Checks.Init(Graph.check_nodes, Graph.M);

	UrbankeAlgorithmAH(/* columns */Checks, /* rows */Variables);

	// Reverse because we have made checks the columns and variables the rows
	Variables.Reverse();
	Checks.Reverse();

	// Determine values
	Systematic = Variables.GetLength() - Checks.GetLength();
	Gap = Checks.Gap;
	Triangle = Variables.GetLength() - Systematic - Gap;

	cout << "GenerateEncoder: Gap = " << Gap << "\n";

	//ofstream bbbTemp("Temp.txt");
	//for (int i = 0; i < Variables.GetLength(); i++)
	//   for (int j = 0; j < Variables[i].GetDegree(); j++)
	//      bbbTemp << Variables[i].AdjacentNode(j).GetID() << " " << i << "\n";
	//bbbTemp.close();
	//exit(0);

	int FirstCheckOfGap = Checks.GetLength() - Gap;
	int FirstVarOfTriangle = Systematic + Gap;

	//------------------------------------------------------
	// Handle gap
	//------------------------------------------------------
	// Init gap matrix
	GapMatrix.Init(Gap, Variables.GetLength());

	for (int i = 0; i < Gap; i++) {
		//cout << FirstCheckOfGap + i << ">" << Checks[FirstCheckOfGap + i].GetID() << " " << Checks[FirstCheckOfGap + i].degree << "\n";
		GapMatrix.Set(i, Checks[FirstCheckOfGap + i]);
	}

	//ofstream bbbTemp2("Temp.txt");
	//GapMatrix.SparseOut(bbbTemp2);
	//bbbTemp2.close();
	//exit(0);

	// Gaussian elimination 
	for (int j = Triangle - 1; j >= 0; j--)      // column
			{
		// Find element appropriate matrix element
		int EliminatedCol = FirstVarOfTriangle + j;   // Column to be eliminated
		check_node &CheckRow = Checks[j]; // j'th check should be nonzero at j'th column
		GFq EliminatingRowVal = CheckRow.Element(EliminatedCol); // Value of row at column to be eliminated

		for (int i = 0; i < GapMatrix.M; i++)     // row of GapMatrix
				{
			GFq ValToBeEliminated = GapMatrix.Element(i, EliminatedCol);
			if (!ValToBeEliminated.IsZero()) {
				GFq Mult = (ValToBeEliminated / EliminatingRowVal).Minus();
				GapMatrix.Add(i, CheckRow, Mult);
			}
		}
	}

	// obtain phi^-1
	// While matrix is singular, switch columns
	BOOLEAN Success = FALSE;
	for (int Attempts = 0; Attempts < 10000; Attempts++) {
		matrix phi = GapMatrix.Extract(Systematic, Gap); // Systematic = first col after systematic
		MinusPhiInverse = phi.Inverse();

		if (!MinusPhiInverse.IsNull())    // if success
		{
			MinusPhiInverse.MultiplyByMinusOne();
			Success = TRUE;
			break;
		} else {  // switch columns randomly
			int i1 = uniform_random(Systematic);
			int i2 = uniform_random(Gap) + Systematic;

			GapMatrix.SwitchColumns(i1, i2);
			Variables.SwitchNodes(i1, i2);
		}
	}

	if (!Success) {
		cout
				<< "Unable to find an invertible phi, maybe increase number of attempts\n";
		exit(1);
	}
}

