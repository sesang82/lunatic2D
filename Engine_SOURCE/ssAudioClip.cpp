#include "ssAudioClip.h"
#include "ssTransform.h"

namespace ss
{
	AudioClip::AudioClip()
		: Resource(eResourceType::AudioClip)
		, mSound(nullptr)
		, mChannel(nullptr)
		, mMinDistance(1.0f)
		, mMaxDistance(1000.0f)
		, mbLoop(false)
		, mbPlaying(false)
	{
		Fmod::mCoreSystem->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &mPitchShiftDSP);
		mPitchShiftDSP->setBypass(true);  // 처음에는 DSP 효과를 우회합니다.
	}

	AudioClip::~AudioClip()
	{
	}

	void AudioClip::SetPlaybackSpeed(float speed)
	{
		if (mChannel)
		{
			mPitchShiftDSP->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, speed);
			mChannel->addDSP(0, mPitchShiftDSP);
			mPitchShiftDSP->setBypass(false);
		}
	}

	bool AudioClip::IsPlaying()
	{
		
		return mChannel->isPlaying(&mbPlaying);
	}

	HRESULT AudioClip::Load(const std::wstring& _Path)
	{
		std::string cPath(_Path.begin(), _Path.end());
		if (!Fmod::CreateSound(cPath, &mSound))
			return S_FALSE;

		mSound->set3DMinMaxDistance(mMinDistance, mMaxDistance);

		return S_OK;
	}

	void AudioClip::Play()
	{
		if (mbLoop)
			mSound->setMode(FMOD_LOOP_NORMAL);
		else
			mSound->setMode(FMOD_LOOP_OFF);

		Fmod::SoundPlay(mSound, &mChannel);
	}

	void AudioClip::Stop()
	{
		mChannel->stop();
	}

	void AudioClip::SetVolume(float _Volume)
	{
		mChannel->setVolume(_Volume);
	}

	void AudioClip::Set3DAttributes(const Vector3 _Pos, const Vector3 _Vel)
	{
		FMOD_VECTOR fmodPos(_Pos.x, _Pos.y, _Pos.z);
		FMOD_VECTOR fmodVel(_Vel.x, _Vel.y, _Vel.z);

		mChannel->set3DAttributes(&fmodPos, &fmodVel);
	}

	
}
