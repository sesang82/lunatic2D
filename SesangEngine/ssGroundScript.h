#pragma once
#include "ssScript.h"


namespace ss
{

    class Collider2D;

    class GroundScript :
        public Script
    {
    public:
        GroundScript();
        ~GroundScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

      

    public:
        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;


    private:
        Transform* mTransform;
    };

}

