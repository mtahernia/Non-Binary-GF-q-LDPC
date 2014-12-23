/*
 * Channel.cpp
 *
 *  Created on: 22 Dec, 2014
 *      Author: Mehrdad Tahernia
 *		  User: mehrdad
 */
//#include "GFq.h"
#include "Matrix.h" // matrix, column vector
#include "Utils_2.h" // vector, array

#include "Channel.h"

/*********************************************************************************
 *
 * CHANNEL
 *
 *********************************************************************************/

void channel::SimulateOutputVector(vector &InVector, vector &OutVector)
{
	OutVector.Allocate(InVector.GetSize());
	for (int i = 0; i < InVector.GetSize(); i++)  // Add noise to each component
		OutVector[i] = this->SimulateOutput( /* channel in */InVector[i]);
}
