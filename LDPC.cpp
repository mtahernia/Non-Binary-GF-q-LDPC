/*
 * LDPC.cpp
 *
 *  Created on: 22 Dec, 2014
 *      Author: Mehrdad Tahernia
 *		  User: mehrdad
 */
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
	BOOLEAN GotMapInUse = FALSE;

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
			GotMapInUse = TRUE;
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

//		Func_RC = Calc_Rightbound_Symbol_Error_Rate();
		Func_RC = Calc_Symbol_Error_Rate();
		sprintf(buffer, "%d: Rightbound SER = %1.10f, %s", i + 1, Func_RC,CharTime());
		cout << buffer;

		// Stop when Func_RC doesn't fall for some consecutive iterations
		if (Func_RC < LastMin) {
			LastMin = Func_RC;
			CountIncreaseIterations = 0;
		} else {
			CountIncreaseIterations++;
			if (CountIncreaseIterations > 50)
				break;
		}

		if (Func_RC < 1e-7)
			break;
	}

	return Func_RC;
}
