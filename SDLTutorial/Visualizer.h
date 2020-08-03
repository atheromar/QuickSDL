#pragma once

#include "Texture.h"
#include "VisualizerInterface.h"
#include <iostream>

using namespace QuickSDL;

class VisualizerItem : public GameEntity
{
private:

	const int ITEM_BUFFER = 2;

public:

	VisualizerItem(int const& value, int const& maxValue, float const& width, int const& maxHeight, int const& index, GameEntity* anchor)
		: m_value(value)
	{
		m_width = width;

		m_tex = new Texture("stars.png", 4, 0, 4, 4);
		m_tex->Parent(this);
		m_tex->Pos(VEC2_ZERO);

		float scaleRatio = value / (float)maxValue;
		Vector2 scale((width - ITEM_BUFFER) / 4.0f, (scaleRatio * maxHeight) / 4.0f);

		m_tex->Scale(scale);

		Parent(anchor);

		Pos(Vector2(index * (float)m_width, Graphics::Instance()->SCREEN_HEIGHT * 0.5f - scaleRatio * maxHeight * 0.5f));
	}

	~VisualizerItem()
	{
		delete m_tex;
		m_tex = nullptr;
	}

	int const& GetValue() const { return m_value; }

	void UpdatePosition(int const& index)
	{
		Pos(Vector2(index * (float)m_width, Pos(SPACE::local).y));
	}

	void SetHighlightSorted()
	{
		m_tex->MoveClipRect(12, 0);
	}

	void SetHighlightCurrent()
	{
		m_tex->MoveClipRect(8, 0);
	}

	void SetHighlightComparison()
	{
		m_tex->MoveClipRect(0, 0);
	}

	void SetHighlightBest()
	{
		m_tex->MoveClipRect(16, 0);
	}

	void ResetHighlight()
	{
		m_tex->MoveClipRect(4, 0);
	}

	void Render() override
	{
		m_tex->Render();
	}

private:
	int m_value;
	Texture* m_tex;

	float m_width;
};

template <class T>
class Visualizer : public VisualizerInterface  
{
public:

	Visualizer()
	{
		m_title = new Texture(m_sortingAlgorithm.GetName(), "ariblk.ttf", 32, { 255, 255, 255 });
		m_title->Parent(this);
		m_title->Pos(VEC2_UP * (float)Graphics::Instance()->SCREEN_HEIGHT * 0.52f);
	}

	~Visualizer()
	{
		for (unsigned int i = 0; i < m_items.size(); i++)
		{
			delete m_items[i];
			m_items[i] = nullptr;
		}

		delete m_anchor;
		m_anchor = nullptr;

		delete m_title;
		m_title = nullptr;
	}

	void Initialize(std::vector<int> const& vec) override
	{
		if (vec.size() < 1)
		{
			return;
		}

		int maxValue = -999999;
		for (unsigned int i = 0; i < vec.size(); i++)
		{
			if (vec[i] > maxValue)
			{
				maxValue = vec[i];
			}
		}

		m_anchor = new GameEntity();
		m_anchor->Parent(this);

		float itemWidth = (float)Graphics::Instance()->SCREEN_WIDTH / vec.size();

		m_anchor->Pos(Vector2(-Graphics::Instance()->SCREEN_WIDTH * 0.5f + itemWidth * 0.5f, 0.0f));

		for (unsigned int i = 0; i < vec.size(); i++)
		{
			m_items.push_back(new VisualizerItem(vec[i], maxValue, itemWidth, Graphics::Instance()->SCREEN_HEIGHT, i, m_anchor));
		}

		i = 0;
		j = 0;
	}

	void Update() override
	{
		m_sortingAlgorithm.SortStep(m_items);
	}

	void Render() override
	{
		for (unsigned int i = 0; i < m_items.size(); i++)
		{
			m_items[i]->Render();
		}

		m_title->Render();
	}

private:

	Texture* m_title;
	GameEntity* m_anchor;
	std::vector<VisualizerItem*> m_items;

	T m_sortingAlgorithm;

	int i;
	int j;
};