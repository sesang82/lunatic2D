#pragma once
#include "ssBullet.h"

namespace ss
{
    class StoneEyeProjectile 
        :public Bullet
    {
    public:
        StoneEyeProjectile();
        virtual ~StoneEyeProjectile();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;

        Vector3 GetFirstPos() { return mFirstPos; }

    public:

        Vector3 mFirstPos;

        float mTime;

    };
}

