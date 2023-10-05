#pragma once
#include "ssMonsterScript.h"


namespace ss
{


    enum class eBossType
    {
        STATUE,
        GODDNESS,
    };

    enum class eStatueState
    {
        MOVING_UP,
        MOVING_DOWN
    };

    enum class eStompState
    {
        NONE,
        SINGLE_STOMP,
        QUADRUPLE_STOMP, // 4번 연속적으로 빠르게 내려감 

    };

    enum class eSummonState
    {
        SPAWN_DIRHIT,
        SPAWN_SWORD,
        SWORD_FIRE,
    };

    class GoddnessScript :
        public MonsterScript
    {

        friend class Energyball;


    public:
        GoddnessScript();
        virtual ~GoddnessScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;


    private:
        Vector3                 mFirstPos;
        Vector3					mDir; // 어슬렁거리는 move상태일때만 사용할 거 
        Vector3					mCurDir; // 거리값으로 계산해낸 방향 

        Vector3                 mPlayerPos;

        int miStompCount;
        int miCompleteStompCount;
        bool mbStomp;
        bool mbFirstStomp;
        bool mTest;

        bool mbEnergySpawn;
        bool mbSwordSpawn;
        bool mbDirHitSpawn;
        bool mbFireSwordBullet;
        bool mLetterBox;

        bool mbMovingDown;
        bool mbMovingDiagonally;

        bool mbFreezingPos;
        bool mbFirstSpawnDone;
        bool mbSecondSpawnDone;
        bool mbSummonFinish;


        int miRandom;
        int mSpawnDirCount;

        float mfChangeTime;


        eBossType mBossType;
        eStatueState mStatueState;
        eStompState mStompState;
        eSummonState mSummonState;

        class Player* mPlayer;



        // ==== 이펙트
        class Effect* mHitGround;
        class Energyball* mEngeryball;



        // ==== 레터박스
        class Background* mLetterBoxBottom;
        class Background* mLetterBoxUP;
        class Background* mBossName;

        // 발판
        class Background* mPlatformLT;
        class Background* mPlatformLB;
        class Background* mPlatformRT;
        class Background* mPlatformRB;
        class Background* mPlatformMidle;


        class Platform* mGroundLT;
        class Platform* mGroundLB;
        class Platform* mGroundRT;
        class Platform* mGroundRB;
        class Platform* mGroundMidle;



        // 칼
        class SwordBullet* mSwordBullet_Mid;
        class SwordBullet* mSwordBullet_Left;
        class SwordBullet* mSwordBullet_Right;

        class Effect* mHitDir_Mid;
        class Effect* mHitDir_Left;
        class Effect* mHitDir_Right;

    public:
        // 함수는 각 하나의 역할만 하게 하기 
        void Intro();
        void IntroEnd();
        void Idle();

        void Hit();

        void Stomp_Ready();
        void Stomp_Ing();
        void Stomp_End();

        void Energyball_Start();
        void Energyball_ing();
        void Energyball_End();

        void Wind_Ready();

        void Dead();

        // --------
        void MoveBack();
        void MoveFront();

        void SummonSpear_Start();
        void SummonSpear_Ing();
        void SummonSpear_End();


        void CreateDirHit();
        void CreateSwordBullet();
        void FireSwordBullet();






    public:
        void SetFirstPos(Vector3 pos) { mFirstPos = pos; }
        void SetPlayer(Player* player) { mPlayer = player; }


        Vector3 GetDir() { return mDir; }
        Vector3 GetCurDir() { return mCurDir; }
        eMonsterState GetCurState() { return mCurState; }

        eStompState  GetRandomStompState();


        bool IsSpawnDirHit()
        {
            return mbDirHitSpawn;
        }

        int GetSpawnDirCount() { return mSpawnDirCount; }

        bool GetSummonFinish() { return mbSummonFinish; }

    };

}


