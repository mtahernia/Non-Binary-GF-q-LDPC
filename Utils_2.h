#ifndef UTILS_2
#define UTILS_2

#include "Functions.h" //uniform_random
#include "Matrix.h"
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

	// Operator overload: copy a column vector to this vector //TODO: column_vector!!!
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
};

#endif
