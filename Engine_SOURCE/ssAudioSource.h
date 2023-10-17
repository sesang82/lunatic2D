#pragma once
#include "ssComponent.h"
#include "ssAudioClip.h"

namespace ss
{
	class AudioSource : public Component
	{
	public:
		AudioSource();
		virtual ~AudioSource();

	private:
		std::shared_ptr<AudioClip> mAudioClip;

	public:
		void SetClip(std::shared_ptr<AudioClip> _Clip)
		{
			mAudioClip = _Clip;
		}

		std::shared_ptr<AudioClip> GetClip()
		{
			return mAudioClip;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	public:
		void Play();
		void Stop();
		void SetLoop(bool loop);
		// 0 ~ 1
		void SetVolume(float _Volume);
		void PlaybackSpeed(float _speed); // 0.5~2까지만. 피치는 유지하면서 재생속도만 증가
	};
}
