#pragma once
#include "ssMonsterScript.h"

namespace ss
{
    class BigWolfScript :
        public MonsterScript
    {
    public:
        BigWolfScript();
        virtual ~BigWolfScript();

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


        Vector3                 mFirstPos;

        Vector3					mDir; // 어슬렁거리는 move상태일때만 사용할 거 
        Vector3					mCurDir; // 거리값으로 계산해낸 방향 

        Vector3                 mLandingPos; // 플레이어에게 점프해서 착지할 플레이어의 위치
     

        Vector2                 mVelocity; // 점프할때 쓸 속도


        bool                    mbNearAttack;
        bool                    mbFarAttack;

        bool                    mbNearAttacking;
        bool                    mbFarAttacking;

        bool                    mbHit;
        bool                    mbStomStart; // 애니메이션 계속 호출되는거 막기 위해서 해둠


        class Player*           mPlayer;

        // ==== 이펙트
        class Effect* mHitGround;

    public:
        // 함수는 각 하나의 역할만 하게 하기 
        void Spawn();
        void Idle();

        void Hit();

        void Appear();
        void Disappear();

        void Breath();
        void Howling();

        void Dash();

        void Stom_start();
        void Stoming();
        void Stom_end();


        void Dead();

        void Animation();




    public:
        void SetFirstPos(Vector3 pos) { mFirstPos = pos; }
        void SetMoveDir(Vector3 dir) { mDir = dir; }

        Vector3 GetDir() { return mDir; }
        Vector3 GetCurDir() { return mCurDir; }
        eMonsterState GetCurState() { return mCurState; }

        void SetPlayer(Player* player) { mPlayer = player; }

    };

}

