#pragma once
#include "ssScript.h"


namespace ss
{

    class Collider2D;

    class WallScript :
        public Script
    {
    public:
        WallScript();
        ~WallScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;



    public:
        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;


    private:
        Transform* mTransform;
       class Player* mPlayer;


    public:
        void SetPlayer(Player* player) { mPlayer = player; }
    };

}

