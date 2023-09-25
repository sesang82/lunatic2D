#pragma once
#include "ssScript.h"

namespace ss
{
    class PlayerAttackColScript :
        public Script
    {
    public:
        PlayerAttackColScript();
        ~PlayerAttackColScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

    private:

        class Animator* mAnimator;
        class Transform* mTransform;
        class Collider2D* mCollider;
        class CharacterState* mState;

        class Player* mPlayer;
        class StoneEye* mStoneEye;


    private:
        class GameObject* mOwnerObj; // 플레이어 스크립트 내용 써먹기 위해서 만들어둠 

    public:
        void SetOriginOwner(GameObject* Obj) { mOwnerObj = Obj; }

    };

}
