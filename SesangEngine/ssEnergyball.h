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
        static int miSpawnedBallCount; // ��ü�� �����ɶ����� ������ �ʱⰪ���� �ʱ�ȭ�Ǳ� ������ static �� 
        Energyball* mFirstEnergyball; 
        float mfTime;
        Vector3 mPlayerPos;


        int miMaxSpawnedBalls;
        bool mbSpawnComplete;
        bool mStorePlayerPos;

        std::vector<Vector2> spawnPatterns; 

       class Animator* mAnimator;
       class Transform* mTransform;
       class GameObject* mOwner;  // ��� ��ü�� ����ϵ��� �ص� 
       class Player* mPlayer;
       

       bool  mbTest;
    public:
       void SetOriginOwenr(GameObject* obj) { mOwner = obj; }
       void SetFirstEnergyball(Energyball* obj) { mFirstEnergyball = obj; }
       void CreateEnergyball();
       void IncreaseSpawnCount() { ++miSpawnedBallCount; }

    
       
 
    };
}

