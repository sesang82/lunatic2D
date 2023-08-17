#pragma once
#include "ssState.h"

namespace ss
{
    class IdleState :
        public State
    {
    public:
        IdleState();
        ~IdleState();

    public:
        virtual void LateUpdate();
        virtual void Enter();
        virtual void Exit();

   
    };
}


