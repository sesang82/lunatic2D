#pragma once
#include "ssMonsterScript.h"

namespace ss
{
    class QueenWolfScript :
        public MonsterScript
    {
    public:
        QueenWolfScript();
        virtual ~QueenWolfScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

    };

}

