#pragma once
#include "..\Engine_SOURCE\ssScene.h"

namespace ss
{
    class IntroScene :
        public Scene
    {
	public:
		IntroScene();
		virtual ~IntroScene();
	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;
    };

}

