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
	
		// ==================== ** 패링은 나중에 만들기 
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
		//				// 충돌이 발생했을 경우, 원래의 방향과 반대로 움직입니다.
		//				ArrowPos.x += -1.f * mSpeed * Time::DeltaTime();
		//			}

		//			else if (GetOwner()->GetName() == L"ArcherArrowObj_L")
		//			{
		//				// 충돌이 발생했을 경우, 원래의 방향과 반대로 움직입니다.
		//				ArrowPos.x += 1.f * mSpeed * Time::DeltaTime();
		//			}

		//			mTransform->SetPosition(ArrowPos);

		//		}


		//		else
		//		{
		//			if (GetOwner()->GetName() == L"ArcherArrowObj_R")
		//			{
		//				// 충돌이 발생했을 경우, 원래의 방향과 반대로 움직입니다.
		//				ArrowPos.x += 1.f * mSpeed * Time::DeltaTime();
		//			}

		//			else if (GetOwner()->GetName() == L"ArcherArrowObj_L")
		//			{
		//				// 충돌이 발생했을 경우, 원래의 방향과 반대로 움직입니다.
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

	void EnergyballScript::OnCollisionStay(Collider2D* other)
	{
	}

	void EnergyballScript::OnCollisionExit(Collider2D* other)
	{
	}


}


