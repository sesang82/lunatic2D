#pragma once
#include "..\\Engine_SOURCE\\ssScene.h"


namespace ss
{
    class Player;
    class Camera;

    class Progressbar;
    // 씬에서 중복되는 요소들을 해당 클래스를 상속 받을 수 있게 구현하기 
    // 상속받아야될 것들은 protected 하면 될듯 

    class BaseScene :
        public Scene
    {
    public:
        BaseScene();
        virtual ~BaseScene();

    public:
        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;

        void LoadUI(); // UI를 Load 시켜두기. LoadMesh처럼 

    public:
        void SetPlayer(GameObject* player) { mPlayer = player; }
        GameObject* GetPlayer() { return mPlayer; }
        Camera* GetCamera() { return mCamera; }


    protected:  
        Camera* mCamera;

        GameObject* mPlayerHP;
        GameObject* mPlayerSP;
        GameObject* mMonsterHP;
        GameObject* mBossHP;

        GameObject* mPlayer;


    };
}

