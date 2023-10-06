#pragma once
#include "ssScript.h"


namespace ss
{
 
    class BigEnergyballScript :
        public Script
    {
    public:
        BigEnergyballScript();
        virtual ~BigEnergyballScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

    private:
        GameObject* mOriginOwner;
        class CharacterState* mState;
        bool mIsGuard;

        class Animator* mAnimator;
        class Transform* mTransform;
        class GameObject* mOwner;  // ��� ��ü�� ����ϵ��� �ص� 
        class Player* mPlayer;






    private:
        void SetOriginOwner(GameObject* obj) { mOriginOwner = obj; }




    };

}