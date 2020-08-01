#pragma once

#include <chrono>

namespace QuickSDL {
	//-----------------------------------------------------------------
	// PerfTimer
	//-----------------------------------------------------------------
	class PerfTimer {

	private: 
		
		std::chrono::high_resolution_clock::time_point mStartTime;

	public:

		PerfTimer();

		void Start();
		double Stop();
	};
}