#pragma once
#include "ssScript.h"


namespace ss
{
    class UIFrameScript :
        public Script
    {
    public:
        UIFrameScript();
        ~UIFrameScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

    private:
        class Player* mPlayer; // 몬스터의 애니메이션 인덱스 정보 땜에 만들어둠 
        class Collider2D* mCol;
        class MeshRenderer* mMr;

    public:
        void SetPlayer(Player* player) { mPlayer = player; }


        bool mbSword;
        bool mbGauntlet;
        bool mbPistol;

    };
}


