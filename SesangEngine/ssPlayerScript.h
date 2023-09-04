#pragma once
#include <ssScript.h>

namespace ss
{
    enum class ePlayerState
	{
		IDLE,
		RUN,
        DASH,
		JUMP,
        FALL,
        LADING,
        HANG,
        STUN,
        HIT,
        GUARD,
		ATTACK,
        SPATTACK,
		DEAD,
        END
	};


    class PlayerScript :
        public Script
    {

        friend class AttackCollider;
        friend class PlayerGuardColScript;

    public:
        PlayerScript();
        ~PlayerScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;


     
        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

        virtual void OnCollisionEnter(Collider2D* in, Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* in, Collider2D* other)  override;
        virtual void OnCollisionExit(Collider2D* in, Collider2D* other)  override;

        //virtual void OnTriggerEnter(Collider2D* collider) {};
        //virtual void OnTriggerStay(Collider2D* collider) {};
        //virtual void OnTriggerExit(Collider2D* collider) {};


    private:
        bool  mbIdleJump;
        bool  mbJump;
        bool  mbChange;
        bool  mChangeFirst; // ���� ������ ���� ù �ִϸ��̼� ���� ����
        bool  mbDash;
        float DashDuration;
        float mSpeed;
        float mTime;

        float mJumpPower;
        float mJumpHeight;
        float mFallTime;
        int   mJumpCount;

        int   mAttackCount;

        int   mWeaponCount;

        class Monster* mMonster;
        class Player* mPlayer;
        class Animator* mAnimator;
        class Rigidbody2D* mRigidbody;
        class CharacterState* mState;
        class Collider2D* mCollider;

        // ���ݿ� �浹ü 
        class AttackCollider* mAttackColliderObj;
        class Transform* mAttackColTr;
        class Collider2D* mAttackCol;

        // ���� �浹ü 
        class AttackCollider*   mGuardColObj;
        class Transform*        mGuardColTr;
        class Collider2D*       mGuardCol;

        // ���� start �浹ü 
        class Platform*     mBossStartColObj;


        // ���� ���� �� �� �մ� ���� ���� ������ ������ѵд�. (��ũ��Ʈ�� ��Ա� ����)
       
  


        Transform* mTransform;

        Vector3 mPos;
        Vector2 mVelocity;

        ePlayerState mCurState;
        ePlayerState mPrevState;

        eWeaponType  mWeaponType;

        Vector3     mPrevDir;
        Vector3     mCurDir;


        float   mCurrentHP;
        float   mCurrentSP;
        float   mDamage;
        float   mHeal;


        Vector2					mPrevColSize;
        Vector2					mPrevColCeter;
        bool                    mbPrev;

    public:
        void ChangeState(ePlayerState state){ mCurState = state;}
        void ChangeWeapon(eWeaponType type) { mWeaponType = type; }


        Vector3 GetPrevDir() { return mPrevDir; }
        Vector3 GetCurDir() { return mCurDir; }


        Vector2 GetPrevColSize() { return mPrevColSize; }
        Vector2 GetPrevColCenter() { return mPrevColCeter; }
        bool    GetPrev() { return mbPrev; }

        void  SetPrev(bool prev) { mbPrev = prev; }

        bool IsDash() { return mbDash; }


        void SetMonster(Monster* monster) { mMonster = monster; } // ���� ���� ������ �� �� ����� ������ 
        void SetBossStartColObj(Platform* obj) { mBossStartColObj = obj; }
 


        // �Լ��� �� �ϳ��� ���Ҹ� �ϰ� �ϱ� 
        void Idle();
        void Run();
        void Jump();
        void Fall();
        void Dash();
        void Hit();
        void Guard();

        void Attack();

        void Animation();


        // ======
        void Enter_Floor();
        void Enter_TreeFloor();

        // ===== Ư�� �ִϸ��̼� ������ ����
        void GuardEnd();
        void AttackEnd();

    };

}
