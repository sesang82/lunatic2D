#pragma once
#include "ssMonsterAttackColScript.h"

// ���� ���� ���� ���ݿ� �浹ü 
namespace ss
{
    class LizardColScript :
        public MonsterAttackColScript
    {
    public:
        LizardColScript();
        ~LizardColScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

    };
}


