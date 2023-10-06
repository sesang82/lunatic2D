#include "ssBigEnergyballScript.h"
#include "ssEnergyball.h"
#include "ssGameState.h"
#include "ssPlayerScript.h"

namespace ss
{
	BigEnergyballScript::BigEnergyballScript()
	{
	}
	BigEnergyballScript::~BigEnergyballScript()
	{
	}
	void BigEnergyballScript::Initialize()
	{
	}
	void BigEnergyballScript::Update()
	{
	}
	void BigEnergyballScript::OnCollisionEnter(Collider2D* other)
	{
		if (other->GetName() == L"colHit_player")
		{
			PlayerScript* script = other->GetOwner()->GetComponent<PlayerScript>();
			bool bDash = script->IsDash();

			// 대쉬 중엔 공격 무력화 
			if (!bDash)
			{
				mState = GameState::GetInst().GetState(L"Player");
				mState->SetCurrentHP(mState->GetCurrentHP() - 10);

				// 공격 당했을 시 HIT 상태로 변경 
				script->ChangeState(ePlayerState::HIT);


				// 총알이 플레이어에게 부딪치면 삭제
				GetOwner()->SetState(ss::GameObject::eState::Dead);
			}
		}

		else if (other->GetName() == L"PlayerGuardCol")
		{
			// 총알 반대로 가게 함 
			mIsGuard = true;


		}
	}
	void BigEnergyballScript::OnCollisionStay(Collider2D* other)
	{
	}
	void BigEnergyballScript::OnCollisionExit(Collider2D* other)
	{
	}
}