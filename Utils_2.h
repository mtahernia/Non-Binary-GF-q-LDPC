#ifndef UTILS_2
#define UTILS_2

// works even without this
#include "Functions.h" //uniform_random
#include "GFq.h"
#include "Matrix.h"


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
