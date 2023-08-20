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


    public:

    };
}

