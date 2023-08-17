#pragma once
#include "..\\Engine_SOURCE\\ssScene.h"


namespace ss
{
    class Player;
    class Camera;

    class Progressbar;
    // ������ �ߺ��Ǵ� ��ҵ��� �ش� Ŭ������ ��� ���� �� �ְ� �����ϱ� 
    // ��ӹ޾ƾߵ� �͵��� protected �ϸ� �ɵ� 

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

        void LoadUI(); // UI�� Load ���ѵα�. LoadMeshó�� 

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

