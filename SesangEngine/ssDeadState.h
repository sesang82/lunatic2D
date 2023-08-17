#pragma once
#include "ssState.h"

namespace ss
{
    class DeadState :
        public State
    {
    public:
        DeadState();
        ~DeadState();

    public:
        virtual void LateUpdate();
        virtual void Enter();
        virtual void Exit();
    };
}

