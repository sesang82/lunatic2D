#pragma once
#include "ssState.h"

namespace ss
{
    class TraceState :
        public State
    {
    public:
        TraceState();
        ~TraceState();

    public:
        virtual void LateUpdate();
        virtual void Enter();
        virtual void Exit();
    };
}

