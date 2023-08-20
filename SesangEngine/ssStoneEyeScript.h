#pragma once
#include "ssMonsterScript.h"


namespace ss
{

    class StoneEyeScript :
        public MonsterScript
    {
    public:
        StoneEyeScript();
        virtual ~StoneEyeScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

 

    private:
        float L_Boundary;
        float R_Boundary;

        class Animator* mAnimator;
        class Rigidbody2D* mRigidbody;
        class Transform* mTransform;
        class Collider2D* mCollider;
        class MeshRenderer* mMeshRenderer;

        class Player* mPlayer;

        Vector3					mDir; // 어슬렁거리는 move상태일때만 사용할 거 
        Vector3					mCurDir;

        Vector3                 mFirstPos; 

        eMonsterState			 mCurState;
        eMonsterState			 mPrevState;



        // === 공격용 충돌체 
        class AttackCollider* mAttackColliderObj;
        class Transform* mAttackColTr;
        class Collider2D* mAttackCol;

        // === Far 공격용 충돌체 
        class StoneEyeProjectile* mArrowObj;
        class ProjectileScript* mArrowScript;
        class Transform* mArrowTr;
        class Collider2D* mArrowCol;



    public:
        // 함수는 각 하나의 역할만 하게 하기 
        void Move();
        void Tracer();

        void Jump();
        void Fall();

        void Landing();

        void Stun();
        void Hit();

        void Guard();

        void NearAttack();
        void FarAttack();

        void Dead();

        void Animation();


        void NearAttackEnd();

    public:
        void SetFirstPos(Vector3 pos) { mFirstPos = pos; }
        void SetMoveDir(Vector3 dir) { mDir = dir; }



    };
}

