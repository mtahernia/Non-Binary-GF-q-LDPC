/*
 * Mapping.cpp
 *
 *  Created on: 22 Dec, 2014
 *      Author: Mehrdad Tahernia
 *		  User: mehrdad
 */

#include "Mapping.h"

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


