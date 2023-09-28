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

        int miStompCount;
        int miCompleteStompCount;
        bool mbStomp;
        bool mTest;

        bool mbEnergySpawn;
    


        eBossType mBossType;
        eStatueState mStatueState;
        class Player* mPlayer;


        // ==== 이펙트
        class Effect* mHitGround;
        class Energyball* mEngeryball;

    public:
        // 함수는 각 하나의 역할만 하게 하기 
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





    public:
        void SetFirstPos(Vector3 pos) { mFirstPos = pos; }
        void SetPlayer(Player* player) { mPlayer = player; }

        Vector3 GetDir() { return mDir; }
        Vector3 GetCurDir() { return mCurDir; }
        eMonsterState GetCurState() { return mCurState; }

    };
}


