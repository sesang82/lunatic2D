#pragma once
#include "ssBaseScene.h"

namespace ss
{
    class TitleScene :
        public BaseScene
    {
    public:
        TitleScene();
        virtual ~TitleScene();

    public:
        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;

        virtual void OnEnter() override;
        virtual void OnExit() override;


        class GameObject* mMainCam;
    };
}

