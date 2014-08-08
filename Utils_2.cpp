#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <wchar.h>
#include "Utils_1.h"
#include "Utils_2.h"
#include "LDPC_2.h"

/************************************************************************
 *
 * GF(q)
 *
 ************************************************************************/

// Static integers
int GFq::q = -1;
GFq GFq::alpha[MAX_Q];
int GFq::reverse_alpha[MAX_Q];
GFq GFq::inverse[MAX_Q];
BOOLEAN GFq::IsPrimeQ = FALSE;
BOOLEAN GFq::IsModuloOperations = FALSE;


BOOLEAN IsPrime(int num) {
	BOOLEAN Reply = TRUE;

	for (int i = 2; i < num; i++) {
		if ((num % i) == 0) {
			Reply = FALSE;
			break;
		}
	}

	return Reply;
}

// Kind of declaration that these exist
int GFq::log_2_q;
int GFq::mask;


void GFq::Initialize(int p_q) {
	if (p_q == GFq::q)         // if already initialized with the same q
		return;

	if (p_q > MAX_Q) {
		cout << "GFq::Initialize: p_q exceeds MAX_Q\n";
		exit(1);
	}

	q = p_q;

	//-----------------------------------------------------------------------
	// Initialize 
	//-----------------------------------------------------------------------

	if ((q > 2) && IsPowerOfTwo(q)) {
		// Store for use by other utilities
		log_2_q = Intlog2(q);
		mask = 0;								// Mask is used to select bits.
		for (int i = 0; i < log_2_q; i++) {
			mask <<= 1;
			mask |= 1;
		}

		switch (q) {
		case 4:
			GenerateAlphas(2);
			break;
		case 8:
			GenerateAlphas(3);
			break;
		case 16:
			GenerateAlphas(4);
			break;
		case 32:
			GenerateAlphas(5);
			break;
		case 64:
			GenerateAlphas(6);
			break;
		case 256:
			GenerateAlphas(8);
			break;
		default:
			cout << "GFq::Initialize: Unsupported value of q\n";
			exit(1);
		}

		IsPrimeQ = FALSE;
		IsModuloOperations = FALSE;
	} else {
		if (q == 2) {
			log_2_q = 1;
			mask = 1;
		}
		IsModuloOperations = TRUE;
		IsPrimeQ = IsPrime(q);
	}

	//-----------------------------------------------------------------------
	// Calc inverse table for elemets of GF(q)
	//-----------------------------------------------------------------------

	for (int i = 1; i < q; i++)
		for (int j = 1; j < q; j++) {
			GFq g1(i), g2(j);
			if ((g1 * g2) == GFq::One())
				inverse[i] = g2;
		}
}  // void GFq::Initialize




// Some predefined generator polynomials for extension fields of 2.
void GFq::GenerateAlphas(int m) {
	int generator_polynomial;

	int X0 = 1, X1 = 1 << 1, X2 = 1 << 2, X3 = 1 << 3, X4 = 1 << 4, X5 = 1 << 5, X6 = 1 << 6,/* X7 = 1 << 7,*/ X8 = 1 << 8; // X7 was not used

	switch (m) {
	case 2:
		generator_polynomial = X0 ^ X1 ^ X2;  // ^ is Bitwise XOR, it acts like modulo 2 adding here
		break;
	case 3:
		generator_polynomial = X0 ^ X1 ^ X3;
		break;
	case 4:
		generator_polynomial = X0 ^ X1 ^ X4;
		break;
	case 5:
		generator_polynomial = X0 ^ X2 ^ X5;
		break;
	case 6:
		generator_polynomial = X0 ^ X1 ^ X6;
		break;
	case 8:
		generator_polynomial = X8 ^ X4 ^ X3 ^ X2 ^ X0;
		break;
	default:
		cout << "GFq::GenerateAlphas: Unidentified Galois field\n";
		exit(1);
		break;
	}

	//------------------------------------------------
	// Generate alphas
	//------------------------------------------------
	int x = 1;
	int overflow = 1 << m;
	for (int i = 0; i < (q - 1); i++) {
		alpha[i].val = x;

		x <<= 1;                // multiply by alpha FIXME: What!!!!!!!!!!!! it looks like multiplication by 2
		if (x & overflow)       // if overflowed
			x ^= generator_polynomial;	// FIXME: I don't understand, why add the generator?
	}

	//------------------------------------------------
	// Generate reverse alphas: inverse function
	//------------------------------------------------
	for (int i = 0; i < (q - 1); i++)
		reverse_alpha[alpha[i].val] = i;
}

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

	matrix Aux(*this);               // temporary storage for this
	Result = Identity(N);      // temporary result

	//------------------------------------------------------------
	// Go through each column and eliminate unwanted rows
	//------------------------------------------------------------
	for (int j = 0; j < N; j++) {
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
