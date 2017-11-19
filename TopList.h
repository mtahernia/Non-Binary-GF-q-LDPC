/*
 * TopList.h
 *
 *  Created on: 23 Dec, 2014
 \*      Author: Mehrdad Tahernia
 */

#ifndef TOPLIST_H_
#define TOPLIST_H_

#include "GFq.h"


/************************************************************************
 *
 * TopList - for use in greedy
 *
 ************************************************************************/
// TODO: I don't Understand
class TopList {
public:
	GFq *BestChange;
	int *VariableIndex;
	double *DistortionImprovement;

	int MaxSize, CurrentSize;
public:
	TopList() :	MaxSize(-1), CurrentSize(-1) ,BestChange(NULL),DistortionImprovement(NULL),VariableIndex(NULL) {}

	~TopList() {
		DeAllocate();
	}

	void DeAllocate() {
		if (MaxSize > -1) {
			delete BestChange;
			delete VariableIndex;
			delete DistortionImprovement;
		}
		MaxSize = -1;
	}

	void Init(int p_MaxSize) {
		if (MaxSize != p_MaxSize) {
			DeAllocate();

			BestChange = new GFq[p_MaxSize];
			VariableIndex = new int[p_MaxSize];
			DistortionImprovement = new double[p_MaxSize];
		}

		MaxSize = p_MaxSize;
		CurrentSize = 0;
	}

	void Add(GFq p_BestChange, int p_VariableIndex,
			double p_DistortionImprovement) {
		// Find place for new distortion
		int Place;
		for (Place = 0; Place < CurrentSize; Place++)
			if (p_DistortionImprovement > DistortionImprovement[Place])
				break;

		// Shift all the rest one place up
		if (CurrentSize < MaxSize)
			CurrentSize++;
		for (int i = CurrentSize - 1; i > Place; i--) {
			BestChange[i] = BestChange[i - 1];
			VariableIndex[i] = VariableIndex[i - 1];
			DistortionImprovement[i] = DistortionImprovement[i - 1];
		}

		// Place new entry
		if (Place < CurrentSize)      // If not passed last place in TopList
				{
			BestChange[Place] = p_BestChange;
			VariableIndex[Place] = p_VariableIndex;
			DistortionImprovement[Place] = p_DistortionImprovement;
		}
	}
};
#endif /* TOPLIST_H_ */
