#include "ssAudioSource.h"
#include "ssAudioClip.h"
#include "ssTransform.h"
#include "ssGameObject.h"
#include "ssTime.h"
#include "ssResources.h"

namespace ss
{
	AudioSource::AudioSource()
		: Component(eComponentType::AudioSource)
		, mAudioClip(nullptr)
	{

	}

	AudioSource::~AudioSource()
	{

	}

	void AudioSource::Initialize()
	{
	}

	void AudioSource::Update()
	{
	}

	void AudioSource::LateUpdate()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
		Vector3 foward = tr->Forward();

		if (mAudioClip)
			mAudioClip->Set3DAttributes(pos, foward);
	}

	void AudioSource::Render()
	{
	}

	void AudioSource::Play()
	{
		mAudioClip->Play();
	}
	void AudioSource::Stop()
	{
		mAudioClip->Stop();
	}
	void AudioSource::SetLoop(bool loop)
	{
		mAudioClip->SetLoop(loop);
	}

	void AudioSource::SetVolume(float _Volume)
	{
		mAudioClip->SetVolume(_Volume);
	}

	void AudioSource::PlaybackSpeed(float _speed)
	{
		mAudioClip->SetPlaybackSpeed(_speed);
	}

	bool AudioSource::IsPlaying()
	{

		 return mAudioClip->IsPlaying();
	}

}