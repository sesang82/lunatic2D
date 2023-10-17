#pragma once
#include "..\\Engine_SOURCE\\ssScene.h"

namespace ss
{
    class EntranceScene :
        public Scene
    {
	public:
		EntranceScene();
		virtual ~EntranceScene();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;

	private:
		Camera* mCamera;
		class Player* mPlayer;

    };
}
