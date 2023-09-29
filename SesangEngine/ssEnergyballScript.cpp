#include "ssEnergyballScript.h"
#include "ssEnergyball.h"
#include "ssGameState.h"
#include "ssPlayerScript.h"


namespace ss
{
	EnergyballScript::EnergyballScript()
		: mvStartPosition(Vector3::Zero)
	{



	}

	EnergyballScript::~EnergyballScript()
	{
	}

	void EnergyballScript::Initialize()
	{
	}

	void EnergyballScript::Update()
	{
	}

	void EnergyballScript::OnCollisionEnter(Collider2D* other)
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
	}

	void EnergyballScript::OnCollisionStay(Collider2D* other)
	{
	}

	void EnergyballScript::OnCollisionExit(Collider2D* other)
	{
	}


}


