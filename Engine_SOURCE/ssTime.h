#pragma once
#include "SesangEngine.h"

namespace ss
{
	class Time
	{
	public:
		static void Initiailize();
		static void Update();
		static void Render();

		__forceinline static double DeltaTime() { return mDeltaTime; }
		static void SetTimeScale(float scale) { mTimeScale = scale; }
		static float GetTimeScale() { return mTimeScale; }

	private:
		static double mDeltaTime;
		static double mTimeScale;
		static double mSecond;
		static LARGE_INTEGER mCpuFrequency;
		static LARGE_INTEGER mPrevFrequency;
		static LARGE_INTEGER mCurFrequency;




		
	};
}
