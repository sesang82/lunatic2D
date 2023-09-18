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
        OVERLOAD_READY, // 모든 무기 공통 
        OVERLOAD_START,
        OVERLOADING,
        OVERLOAD_END,
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

        //virtual void OnTriggerEnter(Collider2D* collider) {};
        //virtual void OnTriggerStay(Collider2D* collider) {};
        //virtual void OnTriggerExit(Collider2D* collider) {};

        void BindConstantBuffer();

    private:
        bool  mbIdleJump;
        bool  mbJump;
        bool  mbChange;
        bool  mChangeFirst; // 무기 변경할 떄의 첫 애니메이션 땜에 만듦
        bool  mbDash;
        float DashDuration;
        float mSpeed;
        float mTime;

        bool mHit;
        bool mbHitting;
        bool mbspAttack;
        bool mbUseOverload;
        bool mbOverloading;
        bool mbPlayerOverloadingEffet;
        bool mTurnOverload;
  

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

        // 공격용 충돌체 
        class AttackCollider* mAttackColliderObj;
        class Transform* mAttackColTr;
        class Collider2D* mAttackCol;

        // 방어용 충돌체 
        class AttackCollider*   mGuardColObj;
        class Transform*        mGuardColTr;
        class Collider2D*       mGuardCol;

        // 보스 start 충돌체 
        class Platform*         mBossStartColObj;

        class Effect*           mHitEffect;
        class Effect*           mSPEffect;

        class Effect*           mPlayerOverloadEffect;
        class Effect*           mOverloadUIEffect;
        class Effect*           mTopHorizion;
        class Effect*           mBottomHorizion;
        class Effect*           mLT;
        class Effect*           mLB;
        class Effect*           mRT;
        class Effect*           mRB;


        // 몬스터 갖고 올 수 잇는 벡터 만들어서 여따가 저장시켜둔다. (스크립트에 써먹기 위함)
       
  


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

        ePlayerState GetState() { return mCurState; }

        Vector2 GetPrevColSize() { return mPrevColSize; }
        Vector2 GetPrevColCenter() { return mPrevColCeter; }
        bool    GetPrev() { return mbPrev; }

        void  SetPrev(bool prev) { mbPrev = prev; }

        bool IsDash() { return mbDash; }


        void SetMonster(Monster* monster) { mMonster = monster; } // 몬스터 정보 가져다 쓸 때 쓸라고 만들어둠 
        void SetBossStartColObj(Platform* obj) { mBossStartColObj = obj; }

        bool IsUseOverload() { return mbUseOverload;}
 


        // 함수는 각 하나의 역할만 하게 하기 
        void Idle();
        void Run();
        void Jump();
        void Fall();
        void Dash();
        void Hit();
        void Guard();

        void Attack();
        void SPAttack();

        void Overload_Ready(); // 모든 무기 공통 
        void Overload_Start();
        void Overloading();
        void Overload_End();

        void Animation();


        // ======
        void Enter_Floor();
        void Enter_TreeFloor();

        // ===== 특정 애니메이션 프레임 관련
        void GuardEnd();
        void AttackEnd();

    };

}
