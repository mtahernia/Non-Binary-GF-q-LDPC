#ifndef UTILS_2
#define UTILS_2

// works even without this
#include "Functions.h" //uniform_random
#include "GFq.h"

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
