#include "PerformanceTimer.h"

namespace QuickSDL {
	
	PerfTimer::PerfTimer()
	{
		Start();
	}

	void PerfTimer::Start()
	{
		mStartTime = std::chrono::high_resolution_clock::now();
	}

	double PerfTimer::Stop()
	{
		return (std::chrono::high_resolution_clock::now() - mStartTime).count * 0.0000000001;
	}
}