#pragma once
#include "..\\Engine_SOURCE\\ssScene.h"

namespace ss
{
	class Camera;

	class Stage1Scene :
		public Scene
	{
	public:
		Stage1Scene();
		virtual ~Stage1Scene();

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

	