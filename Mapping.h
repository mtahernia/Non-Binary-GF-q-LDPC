/*
 * Mapping.h
 *
 *  Created on: 22 Dec, 2014
 *      Author: Mehrdad Tahernia
 *		  User: mehrdad
 */

#ifndef MAPPING_H_
#define MAPPING_H_

#include "GFq.h"


/***************************************************************************
 *
 * mapping
 *
 ***************************************************************************/

class mapping {
public:
	int q;
	double vals[MAX_Q];
public:
	mapping(int p_q = -1, double *p_vals = NULL) {
		if (p_q != -1)
			Set_Q(p_q);

		if (p_vals != NULL)
			std::copy(p_vals, p_vals+q, vals);

	} // end of constructor

	void Set_Q(int p_q) {
		if (p_q > MAX_Q) {
			cout << "Exceeded MAX_Q in mapping (simply increase)\n";
			exit(1);
		}

		q = p_q;
	}
	// Copy mapping from p_MapInUse
	mapping(mapping &p_MapInUse) :
			q(p_MapInUse.q) {
		std::copy(p_MapInUse.vals, p_MapInUse.vals+q, vals);
	}

	// Determine whether the mapping is a binary mapping or not!
	bool IsBinary() {
		if (q != 2)
			return false;
		if (((vals[0] != 0) && (vals[0] != 1)) || ((vals[1] != 0) && (vals[1] != 1)))
			return false;
		return true;
	}

	// Read mapping from input file
	void GetFromFile(std::ifstream &file);

	// Average signal power assuming all constellation points are equiprobable
	double Average_E() {
		double sum_E = 0;
		for (int i = 0; i < q; i++)
			sum_E += pow(vals[i], 2);

		return sum_E / (double) q;
	}

	void Normalize() {
		// Normalize average energy to 1
		double factor = sqrt(Average_E());
		for (int i = 0; i < q; i++)
			vals[i] /= factor;
	}

	// Operator overload for division of a mapping
	void operator/=(double d) {
		for (int i = 0; i < q; i++)
			vals[i] /= d;
	}

	// Operator overload for multiplication of a mapping
	void operator*=(double d) {
		for (int i = 0; i < q; i++)
			vals[i] *= d;
	}

	// Returns the mapping point for a integer x
	double map(int x) {
		return vals[x];
	}

	// Returns the mapping point for g in GF(q)
	double map(GFq &g) {
		return map(g.val);
	}

	// Return q of mapping
	int GetQ() {
		return q;
	}
};


inline std::ostream &operator<<(std::ostream &s, mapping &MapInUse) { // for writing MapInUse to s
	s << "[";
	for (int i = 0; i < MapInUse.GetQ(); i++) {
		if (i != 0)
			s << " ";
		s << MapInUse.vals[i];
	}

	s << "]";

	return s;
}

#endif /* MAPPING_H_ */
