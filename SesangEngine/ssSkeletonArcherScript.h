#pragma once
#include "ssMonsterScript.h"


namespace ss
{

    class SkeletonArcherScript :
        public MonsterScript
    {
    public:
        SkeletonArcherScript();
        virtual ~SkeletonArcherScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;


    private:
        Vector3                 mFirstPos;

    public:
        void SetFirstPos(Vector3 pos) { mFirstPos = pos; }



    };
}

