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
         class GameObject* mOwnerObj; // 몬스터의 애니메이션 인덱스 정보 땜에 만들어둠 

     public:
         void SetOwnerObj(GameObject* monster) { mOwnerObj = monster; }

   
        
    };
}

