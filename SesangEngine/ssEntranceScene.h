#pragma once
#include "ssBaseScene.h"

namespace ss
{
    class EntranceScene :
        public BaseScene
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

    };
}
