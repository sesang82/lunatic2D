#pragma once
#include "ssScript.h"

namespace ss
{
    // 패럴럭스 스크롤링 기법용 스크립트  

    class ParalloxScript :
        public Script
    {
    public:
        ParalloxScript();
        ~ParalloxScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

    private:
        class Player* mPlayer; // 몬스터의 애니메이션 인덱스 정보 땜에 만들어둠 

    public:
        void SetPlayer(Player* monster) { mPlayer = monster; }
    };
}

