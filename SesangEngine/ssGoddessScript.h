#pragma once
#include "ssMonsterScript.h"


namespace ss
{

    class GoddessScript :
        public MonsterScript
    {
    public:
        GoddessScript();
        virtual ~GoddessScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

    };
}
