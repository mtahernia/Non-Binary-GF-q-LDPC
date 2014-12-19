#ifndef UTILS_2
#define UTILS_2

#include <math.h>
#include "Utils_1.h"

/************************************************************************
 *
 * GF(q)
 *
 ************************************************************************/

class GFq {
public:
	static int q;					// General field definition parameter
	static int log_2_q; 		    // If q is not prime
	static int mask;		        // if q is not prime - 1's at first positions
	static GFq alpha[MAX_Q];		// alpha[i] is the ith element of field
	static int reverse_alpha[MAX_Q];// multiplicative inverse of alphas
	static GFq inverse[MAX_Q];		// inverse[i] multiplicative inverse of i
	static BOOLEAN IsPrimeQ;		// is our field prime, extension or invalid
	static BOOLEAN IsModuloOperations; // when we have a prime field, operations become modular

	// Value of field element
	int val;

public:
	// GFq constructor with value g
	GFq(GFq &g) :
			val(g.val) {
	}

	BOOLEAN IsZero() {
		return val == 0;
	}

	BYTE GetVal() { // Unsigned Char
		return val;
	}

	static void Initialize(int p_q);

	static void GenerateAlphas(int m);

	static GFq &One() {
		if (IsModuloOperations) {
			static GFq ConstOne(1);
			return ConstOne;
		} else
			return alpha[0];
	}

	BOOLEAN operator==(GFq g) {
		return val == g.val;
	}

	GFq(int i) :
			val((BYTE) i) {
	}
	GFq(BYTE b = 0) :
			val(b) {
	}

	GFq &operator+=(GFq g) {
		if (IsModuloOperations)
			val = (val + g.val) % q;
		else
			val ^= g.val;

		return *this;
	}

	GFq &operator=(GFq &g) {
		val = g.val;
		return *this;
	}

	GFq &operator=(BYTE b) {
		val = b;
		return *this;
	}

	GFq &operator=(int i) {
		val = (BYTE) i;
		return *this;
	}

	GFq &operator-=(GFq g) {
		if (IsModuloOperations)
			val = (q + val - g.val) % q;
		else
			val ^= g.val;

		return *this;
	}

	GFq &operator*=(GFq g) {
		if ((val == 0) || (g.val == 0))
			val = 0;
		else {
			if (IsModuloOperations) {
				if (!IsPrimeQ) {
					cout
							<< "GFq::operator*=: Invalid multiplication (q is not prime)\n";
					exit(1);
				}

				val = (val * g.val) % q;
			} else
				val = alpha[(reverse_alpha[this->val] + reverse_alpha[g.val])
						% (q - 1)].val;
		}

		return *this;
	}

	GFq &Inverse() {
		return inverse[val];
	}

	GFq &Minus() {
		static GFq g;

		if (IsModuloOperations)
			g.val = (q - val) % q;
		else
			// GF(2^m)
			g.val = val;

		return g;
	}

	void RandomSelect()      // Random select among nonzero elements
	{
		val = uniform_random(q - 1) + 1;
	}

	GFq &operator/=(GFq g) {
		if (val == 0)
			val = 0;
		else if (g.val == 0) {
			cout << "GFq::operator /= division by zero\n";
			exit(1);
		} else {
			if (IsModuloOperations) {
				if (!IsPrimeQ) {
					cout
							<< "GFq::operator*=: Invalid multiplication (q is not prime)\n";
					exit(1);
				}

				*this *= inverse[g.val];
			} else
				val = alpha[(q - 1
						+ (reverse_alpha[this->val] - reverse_alpha[g.val]))
						% (q - 1)].val;
		}

		return *this;
	}

	GFq &operator*(GFq g) {
		static GFq result;

		result = *this;
		result *= g;

		return result;
	}

	GFq &operator/(GFq g) {
		static GFq result;

		result = *this;
		result /= g;

		return result;
	}

	GFq &operator+(GFq g) {
		static GFq result;

		result = *this;
		result += g;

		return result;
	}

	GFq &operator-(GFq g) {
		static GFq result;

		result = *this;
		result -= g;

		return result;
	}
};

inline std::ostream &operator<<(std::ostream &s, GFq g) {
	s << (int) g.val;

	return s;
}

inline std::istream &operator>>(std::istream &s, GFq &g) {
	int i;
	s >> i;

	g.val = i;

	return s;
}

/************************************************************************
 *
 * GF(q) matrix
 *
 ************************************************************************/

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

class column_vector: public matrix {
public:
	column_vector() :
			matrix() {
	}
	column_vector(int p_M) :
			matrix(p_M, 1) {
	}

