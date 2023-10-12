#pragma once
#include "ssBullet.h"

namespace ss
{

    class BigEnergyball :
        public Bullet
    {
        friend class BigEnergyballScript;

        class Engeryball;

    public:
        BigEnergyball();
        virtual ~BigEnergyball();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;

    private:
        static int miSpawnedBallCount; // ��ü�� �����ɶ����� ������ �ʱⰪ���� �ʱ�ȭ�Ǳ� ������ static �� 
        BigEnergyball* mFirstEnergyball;
        BigEnergyball* mEnergyballs;
        float mfTime;
        Vector3 mPlayerPos;
        Vector3 mDirections;


        int miMaxSpawnedBalls;
        bool mbSpawnComplete;
        bool mStorePlayerPos;
        bool mbHit;

        std::vector<Vector2> spawnPatterns;

        class Animator* mAnimator;
        class Transform* mTransform;
        class GameObject* mOwner;  // ��� ��ü�� ����ϵ��� �ص� 
        class Player* mPlayer;


       static bool  mbSpawn;
        
    public:
        void SetOriginOwenr(GameObject* obj) { mOwner = obj; }
        void SetFirstEnergyball(BigEnergyball* obj) { mFirstEnergyball = obj; }
        void CreateBalls();
        void IncreaseSpawnCount() { ++miSpawnedBallCount; }
        void InitializeSpawnCount() { miSpawnedBallCount = 0; }
        int GetSpawnCount() { return miSpawnedBallCount; }

        void SetDir(Vector3 dir) { mDirections = dir; }
        Vector3 GetDir() { return mDirections; }

        void SetHit(bool hit) { mbHit = hit; }
        bool GetHit() { return mbHit; }
        void SetInstance(BigEnergyball* inst) { mEnergyballs = inst; }


        void HitEnd();
        


    



    };
}

