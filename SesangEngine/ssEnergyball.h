#pragma once
#include "ssBullet.h"

namespace ss
{

    class Energyball :
        public Bullet
    {

        class Engeryball;

    public:
        Energyball();
        virtual ~Energyball();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;

    private:
        static int miSpawnedBallCount; // 객체가 생성될때마다 생성자 초기값으로 초기화되기 때문에 static 씀 
        Energyball* mFirstEnergyball; 
        float mfTime;
        Vector3 mPlayerPos;


        int miMaxSpawnedBalls;
        bool mbSpawnComplete;
        bool mStorePlayerPos;

        std::vector<Vector2> spawnPatterns; 

       class Animator* mAnimator;
       class Transform* mTransform;
       class GameObject* mOwner;  // 모든 객체가 사용하도록 해둠 
       class Player* mPlayer;
       

       bool  mbTest;
    public:
       void SetOriginOwenr(GameObject* obj) { mOwner = obj; }
       void SetFirstEnergyball(Energyball* obj) { mFirstEnergyball = obj; }
       void CreateEnergyball();
       void IncreaseSpawnCount() { ++miSpawnedBallCount; }

    
       
 
    };
}

