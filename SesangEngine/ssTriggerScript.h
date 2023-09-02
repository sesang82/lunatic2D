#pragma once
#include "ssScript.h"


namespace ss
{

    class Collider2D;

    class TriggerScript :
        public Script
    {
    public:
        TriggerScript();
        ~TriggerScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;



    public:
        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;


    private:
        Transform* mTransform;
        class Monster* mMonster;


    public:
        void SetMonster(Monster* monster) { mMonster = monster; }
		Monster* GetMonster() { return mMonster; }
    };

}

