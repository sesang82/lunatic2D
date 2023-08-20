#pragma once
#include "ssScript.h"


namespace ss
{

    // 몬스터들 공격용 충돌체 스크립트 (상속용) 
    class MonsterAttackColScript :
        public Script
    {
    public:
        MonsterAttackColScript();
        ~MonsterAttackColScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;
  
       
    protected:
        class Animator* mAnimator;
        class Transform* mTransform;
        class Collider2D* mCollider;
        class CharacterState* mState;

        class Player* mPlayer;
        class Monster* mMonster;
    
    
    
    };
}

