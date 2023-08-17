#pragma once
#include "ssState.h"

namespace ss
{
    class AttackState :
        public State
    {
    public:
        AttackState();
        ~AttackState();

    public:
        virtual void LateUpdate();
        virtual void Enter();
        virtual void Exit();

    };

}


