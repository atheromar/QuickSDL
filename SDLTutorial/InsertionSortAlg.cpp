#include "InsertionSortAlg.h"

InsertionSortAlg::InsertionSortAlg()
{
	m_i = 0;
	m_j = m_i;
}

void InsertionSortAlg::SortStep(std::vector<VisualizerItem*>& items)
{
	if (m_i < items.size())
	{
		if (m_j > 0)
		{
			if (items[m_j]->GetValue() < items[m_j - 1]->GetValue()) {

				VisualizerItem* temp = items[m_j];
				items[m_j] = items[m_j - 1];
				items[m_j - 1] = temp;

				items[m_j]->UpdatePosition(m_j);
				items[m_j - 1]->UpdatePosition(m_j - 1);
				m_j--;
			}
			else
			{
				items[m_j]->SetSelected(false);
				m_j = 0;
			}
		}
		else
		{
			items[m_j]->SetSelected(false);
			m_i++;
			m_j = m_i;
			items[m_j]->SetSelected(true);
		}
	}
}