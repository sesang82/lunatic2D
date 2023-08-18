#pragma once
#include "ssMonsterAttackColScript.h"

namespace ss
{
	class StoneEyeColScript :
		public MonsterAttackColScript
	{
    public:
        StoneEyeColScript();
        ~StoneEyeColScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;

	};
}


