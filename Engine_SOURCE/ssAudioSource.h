#pragma once
#include "ssComponent.h"
#include "ssAudioClip.h"

namespace ss
{
	class AudioSource : public Component
	{
	public:
		AudioSource();
		~AudioSource();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void Play(const std::wstring& _key, bool _loop = false);
		void Play_NoInterrupt(const std::wstring& _key, bool _loop = false);
		void Stop(const std::wstring& _key);
		void SetLoop(const std::wstring& _key, bool loop);

		void AddClipByKey(const std::wstring& _key);
		AudioClip* GetClip(const std::wstring& _key);

		void SetWholeVolume(float _Volume);
		void SetWholePitch(float _Pitch);

		void SetWholeVolume_Ratio(float _Volume);
		void SetWholePitch_Ratio(float _Pitch);

		void SetVolume(const std::wstring& _key, float _Volume);
		void SetPitch(const std::wstring& _key, float _Pitch);

		float GetVolume(const std::wstring& _key);
		float GetPitch(const std::wstring& _key);



	private:
		std::map<std::wstring, AudioClip*> mAudioClips;
	};
}
