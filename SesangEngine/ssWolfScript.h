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
        // === ���ݿ� �浹ü (�ִϸ��̼��� Ư�� �����ӿ� ������ �뵵�� ���)
        class AttackCollider* mAttackColliderObj;
        class Transform* mAttackColTr;
        class Collider2D* mAttackCol;

        // === Near ���� ���� �浹ü 
        class RangeCollider* mNearRangeColObj;
        class ArcherNearRangeScript* mNearRangeScript;
        class Transform* mNearTr;
        class Collider2D* mNearCol;


        // ==== ����Ʈ
        class Effect* mHitGround;

        Vector3                 mFirstPos;

        Vector3					mDir; // ����Ÿ��� move�����϶��� ����� �� 
        Vector3					mCurDir;

        Vector3                 mLandingPos; // �÷��̾�� �����ؼ� ������ �÷��̾��� ��ġ
        Vector2                 mJumpDir; // ������ ����
        
        Vector2                 mVelocity; // �����Ҷ� �� �ӵ�


        bool                    mbNearAttack;
        bool                    mbFarAttack;

        bool                    mbNearAttacking;
        bool                    mbFarAttacking;

        bool                    mbHit;
        bool                    mbJumped; // HitGround �̹��� ��� ������Ʈ ���°� ���� ���� �뵵 

    public:
        // �Լ��� �� �ϳ��� ���Ҹ� �ϰ� �ϱ� 
        void Idle();
        void Move();

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


    public:
        void SetFirstPos(Vector3 pos) { mFirstPos = pos; }
        void SetMoveDir(Vector3 dir) { mDir = dir; }

        Vector3 GetDir() { return mDir; }
        Vector3 GetCurDir() { return mCurDir; }
        eMonsterState GetCurState() { return mCurState; }

        // �� ���� �νĿ� �浹ü�� �� �Լ� Ȱ���ϱ� 
        void IsNearAttack(bool b) { mbNearAttack = b; }
        void IsFarAttack(bool b) { mbFarAttack = b; }

        void SetJumped(bool _b) { mbJumped = _b; }
        bool IsJumped() { return mbJumped; }

    };
}

