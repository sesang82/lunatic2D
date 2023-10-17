#pragma once
#include "..\\Engine_SOURCE\\ssScene.h"

namespace ss
{
    class TitleScene :
        public Scene
    {
    public:
        TitleScene();
        virtual ~TitleScene();

    public:
        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;
        virtual void Destroy() override;

        virtual void OnEnter() override;
        virtual void OnExit() override;


        GameObject* mCam;
    };
}

