#ifndef __MOONLIGHT_TIMER_H__
#define __MOONLIGHT_TIMER_H__
#include <Windows.h>

namespace ml
{
	class Timer
	{
	public:
		Timer();

		float Restart();
		float GetElapsedTime();
		
		void Pause();
		void Resume();

		inline bool IsPaused() { return mPause; }

	private:
		LARGE_INTEGER mStart;
		LARGE_INTEGER mFrequency;
		LARGE_INTEGER mPauseStart;
		float mPauseTime;
		bool mPause;
	};
}

#endif // __MOONLIGHT_TIMER_H__