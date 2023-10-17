#pragma once
#include "ssResource.h"
#include "ssFmod.h"



//#include <Fmod/fmod_studio.hpp>
//#include <Fmod/fmod.hpp>
//#include <Fmod/fmod_common.h>
//#include <Fmod/fmod_codec.h>

//#include "..\External\Include\\DirectXTex\DirectXTex.h"

namespace ss
{
	using namespace ss::math;
	class AudioClip : public Resource
	{
	public:
		AudioClip();
		virtual ~AudioClip();

	private:
		FMOD::Sound* mSound;
		FMOD::Channel* mChannel;
		float				mMinDistance;
		float				mMaxDistance;
		bool				mbLoop;
		FMOD::DSP* mPitchShiftDSP; // 피치를 유지하면서 재생 속도 빠르게 하기 위한 기능 dsp

	public:
		void Play();
		void Stop();
		void SetVolume(float _Volume);		// 0 ~ 1
		void Set3DAttributes(const Vector3 _Pos, const Vector3 _Vel);
		void SetLoop(bool _Loop)
		{
			mbLoop = _Loop;
		}

		void SetPlaybackSpeed(float speed); // 피치는 유지하면서 재생속도만 증가

	public:
		virtual HRESULT Load(const std::wstring& _Path) override;
	};
}

