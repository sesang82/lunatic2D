#include "ssStoneFarRangeScript.h"
#include "ssStoneEye.h"
#include "ssGameState.h"
#include "ssTime.h"
#include "ssStoneEyeScript.h"

namespace ss
{
	StoneFarRangeScript::StoneFarRangeScript()
	{
	}
	StoneFarRangeScript::~StoneFarRangeScript()
	{
	}
	void StoneFarRangeScript::Initialize()
	{		

	}
	void StoneFarRangeScript::Update()
	{


	}
	void StoneFarRangeScript::OnCollisionEnter(Collider2D* other)
	{
		//if (other->GetName() == L"colHit_player")// 충돌체 이름 
		if (other->GetOwner()->GetName() == L"Player")
		{
			StoneEyeScript* stonescript = mOwner->GetComponent<StoneEyeScript>();

			if (stonescript->mCurState != ss::eMonsterState::STUN)
			{
				stonescript->mCurState = ss::eMonsterState::FARATTACK;

				stonescript->IsFarAttack(true);
			}

		}
	}
	void StoneFarRangeScript::OnCollisionStay(Collider2D* other)
	{
	}
	void StoneFarRangeScript::OnCollisionExit(Collider2D* other)
	{
		if (other->GetOwner()->GetName() == L"Player")
		{
			StoneEyeScript* stonescript = mOwner->GetComponent<StoneEyeScript>();

			if (stonescript->mCurState != ss::eMonsterState::STUN)
			{
				StoneEyeScript* stonescript = mOwner->GetComponent<StoneEyeScript>();
				stonescript->mCurState = ss::eMonsterState::MOVE;
				stonescript->IsFarAttack(false);
			}
		}
	}
}
