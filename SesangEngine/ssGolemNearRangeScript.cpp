#include "ssGolemNearRangeScript.h"
#include "ssGameObject.h"
#include "ssWoodGolemScript.h"

namespace ss
{
	GolemNearRangeScript::GolemNearRangeScript()
	{
	}
	GolemNearRangeScript::~GolemNearRangeScript()
	{
	}
	void GolemNearRangeScript::Initialize()
	{
	}
	void GolemNearRangeScript::Update()
	{
	}
	void GolemNearRangeScript::OnCollisionEnter(Collider2D* other)
	{
		if (other->GetOwner()->GetName() == L"Player")
		{

			WoodGolemScript* WoodScript = mOwner->GetComponent<WoodGolemScript>();

			// stun������ �� �������� �ʾƾ��ϹǷ� 
			if (WoodScript->mCurState != ss::eMonsterState::STUN)
			{
				WoodScript->mCurState = ss::eMonsterState::NEARATTACK;

				WoodScript->IsNearAttack(true);
			}

		}
	}
	void GolemNearRangeScript::OnCollisionStay(Collider2D* other)
	{
	}
	void GolemNearRangeScript::OnCollisionExit(Collider2D* other)
	{
	}
}
