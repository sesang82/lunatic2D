#pragma once
#include "ssScript.h"

namespace ss
{
    class AudioListener;
    class AudioSource;

    class SoundMgrScript :
        public Script
    {
	public:
		SoundMgrScript();
		virtual ~SoundMgrScript();

	private:
		AudioListener* mListener;
		AudioSource* mBGM;
		AudioSource* mSFX;

	public:
		void SetListener(AudioListener* _Listener)
		{
			mListener = _Listener;
		}

		void SetBGM(AudioSource* _BGM)
		{
			mBGM = _BGM;
		}

		void SetSFX(AudioSource* _SFX)
		{
			mSFX = _SFX;
		}

	public:
		AudioListener* GetListener()	const
		{
			return mListener;
		}

		AudioSource* GetBGM()	const
		{
			return mBGM;
		}

		AudioSource* GetSFX()	const
		{
			return mSFX;
		}
    };
}

