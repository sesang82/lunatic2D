#pragma once
#include "ssScript.h"

namespace ss
{
    class ItemScript :
        public Script
    {    
     public:
         ItemScript();
         ~ItemScript();

     public:
         virtual void Initialize() override;
         virtual void Update() override;

         virtual void OnCollisionEnter(Collider2D* other) override;
         virtual void OnCollisionStay(Collider2D* other) override;
         virtual void OnCollisionExit(Collider2D* other) override;

     private:
         class GameObject* mOwnerObj; // ������ �ִϸ��̼� �ε��� ���� ���� ������ 

     public:
         void SetOwnerObj(GameObject* monster) { mOwnerObj = monster; }

   
        
    };
}

