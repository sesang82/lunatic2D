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
        Vector3					mDir; // ����Ÿ��� move�����϶��� ����� �� 
        Vector3					mCurDir; // �Ÿ������� ����س� ���� 

        int miStompCount;
        int miCompleteStompCount;
        bool mbStomp;
        bool mTest;

        bool mbEnergySpawn;
    


        eBossType mBossType;
        eStatueState mStatueState;
        class Player* mPlayer;


        // ==== ����Ʈ
        class Effect* mHitGround;
        class Energyball* mEngeryball;

    public:
        // �Լ��� �� �ϳ��� ���Ҹ� �ϰ� �ϱ� 
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


