#include "ssBigEnergyballScript.h"
#include "ssEnergyball.h"
#include "ssGameState.h"
#include "ssPlayerScript.h"
#include "ssAnimator.h"
#include "ssBigEnergyball.h"

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

			// �뽬 �߿� ���� ����ȭ 
			if (!bDash)
			{
				mState = GameState::GetInst().GetState(L"Player");
				mState->SetCurrentHP(mState->GetCurrentHP() - 10);

				// ���� ������ �� HIT ���·� ���� 
				script->ChangeState(ePlayerState::HIT);


				// �Ѿ��� �÷��̾�� �ε�ġ�� ����
				GetOwner()->SetState(ss::GameObject::eState::Dead);
			}
		}


		else if (other->GetOwner()->GetName() == L"col_Floor")
		{
			BigEnergyball* sword = (BigEnergyball*)GetOwner();
			
			sword->SetHit(true);

		


		}


	}
	void BigEnergyballScript::OnCollisionStay(Collider2D* other)
	{
		
	
	}
	void BigEnergyballScript::OnCollisionExit(Collider2D* other)
	{
	}
}