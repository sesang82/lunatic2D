#pragma once
#include "..\\Engine_SOURCE\\ssScene.h"

namespace ss
{
    class LibraryScene :
        public Scene
    {
	public:
		LibraryScene();
		virtual ~LibraryScene();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;

	private:
		class Camera* mCamera;
		GameObject* mCameraObj;
		class Player* mPlayer;
    };
}