	void Init(int p_M) {
		matrix::Init(p_M, 1);
	}

//   ~column_vector()
//   {
//      deAllocate();
//   }

	GFq &operator[](int i) {
		return this->Elements[i];
	}

	// This one is not used in this code
	column_vector &operator=(long p_Val) {
		// LSB is first
		for (int i = 0; i < M; i++) {
			(*this)[i].val = p_Val & GFq::mask;
			p_Val >>= GFq::log_2_q;
		}

		return *this;
	}

	column_vector &operator=(matrix &Matrix) {
		if (Matrix.N != 1) {
			cout
					<< "column_vector::operator= : Incompatible rows/columns in assignment\n";
			exit(1);
		}

		Init(Matrix.M);

		for (int i = 0; i < M; i++)
			(*this)[i] = Matrix.Element(i, 0);

		return *this;
	}

	// Not used
	void Extract(column_vector &p_Source, int RowFirst) {
		int SourceIndex = RowFirst;

		// If attempting to extract more elements than exist in source
		if (M > (p_Source.M - RowFirst)) {
			cout << "column_vector::Extract : Incompatible rows\n";
			exit(1);
		}

		for (int i = 0; i < M; i++, SourceIndex++)
			(*this)[i] = p_Source[SourceIndex];
	}

	// Not used
	void Combine(column_vector &p_Vector1, column_vector &p_Vector2) {
		if (M != (p_Vector1.M + p_Vector2.M)) {
			cout << "column_vector::Combine: Incompatible rows\n";
			exit(1);
		}

		int index = 0;
		for (int i = 0; i < p_Vector1.M; index++, i++)
			(*this)[index] = p_Vector1[i];
		for (int i = 0; i < p_Vector2.M; index++, i++)
			(*this)[index] = p_Vector2[i];
	}
};

std::ostream &operator<<(std::ostream &s, matrix &m);

#define MAX_BUF 10000

std::istream &operator>>(std::istream &s, matrix &m);

/************************************************************************
 *
 * IntArray
 *
 ************************************************************************/

// M by N matrix of integer values
class IntArray {
public:
	int *Elements;
	int M, N;

public:
	IntArray() :
			Elements(NULL), M(0), N(0) {
	}

	IntArray(int p_M, int p_N) :
			Elements(NULL), M(0), N(0) {
		Allocate(p_M, p_N);
	}

	~IntArray() {
		deAllocate();
	}

	void Allocate(int p_M, int p_N) {
		if ((M != p_M) || (N != p_N)) {
			deAllocate();
			M = p_M;
			N = p_N;
			Elements = new int[M * N];
		}
	}

	void deAllocate() {
		M = 0;
		N = 0;
		if (Elements != NULL)
			delete [] Elements;
		Elements = NULL;
	}

	int &Element(int i, int j) {
		return Elements[i * N + j];
	}

};

/************************************************************************
 *
 * Vector
 *
 ************************************************************************/

class vector {
public:
	double *Elements;
	int CountElements;

public:

	vector(int p_Elements = 0) :
			Elements(NULL), CountElements(0) {
		Allocate(p_Elements);
	}

	~vector() {
		deAllocate();
	}

	int GetSize() {
		return CountElements;
	}

	double &operator[](int i) {
		return Elements[i];
	}

	// Allocate an empty vector of length p_Elements
	void Allocate(int p_Elements) {
		if (CountElements != p_Elements) {	// If new size is different, deallocate and allocate with new size
			deAllocate();
			if (p_Elements != 0) {
				Elements = new double[p_Elements];
				CountElements = p_Elements;
			}
		}
	}

	// Deallocate Elements to avoid memory leak
	void deAllocate() {
		if (Elements != NULL)
			delete [] Elements;
		Elements = NULL;
		CountElements = 0;
	}

	// Operator overload: Copy p_Vector to this vector
	vector &operator=(vector &p_Vector) {
		Allocate(p_Vector.CountElements);
		for (int i = 0; i < CountElements; i++)
			Elements[i] = p_Vector.Elements[i];

		return *this;
	}

	// Operator overload: copy a column vector to this vector
	vector &operator=(column_vector &p_Vector) {
		Allocate(p_Vector.M);
		for (int i = 0; i < CountElements; i++)
			Elements[i] = p_Vector.Elements[i].val;

		return *this;
	}

};


// Operator overload for outputting a vector to an ostream
inline std::ostream &operator<<(std::ostream &s, vector &v) {
	s << "[";
	for (int i = 0; i < v.GetSize(); i++) {
		if (i != 0)
			s << " ";
		s << v[i];
	}
	s << "]";

	return s;
}

#endif
