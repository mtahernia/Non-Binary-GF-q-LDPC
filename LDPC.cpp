/*
 * LDPC.cpp
 *
 *  Created on: 22 Dec, 2014
 *      Author: Mehrdad Tahernia
 *		  User: mehrdad
 */
#include <cstdlib>
#include "Report.h"
#include "Channel.h"
#include "Encoding.h"
#include "Utils_2.h" // vector, array
#include "Variable_Node.h"
#include "Check_Node.h"
#include "Edge.h"

#include "LDPC.h"

/**************************************************************
 *
 * LDPC Constructors
 *
 **************************************************************/

void LDPC_Code::GetFromFile(std::ifstream &file) {
	char dummy_buffer[10000];
	int rhos_index, lambdas_index;

	rhos_index = lambdas_index = 0;
	bool GotMapInUse = false;

	//----------------------------------------------
	// Go over file
	//----------------------------------------------
	while (!file.eof()) {
		switch (file.peek()) {
		case 'r': // Read Rhos from file
			if (rhos_index >= MAX_RHOS) {
				cout << "LDPC_Code::GetFromFile: MAX_RHOS exceeded\n";
				exit(1);
			}

			file >> rho_degs[rhos_index] >> rho_wts[rhos_index];
			file.getline(dummy_buffer, sizeof(dummy_buffer));  // reach EoL
			rhos_index++;
			break;
		case 'l': // Read Lambdas From File
			if (lambdas_index >= MAX_LAMBDAS) {
				cout << "LDPC_Code::GetFromFile: MAX_LAMBDAS exceeded\n";
				exit(1);
			}

			file >> lambda_degs[lambdas_index] >> lambda_wts[lambdas_index];

			lambdas_index++;
			file.getline(dummy_buffer, sizeof(dummy_buffer));  // reach EoL
			break;
		case 'm': // Read mapping from file
			GotMapInUse = true;
			MapInUse.GetFromFile(file);

			// Initialize GF(q)
			GFq::Initialize(MapInUse.GetQ());
			break;
		default:
			file.getline(dummy_buffer, sizeof(dummy_buffer));   // Skip line and go to beginning of the next line if nothing matches r l m
		}
	}

	if (!GotMapInUse) {
		cout << "mapping not defined\n";
		exit(1);

	}
	rho_degs[rhos_index] = -1;
	rho_wts[rhos_index] = -1;
	lambda_degs[lambdas_index] = -1;
	lambda_wts[lambdas_index] = -1;
}


// Main Constructor of LDPC_Code class
LDPC_Code::LDPC_Code(std::ifstream &File, int p_BlockLength, channel *p_Channel) :
				BlockLength(p_BlockLength), Channel(p_Channel) {
	GetFromFile(File);
}

/*************************************************************************
 *
 *  Calc rate
 *
 *************************************************************************/

double LDPC_Code::sigma_lambda() {
	double n;

	n = 0;

	for (int i = 0; lambda_degs[i] != -1; i++)
		n += lambda_wts[i] / lambda_degs[i];

	return n;
}

double LDPC_Code::sigma_rho() {
	double m;

	m = 0;

	for (int i = 0; rho_degs[i] != -1; i++)
		m += rho_wts[i] / rho_degs[i];

	return m;
}

double LDPC_Code::Calc_Symbol_Rate() {
	double SigmaLambda = sigma_lambda();
	double SigmaRho = sigma_rho();

	return 1 - SigmaRho / SigmaLambda;
}

/************************************************************************
 *
 * LDPC code
 *
 ************************************************************************/

/**
 * Initialize Variable nodes with channel output
 */
void LDPC_Code::Init_Messages(vector &ChannelOutput) {
	if (ChannelOutput.GetSize() != Variables.GetLength()) {
		cout << "LDPC_Code::Init_Messages: Incompatible sizes\n";
		exit(1);
	}

	for (int i = 0; i < Variables.GetLength(); i++)
		Variables[i].Initialize(*Channel, ChannelOutput[i]);
}

void LDPC_Code::GetZeroCodeword(vector &Codeword) {
	Codeword.Allocate(Variables.GetLength());

	for (int i = 0; i < Variables.GetLength(); i++)
		Codeword[i] = Variables[i].GetZeroSignal();
}

void LDPC_Code::GetCodeword(vector &Codeword) {
	Codeword.Allocate(Variables.GetLength());

	for (int i = 0; i < Variables.GetLength(); i++)
		Codeword[i] = Variables[i].GetSignal();
}


void LDPC_Code::Leftbound_Iteration() {
	for (int i = 0; i < Checks.GetLength(); i++)
		Checks[i].CalcAllLeftboundMessages();
}

void LDPC_Code::Rightbound_Iteration() {
	for (int i = 0; i < Variables.GetLength(); i++)
		Variables[i].CalcAllRightboundMessages();
}

void LDPC_Code::FinalIteration() {
	for (int i = 0; i < Variables.GetLength(); i++)
		Variables[i].CalcFinalMessage();
}

double LDPC_Code::Calc_Symbol_Error_Rate() {
	double acc_correct = 0;
	for (int i = 0; i < Variables.GetLength(); i++)
//		acc_correct += Variables[i].FinalEstimate.Decision().IsZero();
		acc_correct += Variables[i].DecSymbol == Variables[i].Symbol;
	return 1.0 - acc_correct / (double) Variables.GetLength();
}

