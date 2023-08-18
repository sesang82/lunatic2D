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

        Vector3					mPrevDir;
        Vector3					mCurDir;

        eMonsterState			 mCurState;
        eMonsterState			 mPrevState;



        // === ���ݿ� �浹ü 
        class AttackCollider* mAttackColliderObj;
        class Transform* mAttackColTr;
        class Collider2D* mAttackCol;

        // === �Ϲ� ���Ϳ� hp��
        class MonsterBar* mMonsterHpBarObj;
        class Transform* mMonsterHpBarTr;




    public:
        // �Լ��� �� �ϳ��� ���Ҹ� �ϰ� �ϱ� 
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


    };
}

