#pragma once
#include "ssMonster.h"


namespace ss
{
    class StoneEye :
        public Monster
    {
    public:
        StoneEye();
        virtual ~StoneEye();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;
    };
}

