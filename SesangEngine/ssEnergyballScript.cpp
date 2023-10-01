#include "ssEnergyballScript.h"
#include "ssEnergyball.h"
#include "ssGameState.h"
#include "ssPlayerScript.h"


namespace ss
{
	EnergyballScript::EnergyballScript()
		//: mvStartPosition(Vector3::Zero)
		: mIsGuard(false)

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
	
		// ==================== ** �и��� ���߿� ����� 
		//CharacterState* StoneEyeState = mOriginOwner->GetComponent<CharacterState>();

		//if (nullptr != StoneEyeState)
		//{
		//	if (StoneEyeState->GetCurrentHP() > 0.f)
		//	{
		//		Vector3 ArrowPos = mTransform->GetPosition();


		//		if (mIsGuard)
		//		{

		//			if (GetOwner()->GetName() == L"ArcherArrowObj_R")
		//			{
		//				// �浹�� �߻����� ���, ������ ����� �ݴ�� �����Դϴ�.
		//				ArrowPos.x += -1.f * mSpeed * Time::DeltaTime();
		//			}

		//			else if (GetOwner()->GetName() == L"ArcherArrowObj_L")
		//			{
		//				// �浹�� �߻����� ���, ������ ����� �ݴ�� �����Դϴ�.
		//				ArrowPos.x += 1.f * mSpeed * Time::DeltaTime();
		//			}

		//			mTransform->SetPosition(ArrowPos);

		//		}


		//		else
		//		{
		//			if (GetOwner()->GetName() == L"ArcherArrowObj_R")
		//			{
		//				// �浹�� �߻����� ���, ������ ����� �ݴ�� �����Դϴ�.
		//				ArrowPos.x += 1.f * mSpeed * Time::DeltaTime();
		//			}

		//			else if (GetOwner()->GetName() == L"ArcherArrowObj_L")
		//			{
		//				// �浹�� �߻����� ���, ������ ����� �ݴ�� �����Դϴ�.
		//				ArrowPos.x += -1.f * mSpeed * Time::DeltaTime();
		//			}

		//			mTransform->SetPosition(ArrowPos);

		//		}
		//	}
		//}

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

		else if (other->GetName() == L"PlayerGuardCol")
		{
			// �Ѿ� �ݴ�� ���� �� 
			mIsGuard = true;


		}
	}

	void EnergyballScript::OnCollisionStay(Collider2D* other)
	{
	}

	void EnergyballScript::OnCollisionExit(Collider2D* other)
	{
	}


}


