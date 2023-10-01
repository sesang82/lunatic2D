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
        GameObject* mOriginOwner;
        class CharacterState* mState;
        bool mIsGuard;

    private:
        void SetOriginOwner(GameObject* obj) { mOriginOwner = obj; }




    };

}