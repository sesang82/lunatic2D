#pragma once
#include "ssGameObject.h"

namespace ss
{
    class Bullet :
        public GameObject
    {
    public:
        Bullet();
        virtual ~Bullet();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;


    protected:
        class MeshRenderer* mr;
        Transform* tr;

    public:
        Transform* GetTransfrom() { return tr; }
    };
}