double LDPC_Code::Calc_Rightbound_Symbol_Error_Rate() {
	double acc_correct = 0;

	for (int i = 0; i < Graph.E; i++)
		acc_correct += Graph.edges[i].RightBoundMessage.Decision().IsZero();
	return 1.0 - acc_correct / Graph.E;
}

double LDPC_Code::Belief_Propagation_Decoder(int Count_Iterations) {
	static char buffer[500];
	double Func_RC = 0; //=0 is for initialization of the variable so the compiler won't give warnings
	double LastMin = INF;
	int CountIncreaseIterations = 0;

	cout << "Initial symbol error rate = " << Calc_Symbol_Error_Rate() << "\n";

	for (int i = 0; i < Count_Iterations; i++) {

		Leftbound_Iteration();
		Rightbound_Iteration();

//		Func_RC = Calc_Rightbound_Symbol_Error_Rate(); //FIXME: Not modified for PNC yet
//		sprintf(buffer, "%d: Rightbound SER = %1.10f, %s", i + 1, Func_RC,CharTime());
		Func_RC = Calc_Symbol_Error_Rate();
//		sprintf(buffer, "%d: SER = %1.10f, %s", i + 1, Func_RC,CharTime());
//		cout << buffer;
		cout << ".";

		// Stop when Func_RC doesn't fall for some consecutive iterations
		if (Func_RC < LastMin) {
			LastMin = Func_RC;
			CountIncreaseIterations = 0;
		} else {
			CountIncreaseIterations++;
			if (CountIncreaseIterations > 50){
				sprintf(buffer, "\n%d: SER = %1.10f, %s\n", i + 1, Func_RC,CharTime());
				cout << buffer;
				break;
			}

		}

		if (Func_RC < 1e-7){
			sprintf(buffer, "\n%d: SER = %1.10f, %s\n", i + 1, Func_RC,CharTime());
			cout << buffer;
			break;
		}

	}

	return Func_RC;
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
//	for (int i = 0; i < Systematic; i++)
//	cout << "Var Nodes: " << Variables.GetLength() << "\n";
	for (int i = 0; i < Variables.GetLength(); i++)
		Variables[i].Symbol.val = uniform_random(GFq::q);
}

void LDPC_Code::Get_Symbols(GFq *Symbols)
{
	for (int i = 0; i < Variables.GetLength(); i++)
		*(Symbols+i) = Variables[i].Symbol;
}

void LDPC_Code::Set_Symbols(GFq *Symbols)
{
	for (int i = 0; i < Variables.GetLength(); i++)
		Variables[i].Symbol = *(Symbols+i);
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
		Variables[FirstVarOfTriangle + i].Symbol =	(Checks[i].Value() / label).Minus();
	}
}

int LDPC_Code::GenerateEncoder_WithoutGap() {
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
//		exit(1);
		return 1;
	}

	for (int i = FirstCheckOfGap; i < Checks.GetLength(); i++) {
		Checks[i].Disconnect();
	}

	// Update variables
	Systematic += Gap;
	Gap = 0;
	GapMatrix.deAllocate();
	MinusPhiInverse.deAllocate();
	return 0;
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
	bool Success = false;
	for (int Attempts = 0; Attempts < 10000; Attempts++) {
		matrix phi = GapMatrix.Extract(Systematic, Gap); // Systematic = first col after systematic
		MinusPhiInverse = phi.Inverse();

		if (!MinusPhiInverse.IsNull())    // if success
		{
			MinusPhiInverse.MultiplyByMinusOne();
			Success = true;
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

double LDPC_Code::SumLambda() {
	double sum = 0;
	for (int i = 0; lambda_degs[i] != -1; i++)
		sum += lambda_wts[i];
	return sum;
}

double LDPC_Code::SumRho() {
	double sum = 0;
	for (int i = 0; rho_degs[i] != -1; i++)
		sum += rho_wts[i];
	return sum;
}

double LDPC_Code::Calc_Bit_Rate() {
	return Calc_Symbol_Rate() * log((double) GFq::q) / log(2.);
}

void LDPC_Code::MakeLambdasValid()   /// Make lambdas sum = 1
{
	double sum = SumLambda();
	for (int i = 0; lambda_degs[i] != -1; i++)
		lambda_wts[i] /= sum;
}

void LDPC_Code::MakeRhosValid()   /// Make rhos sum = 1
{
	double sum = SumRho();
	for (int i = 0; rho_degs[i] != -1; i++)
		rho_wts[i] /= sum;
}


int LDPC_Code::CountLambdaDegs() {
	int count;
	for (count = 0; lambda_degs[count] != -1; count++)
		;
	return count;
}

int LDPC_Code::CountRhoDegs() {
	int count;
	for (count = 0; rho_degs[count] != -1; count++)
		;
	return count;
}

void LDPC_Code::ResetGraph() {
	Graph.Reset(BlockLength, lambda_degs, lambda_wts, rho_degs, rho_wts, MapInUse);
	Variables.Init(Graph.variable_nodes, Graph.N);
	Checks.Init(Graph.check_nodes, Graph.M);
}
