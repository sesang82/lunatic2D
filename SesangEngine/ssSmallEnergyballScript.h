#pragma once
#include "ssScript.h"

namespace ss
{
    struct EnergyBallPattern 
    {
        Vector3 position;  // ���������� ��ġ
        float delay;  // ���� �� ������
    };


    class SmallEnergyballScript :
        public Script
    {
    public:
        SmallEnergyballScript();
        virtual ~SmallEnergyballScript();

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

        float mfTime;
        Vector3 mDir;

 





    public:
      

    };

}