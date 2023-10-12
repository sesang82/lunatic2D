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
        static int miSpawnedBallCount; // 객체가 생성될때마다 생성자 초기값으로 초기화되기 때문에 static 씀 
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
        class GameObject* mOwner;  // 모든 객체가 사용하도록 해둠 
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

