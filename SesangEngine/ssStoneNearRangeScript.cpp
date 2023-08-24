#include "ssStoneNearRangeScript.h"
#include "ssStoneEye.h"
#include "ssGameState.h"
#include "ssTime.h"
#include "ssStoneEyeScript.h"

namespace ss
{
	StoneNearRangeScript::StoneNearRangeScript()
	{
	}
	StoneNearRangeScript::~StoneNearRangeScript()
	{
	}
	void StoneNearRangeScript::Initialize()
	{
		mMonster = (StoneEye*)GetOwner();

	}
	void StoneNearRangeScript::Update()
	{
	}
	void StoneNearRangeScript::OnCollisionEnter(Collider2D* other)
	{
		//if (other->GetName() == L"colHit_player")// 충돌체 이름 
		if(other->GetOwner()->GetName() == L"Player")
		{

			StoneEyeScript* stonescript = mOwner->GetComponent<StoneEyeScript>();

			if (stonescript->mCurState != ss::eMonsterState::STUN)
			{
				stonescript->mCurState = ss::eMonsterState::NEARATTACK;

				stonescript->IsNearAttack(true);
			}

		}
	}
	void StoneNearRangeScript::OnCollisionStay(Collider2D* other)
	{
	}
	void StoneNearRangeScript::OnCollisionExit(Collider2D* other)
	{
		if (other->GetOwner()->GetName() == L"Player")
		{
			StoneEyeScript* stonescript = mOwner->GetComponent<StoneEyeScript>();

			if (stonescript->mCurState != ss::eMonsterState::STUN)
			{
				StoneEyeScript* stonescript = mOwner->GetComponent<StoneEyeScript>();
				stonescript->mCurState = ss::eMonsterState::FARATTACK;
				stonescript->IsNearAttack(false);
			}
		}
	}
}
