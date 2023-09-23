#pragma once
#include "ssScript.h"

namespace ss
{

    class PistolBulletScript :
        public Script
    {
        friend class PlayerPistolBullet;

    public:
        PistolBulletScript();
        virtual ~PistolBulletScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;


    private:
        bool mReverse;
        GameObject* mOriginOwner;
        float mSpeed;
        bool mIsGuard;

        class Animator*         mAnimator;
        class Transform*        mTransform;
        class Collider2D*       mCollider;
        class CharacterState*   mState;
        class MeshRenderer*     mMr;

        class Player*           mPlayer;
        class Monster*          mMonster;


    public:
        void SetReserve(bool reserve) { mReverse = reserve; }
        void SetOriginOwner(GameObject* obj) { mOriginOwner = obj; }

    };

}

