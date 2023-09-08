#pragma once
#include "ssScript.h"

// ���� ���� ���� ���ݿ� �浹ü 
namespace ss
{
    class EffectScript :
        public Script
    {
    public:
        EffectScript();
        ~EffectScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

    private:
        class GameObject* mOwnerObj; // ������ �ִϸ��̼� �ε��� ���� ���� ������ 

    public:
        void SetOriginOwner(GameObject* Obj) { mOwnerObj = Obj; }

    };
}


