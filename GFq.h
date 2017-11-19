/*
 * GFq.h
 *
 *  Created on: 22 Dec, 2014
 *      Author: Mehrdad Tahernia
 *		  User: mehrdad
 */

#ifndef GFQ_H_
#define GFQ_H_

#include "Functions.h"

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
	static bool IsPrimeQ;		// is our field prime, extension or invalid
	static bool IsModuloOperations; // when we have a prime field, operations become modular

	int val;// Value of field element

public:
	// GFq constructor with value g
	GFq(GFq &g) : val(g.val) {}
	GFq(int i) : val((BYTE) i) {}
	GFq(BYTE b = 0) :val(b) {}

	bool IsZero() {return val == 0;}
	BYTE GetVal() {return val;} 	// Unsigned Char
	static void Initialize(int p_q);
	static void GenerateAlphas(int m);
	static GFq &One();

	bool operator==(GFq g);// {	return val == g.val;}
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
					cout << "GFq::operator*=: Invalid multiplication (q is not prime)\n";
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

#endif /* GFQ_H_ */
