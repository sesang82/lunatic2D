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
        class GameObject* mOwner;  // ��� ��ü�� ����ϵ��� �ص� 
        class Player* mPlayer;
    public:



    };
}

