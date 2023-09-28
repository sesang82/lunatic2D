#pragma once
#include "ssScript.h"
#include "ssEnergyball.h"



namespace ss
{
    struct EnergyBallPattern 
    {
        Vector3 position;  // 에너지볼의 위치
        float delay;  // 생성 전 딜레이
    };


    class EnergyballScript :
        public Script
    {
    public:
        EnergyballScript();
        virtual ~EnergyballScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;



    private:
        std::vector<EnergyBallPattern> firstGroupPattern;
        std::vector<EnergyBallPattern> secondGroupPattern;
        std::vector<EnergyBallPattern> ThirdGroupPattern;
        std::vector<EnergyBallPattern> FourGroupPattern;

        Vector3  mvStartPosition;



    public:
        void SpawnEnergyBalls();
        Energyball* CreateBall(Vector3 position);

        Vector3  CalculateSpawnPosition(Vector3 startPosition, int group, int index);


    };

}