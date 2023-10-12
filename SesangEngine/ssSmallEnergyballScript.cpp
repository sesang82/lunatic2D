#include "ssSmallEnergyballScript.h"
#include "ssEnergyball.h"
#include "ssGameState.h"
#include "ssPlayerScript.h"
#include "ssObject.h"
#include "ssTime.h"
#include "ssSmallEnergyball.h"
#include "ssAnimator.h"


namespace ss
{
	SmallEnergyballScript::SmallEnergyballScript()
		: mAnimator(nullptr)
		, mTransform(nullptr)
		, mfTime(0.0f)
	{


	}

	SmallEnergyballScript::~SmallEnergyballScript()
	{
	}

	void SmallEnergyballScript::Initialize()
	{
		
	}

	void SmallEnergyballScript::Update()
	{
	

	}

	void SmallEnergyballScript::OnCollisionEnter(Collider2D* other)
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

		else if (other->GetName() == L"PlayerGuardCol")
		{
			// �Ѿ� �ݴ�� ���� �� 
			mIsGuard = true;


		}

		else if (other->GetOwner()->GetName() == L"col_Floor")
		{

			GetOwner()->GetComponent<Animator>()->PlayAnimation(L"Energyball_S_Parrying_End", false);

		}
	}

	void SmallEnergyballScript::OnCollisionStay(Collider2D* other)
	{
	}

	void SmallEnergyballScript::OnCollisionExit(Collider2D* other)
	{
	}




}


