#pragma once
#include "..\\Engine_SOURCE\\ssScene.h"

namespace ss
{
	class EntryScene :
		public Scene
	{
	public:
		EntryScene();
		virtual ~EntryScene();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;

	private:
		class Player* mPlayer;
		class Camera* mCamera;

	};
}

