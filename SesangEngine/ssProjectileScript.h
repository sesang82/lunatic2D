#pragma once
#include "ssMonsterAttackColScript.h"

namespace ss
{
    class ProjectileScript :
        public MonsterAttackColScript
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

    private:
        bool mReverse;
       GameObject* mOriginOwner;

    public:
        void SetReserve(bool reserve) { mReverse = reserve; }
        void SetOriginOwner(GameObject* obj) { mOriginOwner = obj; }
 
    };



}

