#pragma once

#include "Visualizer.h"

class InsertionSortAlg {

public:

	InsertionSortAlg();

	void SortStep(std::vector<VisualizerItem*>& items);

private:

	int m_i;
	int m_j;
};
