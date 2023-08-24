#pragma once
#include "ssGameObject.h"

namespace ss
{
    class Effect :
        public GameObject
    {
        public:
            Effect();
            virtual ~Effect();

            virtual void Initialize() override;
            virtual void Update() override;
            virtual void LateUpdate() override;
            virtual void Render() override;


        protected:    
            GameObject* mEffectOwner;


    public:
        void SetEffectOwner(GameObject* obj) { mEffectOwner = obj;}
 
    };
}

