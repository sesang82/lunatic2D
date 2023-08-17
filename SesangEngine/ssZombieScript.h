#pragma once
#include "ssMonsterScript.h"


namespace ss
{
    class ZombieScript :
        public MonsterScript
    {
    public:
        ZombieScript();
        virtual ~ZombieScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

    };
}

