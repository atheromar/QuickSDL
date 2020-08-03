#pragma once
#include <vector>
#include "GameEntity.h"

class VisualizerInterface : public QuickSDL::GameEntity {

public:
	virtual void Initialize(std::vector<int> const& vec) = 0;
};