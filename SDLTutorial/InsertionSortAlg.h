#pragma once

#include "Visualizer.h"

class InsertionSortAlg {

public:

	InsertionSortAlg()
	{
		m_i = 0;
		m_j = m_i;
		m_sortingDone = false;
	}

	std::string GetName()
	{
		return "Insertion Sort";
	}

	void SortStep(std::vector<VisualizerItem*>& items)
	{
		if (m_i < items.size())
		{
			bool swapped = false;

			if (m_j > 0)
			{
				if (items[m_j]->GetValue() < items[m_j - 1]->GetValue()) 
				{

					VisualizerItem* temp = items[m_j];
					items[m_j] = items[m_j - 1];
					items[m_j - 1] = temp;

					items[m_j]->UpdatePosition(m_j);
					items[m_j - 1]->UpdatePosition(m_j - 1);
					swapped = true;
				}
				--m_j;
			}

			if (!swapped || m_j == 0)
			{
				m_i++;
				m_j = m_i;
			}
		}
		else if(!m_sortingDone)
		{
			m_sortingDone = true;
		}

		UpdateHighlights(items);
	}

private:

	void UpdateHighlights(std::vector<VisualizerItem*>& items)
	{
		for (unsigned int i = 0; i < items.size(); i++)
		{
			if (!m_sortingDone && i == m_j)
			{
				items[i]->SetHighlightCurrent();
			}
			else if (!m_sortingDone && m_j > 0 && i == m_j - 1)
			{
				items[i]->SetHighlightComparison();
			}
			else if (i <= m_i)
			{
				items[i]->SetHighlightSorted();
			}
			else
			{
				items[i]->ResetHighlight();
			}
		}
	}

private:

	unsigned int m_i;
	unsigned int m_j;

	bool m_sortingDone;
};
