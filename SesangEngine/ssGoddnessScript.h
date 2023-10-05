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
        QUADRUPLE_STOMP, // 4�� ���������� ������ ������ 

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
        Vector3					mDir; // ����Ÿ��� move�����϶��� ����� �� 
        Vector3					mCurDir; // �Ÿ������� ����س� ���� 

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



        // ==== ����Ʈ
        class Effect* mHitGround;
        class Energyball* mEngeryball;



        // ==== ���͹ڽ�
        class Background* mLetterBoxBottom;
        class Background* mLetterBoxUP;
        class Background* mBossName;

        // ����
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



        // Į
        class SwordBullet* mSwordBullet_Mid;
        class SwordBullet* mSwordBullet_Left;
        class SwordBullet* mSwordBullet_Right;

        class Effect* mHitDir_Mid;
        class Effect* mHitDir_Left;
        class Effect* mHitDir_Right;

    public:
        // �Լ��� �� �ϳ��� ���Ҹ� �ϰ� �ϱ� 
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


