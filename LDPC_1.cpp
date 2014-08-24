#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "LDPC_1.h"
#include "LDPC_2.h"
#include "Utils_1.h"
#include <ctype.h>
#include <wchar.h>

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

/**************************************************************
 *
 * mapping read from file
 * 
 **************************************************************/

void mapping::GetFromFile(std::ifstream &file)
// Read the mapping from the current position in the file
{
	file >> q;

	if (q > MAX_Q) {
		cout << "q exceeds MAX_Q in mapping:GetFromFile\n";
		exit(1);
	}

	for (int i = 0; i < q; i++) {
		file >> vals[i];
	}
}

