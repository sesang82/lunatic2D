#pragma once
#include "ssGameObject.h"

namespace ss
{
    class Item :
        public GameObject
    {
    public:
        Item();
        virtual ~Item();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;


    protected:
        GameObject* mItemOwner;


    public:
        void SetItemOwner(GameObject* obj) { mItemOwner = obj; }

    };
}

