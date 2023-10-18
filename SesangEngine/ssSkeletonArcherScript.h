#pragma once
#include "ssMonsterScript.h"


namespace ss
{

    class SkeletonArcherScript :
        public MonsterScript
    {
        friend class ArcherNearRangeScript;
        friend class ArcherFarRangeScript;


    public:
        SkeletonArcherScript();
        virtual ~SkeletonArcherScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;


    private:
        Vector3                 mFirstPos;

        Vector3					mDir; // ����Ÿ��� move�����϶��� ����� �� 
        Vector3					mCurDir;


        bool                    mbNearAttack;
        bool                    mbFarAttack;
        bool                    mbAttacking;


        // === ���ݿ� �浹ü (�ִϸ��̼��� Ư�� �����ӿ� ������ �뵵�� ���)
        class AttackCollider* mAttackColliderObj;
        class Transform* mAttackColTr;
        class Collider2D* mAttackCol;

        // === Near ���� ���� �浹ü 
        class RangeCollider* mNearRangeColObj;
        class ArcherNearRangeScript* mNearRangeScript;
        class Transform* mNearTr;
        class Collider2D* mNearCol;


        // === far ���� ���� �浹ü 
        class RangeCollider* mFarRangeColObj;
        class Transform* mFarTr;
        class Collider2D* mFarCol;


        // === Far ���ݿ� ȭ�� �浹ü 
        class ArcherArrow* mArrowObj;
        class ArcherArrowScript* mArrowScript;
        class Transform* mArrowTr;
        class Collider2D* mArrowCol;

        // ==== ����Ʈ
        class Effect* mEffectObj;


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
        void NearAttackEnd();
        void FarAttack();

        void Dead();

        void Animation();

        void FarAttackEnd();
        void StunEnd();

        // ���� ����
        void Dead_Start();
        void Hit_Start();
        void Near_sfx();
        void Far_Ready_sfx();
        void Far_sfx();



    public:
        void SetFirstPos(Vector3 pos) { mFirstPos = pos; }
        void SetMoveDir(Vector3 dir) { mDir = dir; }

        Vector3 GetDir() { return mDir; }
        Vector3 GetCurDir() { return mCurDir; }
        eMonsterState GetCurState() { return mCurState; }

        // �� ���� �νĿ� �浹ü�� �� �Լ� Ȱ���ϱ� 
        void IsNearAttack(bool b) { mbNearAttack = b; }
        void IsFarAttack(bool b) { mbFarAttack = b; }



    };
}

