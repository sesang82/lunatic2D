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
        static Energyball* mFirstEnergyball; // ù��° ���������� ���� ������ �����ص� �뵵


        int miMaxSpawnedBalls;

        std::vector<Vector2> spawnPatterns; 

       class Animator* mAnimator;
       class Transform* mTransform;
       class GameObject* mOwner;  // ��� ��ü�� ����ϵ��� �ص� 
       

       bool  mbTest;
    public:
       void SetOriginOwenr(GameObject* obj) { mOwner = obj; }
       void SetFirstEnergyball(Energyball* obj) { mFirstEnergyball = obj; }
       void CreateEnergyball();
      
       
     


    };
}

