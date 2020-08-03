#pragma once

#include "Visualizer.h"

class SelectionSortAlg {

public:

	SelectionSortAlg()
	{
		m_firstStep = true;
		m_sortingDone = false;
	}

	std::string GetName()
	{
		return "Selection Sort";
	}

	unsigned int const& GetComparisons()
	{
		return m_comparisons;
	}

	unsigned int const& GetSwaps()
	{
		return m_swaps;
	}

	void SortStep(std::vector<VisualizerItem*>& items)
	{
		if (m_firstStep)
		{
			m_firstStep = false;
			m_i = items.size() - 1;
			m_j = 0;
			m_maxIndex = m_i;

			m_comparisons = 0;
			m_swaps = 0;
		}

		if (m_i > 0)
		{
			if (m_j < m_i)
			{
				++m_comparisons;
				if (items[m_maxIndex]->GetValue() < items[m_j]->GetValue()) {

					m_maxIndex = m_j;
				}
				m_j++;
			}
			else
			{
				VisualizerItem* temp = items[m_maxIndex];
				items[m_maxIndex] = items[m_i];
				items[m_i] = temp;

				m_i--;
				m_j = 0;
				m_maxIndex = m_i;
				
				++m_swaps;
			}
		}
		else
		{
			m_sortingDone = true;
		}

		UpdatePositions(items);
		UpdateHighlights(items);
	}

private:

	void UpdatePositions(std::vector<VisualizerItem*>& items)
	{
		for (unsigned int i = 0; i < items.size(); i++)
		{
			items[i]->UpdatePosition(i);
		}
	}

	void UpdateHighlights(std::vector<VisualizerItem*>& items)
	{
		for (unsigned int i = 0; i < items.size(); i++)
		{
			if (!m_sortingDone && i == m_i)
			{
				items[i]->SetHighlightCurrent();
			}
			else if (i >= m_i)
			{
				items[i]->SetHighlightSorted();
			}
			else if (i == m_j)
			{
				items[i]->SetHighlightComparison();
			}
			else if (i == m_maxIndex)
			{
				items[i]->SetHighlightBest();
			}
			else
			{
				items[i]->ResetHighlight();
			}
		}
	}

private:

	bool m_firstStep;
	bool m_sortingDone;

	unsigned int m_i;
	unsigned int m_j;
	unsigned int m_maxIndex;

	unsigned int m_comparisons;
	unsigned int m_swaps;
};
