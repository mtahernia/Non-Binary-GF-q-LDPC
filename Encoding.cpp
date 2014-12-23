/*********************************************************************************
 * The encoder in this file is based on:
 *
 * T. Richardson and R.Urbanke, "Efficient encoding of low-density parity-check codes," IEEE Trans. Inform. Theory, vol. 47,
 * pp. 638--656, February 2001.
 *
 *********************************************************************************/

//#include "LDPC.h"

#include "Functions.h"
#include "GFq.h"
#include "Node.h"
#include "Check_Node.h"
#include "Variable_Node.h"
#include "Edge.h"

#include "Encoding.h"

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
	int SystematicColumns = max(Columns.GetLength() - Rows.GetLength(),	0) + MIN_GAP;

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
			cout	<< "LDPC_Code::GenerateEncoder(): No unknown column was found, while "
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


