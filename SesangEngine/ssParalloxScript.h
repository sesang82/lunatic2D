#pragma once
#include "ssScript.h"

namespace ss
{
    // �з����� ��ũ�Ѹ� ����� ��ũ��Ʈ  

    class ParalloxScript :
        public Script
    {
    public:
        ParalloxScript();
        ~ParalloxScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

    private:
        class Player* mPlayer; // ������ �ִϸ��̼� �ε��� ���� ���� ������ 

        Vector3 mPlayerPrePos;
        Vector3 mPlayerCurPos;

        bool    mFirst;
        float   mSpeed;

        float   mMinX;
        float   mMaxX;

        float   mMinY;
        float   mMaxY;

        class BackGround* mBG1;
        class BackGround* mBG2;
        class BackGround* mBG3;
        class BackGround* mBG4;

    public:
        void SetPlayer(Player* monster) { mPlayer = monster; }
        void SetSpeed(float speed) { mSpeed = speed; }


        void SetMinX(float min) { mMinX = min; }
        void SetMaxX(float max) { mMaxX = max; }

        // === �������� x 
        void SetMinY(float min) { mMinY = min; } 
        void SetMaxY(float max) { mMaxY = max; } 

      
    };
}

