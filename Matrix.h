/*
 * Matrix.h
 *
 *  Created on: 22 Dec, 2014
 *      Author: Mehrdad Tahernia
 *		  User: mehrdad
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include "GFq.h"

/************************************************************************
 *
 * GF(q) matrix
 *
 ************************************************************************/
// Forward decleration of check_node class, in cpp file, the definition will be included (LDPC_2.h)
class check_node;

class matrix {
public:
	GFq *Elements;
	int M, N;
	int TotalSize;
public:

	matrix() :
			Elements(NULL), M(0), N(0), TotalSize(0) {
	}

	matrix(matrix &M) :
			Elements(NULL), M(0), N(0), TotalSize(0) {
		*this = M;
	}

	matrix &operator=(matrix &A) {
		Init(A.M, A.N);
		for (int i = 0; i < TotalSize; i++)
			Elements[i] = A.Elements[i];

		return *this;
	}

	// Extract column ColFirst up to ColFirst+CountCols-1
	matrix &Extract(int ColFirst, int CountCols) {
		static matrix A;

		A.Init(M, CountCols);
		for (int j = 0; j < CountCols; j++)
			for (int i = 0; i < M; i++)
				A.Element(i, j) = Element(i, ColFirst + j);

		return A;
	}


	void Init(int p_M, int p_N) {
		if ((M != p_M) || (N != p_N)) {
			deAllocate();
			M = p_M;
			N = p_N;
			TotalSize = p_M * p_N;

			if (TotalSize > 0) {
				Elements = new GFq[TotalSize];
				for (int i = 0; i < TotalSize; i++)
					Elements[i].val = 0;
			} else
				Elements = NULL;
		}
	}

	matrix(int p_M, int p_N = 1) :
			Elements(NULL), M(0), N(0), TotalSize(0) {
		Init(p_M, p_N);
	}

	// Returns the reference to i,j element
	GFq &Element(int i, int j)
	// i in range 0,...,M-1 and j in range 0,...,N-1
			{
		return Elements[N * i + j];
	}

	// Operator overload, returns reference to ith element, assuming the rows are concatenated like a train!
	GFq &operator[](int i) {
		return Elements[i];
	}

	~matrix() {
		deAllocate();
	}

	void deAllocate() {
		if (TotalSize > 0) {
			delete [] Elements;
			TotalSize = M = N = 0;
			Elements = NULL;
		}
	}

	// FIXME:
	void Add(int row, check_node &Check, GFq Mult);
	void Set(int row, check_node &Check);

	void SwitchColumns(int j1, int j2) {
		GFq Aux;

		for (int i = 0; i < M; i++) {
			Aux = Element(i, j1);
			Element(i, j1) = Element(i, j2);
			Element(i, j2) = Aux;
		}
	}

	matrix &operator*(matrix &B) {
		static matrix Result;
		matrix &A = *this;

		if (A.N != B.M) {
			cout << "Attempt to multiply incompatible matrices\n";
			exit(1);
		}

		Result.Init(A.M, B.N);

		for (int i = 0; i < Result.M; i++)
			for (int j = 0; j < Result.N; j++) {
				Result.Element(i, j) = 0;
				for (int k = 0; k < A.N; k++)
					Result.Element(i, j) += A.Element(i, k) * B.Element(k, j);
			}

		return Result;
	}

	void SwitchRows(int i1, int i2) {
		GFq Aux;

		for (int j = 0; j < N; j++) {
			Aux = Element(i1, j);
			Element(i1, j) = Element(i2, j);
			Element(i2, j) = Aux;
		}
	}

	void MultiplyByMinusOne() {
		for (int i = 0; i < TotalSize; i++)
			Elements[i] = Elements[i].Minus();
	}

	// Add mult*(i2th row) to i1th row
	void AddRow(int i1, int i2, GFq mult) {
		for (int j = 0; j < N; j++)
			Element(i1, j) += Element(i2, j) * mult;
	}

	// multiply row i by mult
	void MultRow(int i, GFq mult) {
		for (int j = 0; j < N; j++)
			Element(i, j) *= mult;
	}

	// return the inverse, using Gauss-Jordan elimination method
	matrix &Inverse();

	void SetNull() {
		deAllocate();
	}

	BOOLEAN IsNull() {
		return (TotalSize == 0);
	}
};


#endif /* MATRIX_H_ */
