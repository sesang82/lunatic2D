#pragma once
#include "ssMonsterScript.h"


namespace ss
{

    class WolfScript :
        public MonsterScript
    {
    public:
        WolfScript();
        virtual ~WolfScript();

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

        Vector3                 mLandingPos; // 플레이어에게 점프해서 착지할 플레이어의 위치
        Vector2                 mJumpDir; // 점프할 방향

        Vector2                 mVelocity; // 점프할때 쓸 속도


        bool                    mbNearAttack;
        bool                    mbFarAttack;

        bool                    mbNearAttacking;
        bool                    mbFarAttacking;

        bool                    mbHit;
        bool                    mbJumped; // HitGround 이미지 계속 업데이트 도는거 막기 위한 용도 

    public:
        // 함수는 각 하나의 역할만 하게 하기 
        void Idle();
        void Move();

        void Jump();
        void Fall();

        void Landing();

        void Stun();
        void Hit();

        void NearAttack();
        void NearAttackEnd();
        void FarAttack();

        void Dead();

        void Animation();

        void FarAttackEnd();
        void StunEnd();


        // 음악
        void Dead_Start();
        void Hit_Start();
        void NearAttack_sfx();


    public:
        void SetFirstPos(Vector3 pos) { mFirstPos = pos; }
        void SetMoveDir(Vector3 dir) { mDir = dir; }

        Vector3 GetDir() { return mDir; }
        Vector3 GetCurDir() { return mCurDir; }
        eMonsterState GetCurState() { return mCurState; }

        // 각 범위 인식용 충돌체에 이 함수 활용하기 
        void IsNearAttack(bool b) { mbNearAttack = b; }
        void IsFarAttack(bool b) { mbFarAttack = b; }

        void SetJumped(bool _b) { mbJumped = _b; }
        bool IsJumped() { return mbJumped; }

    };
}

