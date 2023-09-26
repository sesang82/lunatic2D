#pragma once
#include "..\Engine_SOURCE\ssScene.h"

namespace ss
{
    class Boss2Scene :
        public Scene
    {
	public:
		Boss2Scene();
		virtual ~Boss2Scene();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;


	private:
		Camera* mCamera;
		class Monster* mBoss1;
		class Player* mPlayer;

    };
}

