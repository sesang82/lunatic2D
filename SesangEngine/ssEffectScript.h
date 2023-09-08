#pragma once
#include "ssScript.h"

// 스톤 아이 근접 공격용 충돌체 
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
        class GameObject* mOwnerObj; // 몬스터의 애니메이션 인덱스 정보 땜에 만들어둠 

    public:
        void SetOriginOwner(GameObject* Obj) { mOwnerObj = Obj; }

    };
}


