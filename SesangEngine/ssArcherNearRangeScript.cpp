#include "ssArcherNearRangeScript.h"
#include "ssMonster.h"
#include "ssGameState.h"
#include "ssTime.h"
#include "ssSkeletonArcherScript.h"


namespace ss
{
	ArcherNearRangeScript::ArcherNearRangeScript()
	{
	}
	ArcherNearRangeScript::~ArcherNearRangeScript()
	{
	}
	void ArcherNearRangeScript::Initialize()
	{
	}
	void ArcherNearRangeScript::Update()
	{
	}
	void ArcherNearRangeScript::OnCollisionEnter(Collider2D* other)
	{
		if (other->GetOwner()->GetName() == L"Player")
		{

			SkeletonArcherScript* Archerscript = mOwner->GetComponent<SkeletonArcherScript>();

			// stun상태일 시 공격하지 않아야하므로 
			if (Archerscript->mCurState != ss::eMonsterState::STUN && Archerscript->mCurState != ss::eMonsterState::DEAD)
			{
				Archerscript->mCurState = ss::eMonsterState::NEARATTACK;

				Archerscript->IsNearAttack(true);
			}

		}
	}
	
	void ArcherNearRangeScript::OnCollisionStay(Collider2D* other)
	{

		if (other->GetOwner()->GetName() == L"Player")
		{
			SkeletonArcherScript* Archerscript = mOwner->GetComponent<SkeletonArcherScript>();

			if (Archerscript->mCurState != ss::eMonsterState::STUN && Archerscript->mCurState != ss::eMonsterState::DEAD)
			{
				SkeletonArcherScript* Archerscript = mOwner->GetComponent<SkeletonArcherScript>();
				Archerscript->mCurState = ss::eMonsterState::NEARATTACK;
				Archerscript->IsNearAttack(true);
			}
		}
	}
	void ArcherNearRangeScript::OnCollisionExit(Collider2D* other)
	{

	}
}