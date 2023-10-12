#pragma once
#include "ssScript.h"

namespace ss
{

    class SwordBulletScript :
        public Script
    {
    public:
        SwordBulletScript();
        virtual ~SwordBulletScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

    private:
        class CharacterState* mState;
        bool mIsGuard;





    };

}