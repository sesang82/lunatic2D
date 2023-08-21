#pragma once
#include "ssGameObject.h"

namespace ss
{
    class RangeCollider
        : public GameObject
    {
    public:
        RangeCollider();
        virtual ~RangeCollider();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;

    };
}
