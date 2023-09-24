#pragma once
#include "ssScript.h"

namespace ss
{


    class GolemFarRangeScript :
        public Script
    {
    public:
        GolemFarRangeScript();
        ~GolemFarRangeScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;


    private:
        GameObject* mOwner;


    public:

        void SetOwner(GameObject* _object) { mOwner = _object; }
    };

}
