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
        // === ���ݿ� �浹ü (�ִϸ��̼��� Ư�� �����ӿ� ������ �뵵�� ���)
        class AttackCollider* mAttackColliderObj;
        class Transform* mAttackColTr;
        class Collider2D* mAttackCol;

        // === Near ���� ���� �浹ü 
        class RangeCollider* mNearRangeColObj;
        class ArcherNearRangeScript* mNearRangeScript;
        class Transform* mNearTr;
        class Collider2D* mNearCol;


        Vector3                 mFirstPos;

        Vector3					mDir; // ����Ÿ��� move�����϶��� ����� �� 
        Vector3					mCurDir; // �Ÿ������� ����س� ���� 

        Vector3                 mLandingPos; // �÷��̾�� �����ؼ� ������ �÷��̾��� ��ġ
     

        Vector2                 mVelocity; // �����Ҷ� �� �ӵ�


        bool                    mbNearAttack;
        bool                    mbFarAttack;

        bool                    mbNearAttacking;
        bool                    mbFarAttacking;

        bool                    mbHit;
        bool                    mbStomStart; // �ִϸ��̼� ��� ȣ��Ǵ°� ���� ���ؼ� �ص�


        class Player*           mPlayer;

        // ==== ����Ʈ
        class Effect* mHitGround;

    public:
        // �Լ��� �� �ϳ��� ���Ҹ� �ϰ� �ϱ� 
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

