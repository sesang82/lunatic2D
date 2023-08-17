#pragma once

#include "ssGameObject.h"

namespace ss
{

    class AttackCollider :
        public GameObject
    {
    public:
        AttackCollider();
        virtual ~AttackCollider();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;


    protected:

    };
}

