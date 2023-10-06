#pragma once
#include "ssScript.h"

namespace ss
{
    struct EnergyBallPattern 
    {
        Vector3 position;  // 에너지볼의 위치
        float delay;  // 생성 전 딜레이
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
        class GameObject* mOwner;  // 모든 객체가 사용하도록 해둠 
        class Player* mPlayer;

        float mfTime;
        Vector3 mDir;

 





    public:
      

    };

}