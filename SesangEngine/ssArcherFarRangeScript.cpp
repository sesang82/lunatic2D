#include "ssArcherFarRangeScript.h"
#include "ssMonster.h"
#include "ssGameState.h"
#include "ssTime.h"
#include "ssSkeletonArcherScript.h"

namespace ss
{
	ArcherFarRangeScript::ArcherFarRangeScript()
	{
	}
	ArcherFarRangeScript::~ArcherFarRangeScript()
	{
	}
	void ArcherFarRangeScript::Initialize()
	{

	}
	void ArcherFarRangeScript::Update()
	{


	}
	void ArcherFarRangeScript::OnCollisionEnter(Collider2D* other)
	{
		//if (other->GetName() == L"colHit_player")// 충돌체 이름 
		if (other->GetOwner()->GetName() == L"Player")
		{
			SkeletonArcherScript* stonescript = mOwner->GetComponent<SkeletonArcherScript>();

			if (stonescript->mCurState != ss::eMonsterState::STUN)
			{
				stonescript->mCurState = ss::eMonsterState::FARATTACK;

				stonescript->IsFarAttack(true);
			}

		}
	}
	void ArcherFarRangeScript::OnCollisionStay(Collider2D* other)
	{
	}
	void ArcherFarRangeScript::OnCollisionExit(Collider2D* other)
	{
		if (other->GetOwner()->GetName() == L"Player")
		{
			SkeletonArcherScript* stonescript = mOwner->GetComponent<SkeletonArcherScript>();

			if (stonescript->mCurState != ss::eMonsterState::STUN)
			{
				SkeletonArcherScript* stonescript = mOwner->GetComponent<SkeletonArcherScript>();
				stonescript->mCurState = ss::eMonsterState::MOVE;
				stonescript->IsFarAttack(false);
			}
		}
	}
}

