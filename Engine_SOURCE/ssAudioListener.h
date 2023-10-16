#pragma once
#include "ssComponent.h"


namespace ss
{
	class AudioListener : public Component
	{
	public:
		AudioListener();
		virtual ~AudioListener();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	private:
	};
}