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


    enum class eWeaponType
    {
        NONE,
        SWORD,
        BOW,
        PISTOL,
        END,
    };

    class PlayerScript :
        public Script
    {

        friend class AttackCollider;

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
        bool  mChangeFirst; // 무기 변경할 떄의 첫 애니메이션 땜에 만듦
        bool  IsDash;
        float DashDuration;
        float mSpeed;

        float mJumpPower;
        float mJumpHeight;
        float mFallTime;
        int   mJumpCount;

        int   mAttackCount;

        int   mWeaponCount;

        class Player* mPlayer;
        class Animator* mAnimator;
        class Rigidbody2D* mRigidbody;
        class CharacterState* mState;
        class Collider2D* mCollider;


        class AttackCollider* mAttackColliderObj;
        class Transform* mAttackColTr;
        class Collider2D* mAttackCol;
       
  


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


        // === 깜빡이는 용도
        bool isFlashing;// 깜빡이는 상태인지 여부
        float flashDuration; // 깜빡이는 지속 시간 (예: 1초)
        float flashingTime;// 깜빡이는 상태가 시작된 이후의 경과 시간


    public:
        void ChangeState(ePlayerState state){ mCurState = state;}
        void ChangeWeapon(eWeaponType type) { mWeaponType = type; }


        Vector3 GetPrevDir() { return mPrevDir; }
        Vector3 GetCurDir() { return mCurDir; }


        Vector2 GetPrevColSize() { return mPrevColSize; }
        Vector2 GetPrevColCenter() { return mPrevColCeter; }
        bool    GetPrev() { return mbPrev; }

        void  SetPrev(bool prev) { mbPrev = prev; }
        void TakeDamage(); // 플레이어가 데미지를 받았을 때 호출 


        // 함수는 각 하나의 역할만 하게 하기 
        void Idle();
        void Run();
        void Jump();
        void Fall();
        void Dash();
        void Hit();

        void Attack();

        void Animation();


        // ======
        void Enter_Floor();
        void Enter_TreeFloor();

        // ===== 특정 애니메이션 프레임 관련
        void S_Attack();
        void S_AttackEnd();

    };

}
