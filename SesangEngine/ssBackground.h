#pragma once
#include "ssGameObject.h"

namespace ss
{
    class MeshRenderer;

    class Background :
        public GameObject
    {
    public:
        Background();
        virtual ~Background();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;


    protected:
        MeshRenderer* mr;
        Transform* tr;
       class Animator* anim;
        class Monster* mBoss;

    public:
        Transform* GetTransfrom() { return tr; }
        void SetBoss(Monster* boss) { mBoss = boss; }

        void Fire_turnOnStart_End();
    };
}

