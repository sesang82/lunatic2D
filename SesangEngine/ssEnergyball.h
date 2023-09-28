#pragma once
#include "ssBullet.h"

namespace ss
{

    class Energyball :
        public Bullet
    {

        class Engeryball;

    public:
        Energyball();
        virtual ~Energyball();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;

    private:
       class Animator* mAnimator;
       class Transform* mTransform;
       class GameObject* mOwner; 
       

       bool  mbTest;
    public:
       void SetOriginOwenr(GameObject* obj) { mOwner = obj; }

      
       
     


    };
}

