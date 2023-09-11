#pragma once
#include "ssScript.h"

namespace ss
{
    // �з����� ��ũ�Ѹ� ����� ��ũ��Ʈ  

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
        class Player* mPlayer; // ������ �ִϸ��̼� �ε��� ���� ���� ������ 

    public:
        void SetPlayer(Player* monster) { mPlayer = monster; }
    };
}

