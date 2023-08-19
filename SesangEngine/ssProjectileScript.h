#pragma once
#include "ssScript.h"

namespace ss
{
    class ProjectileScript :
        public Script
    {
    public:
        ProjectileScript();
        virtual ~ProjectileScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

    public:
        class CharacterState* mState;

    };



}

