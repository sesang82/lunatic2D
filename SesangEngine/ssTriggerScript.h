#pragma once
#include "ssScript.h"


namespace ss
{

    class Collider2D;

    class TriggerScript :
        public Script
    {
    public:
        TriggerScript();
        ~TriggerScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;



    public:
        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;


    private:
        Transform* mTransform;
        class Monster* mMonster;
        class Player* mPlayer;

        class GameObject* mCamera;
        class GameObject* mUICamera;

        class GameObject* mLetterBoxUP;
        class GameObject* mLetterBoxBottom;
        class GameObject* mBossName;

    public:
        void SetMonster(Monster* monster) { mMonster = monster; }
		Monster* GetMonster() { return mMonster; }

        void SetCamera(GameObject* camera) { mCamera = camera; }
        void SetUICamera(GameObject* camera) { mUICamera = camera; }

        void SetPlayer(Player* player) { mPlayer = player; }
    };

}

