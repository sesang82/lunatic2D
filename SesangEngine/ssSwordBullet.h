#pragma once
#include "ssBullet.h"

namespace ss
{
    class SwordBullet
        :public Bullet
    {
    public:
        SwordBullet();
        virtual ~SwordBullet();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;

    private:
        Vector3 mFirstPos;

        float mTime;

        class Animator* mAnimator;
        class Transform* mTransform;
        class GameObject* mOwner;  // 모든 객체가 사용하도록 해둠 
        class Player* mPlayer;
    public:



    };
}

