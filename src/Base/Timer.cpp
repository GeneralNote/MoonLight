#include <MoonLight/Base/Timer.h>

namespace ml
{
	Timer::Timer()
	{
		QueryPerformanceCounter(&mStart);
		QueryPerformanceFrequency(&mFrequency);
		mPause = false;
		mPauseTime = 0;
	}
	float Timer::Restart()
	{
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);

		float ret = 0;
		if (mPause)
			ret = (mPauseStart.QuadPart - mStart.QuadPart);
		else
			ret = (end.QuadPart - mStart.QuadPart);
		ret = ret / (float)mFrequency.QuadPart - mPauseTime;

		mStart = end;
		mPauseTime = 0;

		return ret;
	}
	float Timer::GetElapsedTime()
	{
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);

		float ret = 0;
		if (mPause)
			ret = (mPauseStart.QuadPart - mStart.QuadPart);
		else
			ret = (end.QuadPart - mStart.QuadPart);

		return ret / (float)mFrequency.QuadPart - mPauseTime;
	}
	void Timer::Pause()
	{
		if (!mPause) {
			mPause = true;
			QueryPerformanceCounter(&mPauseStart);
		}
	}
	void Timer::Resume()
	{
		if (mPause) {
			LARGE_INTEGER end;
			QueryPerformanceCounter(&end);

			mPauseTime += (end.QuadPart - mPauseStart.QuadPart) / (float)mFrequency.QuadPart;
			mPause = false;
		}
	}
}