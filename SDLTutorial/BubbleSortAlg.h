#pragma once

#include "Visualizer.h"

class BubbleSortAlg {

public:

	BubbleSortAlg()
	{
		m_swapped = false;
		m_sortingDone = false;
		m_i = 0;
		m_iteration = 0;
	}

	std::string GetName()
	{
		return "Bubble Sort";
	}

	void SortStep(std::vector<VisualizerItem*>& items)
	{
		if (m_sortingDone)
			return;

		if (m_i < items.size() - 1)
		{
			if (items[m_i]->GetValue() > items[m_i + 1]->GetValue()) {

				VisualizerItem* temp = items[m_i];
				items[m_i] = items[m_i + 1];
				items[m_i + 1] = temp;

				items[m_i]->UpdatePosition(m_i);
				items[m_i + 1]->UpdatePosition(m_i + 1);

				m_swapped = true;
			}
			++m_i;

			if (m_i >= items.size() - m_iteration - 1)
			{
				if (m_swapped)
				{
					m_swapped = false;
					m_i = 0;
					m_iteration++;
				}
				else
				{
					m_sortingDone = true;
				}
			}
		}

		UpdateHighlights(items);
	}

private:

	void UpdateHighlights(std::vector<VisualizerItem*>& items)
	{
		for (unsigned int i = 0; i < items.size(); i++)
		{
			if (!m_sortingDone && i == m_i)
			{
				items[i]->SetHighlightCurrent();
			}
			else if (m_sortingDone || i >= items.size() - m_iteration)
			{
				items[i]->SetHighlightSorted();
			}
			else if (i == m_i + 1)
			{
				items[i]->SetHighlightComparison();
			}
			else
			{
				items[i]->ResetHighlight();
			}
		}
	}

private:

	bool m_swapped;
	bool m_sortingDone;

	unsigned int m_i;
	unsigned int m_iteration;
};
