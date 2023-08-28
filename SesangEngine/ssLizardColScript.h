#pragma once
#include "ssMonsterAttackColScript.h"

// 스톤 아이 근접 공격용 충돌체 
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


