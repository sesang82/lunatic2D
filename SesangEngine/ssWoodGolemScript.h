#pragma once
#include "ssMonsterScript.h"


namespace ss
{
    class WoodGolemScript :
        public MonsterScript
    {
    public:
        WoodGolemScript();
        virtual ~WoodGolemScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

    private:
        // === 공격용 충돌체 (애니메이션의 특정 프레임에 공격할 용도로 사용)
        class AttackCollider* mAttackColliderObj;
        class Transform* mAttackColTr;
        class Collider2D* mAttackCol;

        // === Near 공격 판정 충돌체 
        class RangeCollider* mNearRangeColObj;
        class ArcherNearRangeScript* mNearRangeScript;
        class Transform* mNearTr;
        class Collider2D* mNearCol;


        // ==== 이펙트
        class Effect* mHitGround;

        Vector3                 mFirstPos;

        Vector3					mDir; // 어슬렁거리는 move상태일때만 사용할 거 
        Vector3					mCurDir;


        bool                    mbNearAttack;
        bool                    mbFarAttack;

        bool                    mbNearAttacking;
        bool                    mbFarAttacking;
        bool                    mbHit;
        bool                    mIsDead;
        bool                    mbPaunched; // Paunched 이미지 계속 업데이트 도는거 막기 위한 용도 




    public:
        // 함수는 각 하나의 역할만 하게 하기 
        void Idle();
        void Move();
        void Tracer();

        void Jump();
        void Fall();

        void Landing();

        void Stun();
        void Hit();
    
        void NearAttack();
        void FarAttack();

        void Dead();

        void Animation();

        void FarAttackEnd();
        void StunEnd();

        // 음악
        void Hit_Start();
        void Dead_Start();


    public:
        void SetFirstPos(Vector3 pos) { mFirstPos = pos; }
        void SetMoveDir(Vector3 dir) { mDir = dir; }

        Vector3 GetDir() { return mDir; }
        Vector3 GetCurDir() { return mCurDir; }
        eMonsterState GetCurState() { return mCurState; }

        // 각 범위 인식용 충돌체에 이 함수 활용하기 
        void IsNearAttack(bool b) { mbNearAttack = b; }
        void IsFarAttack(bool b) { mbFarAttack = b; }
        
        void SetPaunched(bool _b) { mbPaunched = _b; } 
        bool IsPaunched() { return mbPaunched; }


    };
}

