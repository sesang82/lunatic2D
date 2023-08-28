#pragma once
#include "ssScript.h"

// ���� ���� ���� ���ݿ� �浹ü 
namespace ss
{
    class HitGroundScript :
        public Script
    {
    public:
        HitGroundScript();
        ~HitGroundScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

    private:
        class Monster* mMonster; // ������ �ִϸ��̼� �ε��� ���� ���� ������ 

    public:
        void SetMonsterOwner(Monster* monster) { mMonster = monster; }

    };
}


