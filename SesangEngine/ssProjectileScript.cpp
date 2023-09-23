#include "ssProjectileScript.h"
#include "ssState.h"
#include "ssGameState.h"
#include "ssGameObject.h"
#include "ssTime.h"
#include "ssStoneEye.h"
#include "ssStoneEyeScript.h"
#include "ssPlayerScript.h"


namespace ss
{
	ProjectileScript::ProjectileScript()
		: mReverse(false)
		, mSpeed(80.f)
		, mIsGuard(false)
	{
	}
	ProjectileScript::~ProjectileScript()
	{
		
	}
	void ProjectileScript::Initialize()
	{
		(StoneEye*)mOriginOwner;

		mState = GetOwner()->GetComponent<CharacterState>();
		mAnimator = GetOwner()->GetComponent<Animator>();
		mTransform = GetOwner()->GetComponent<Transform>();
		mCollider = GetOwner()->GetComponent<Collider2D>();


	}
	void ProjectileScript::Update()
	{
	


		StoneEyeScript* monScript = mOriginOwner->GetComponent<StoneEyeScript>();
		CharacterState* StoneEyeState = mOriginOwner->GetComponent<CharacterState>();

		if (nullptr != StoneEyeState)
		{
			if (StoneEyeState->GetCurrentHP() > 0.f)
			{
				Vector3 PlayerDir = monScript->GetCurDir();
				Vector3 ArrowPos = mTransform->GetPosition();

				if (mIsGuard)
				{

					if (GetOwner()->GetName() == L"StoneEyeFarObjR")
					{
						// 충돌이 발생했을 경우, 원래의 방향과 반대로 움직입니다.
						ArrowPos.x += -1.f * mSpeed * Time::DeltaTime();
					}

					else if (GetOwner()->GetName() == L"StoneEyeFarObjL")
					{
						// 충돌이 발생했을 경우, 원래의 방향과 반대로 움직입니다.
						ArrowPos.x += 1.f * mSpeed * Time::DeltaTime();
					}

					mTransform->SetPosition(ArrowPos);

				}


				else
				{
					if (GetOwner()->GetName() == L"StoneEyeFarObjR")
					{
						// 충돌이 발생했을 경우, 원래의 방향과 반대로 움직입니다.
						ArrowPos.x += 1.f * mSpeed * Time::DeltaTime();
					}

					else if (GetOwner()->GetName() == L"StoneEyeFarObjL")
					{
						// 충돌이 발생했을 경우, 원래의 방향과 반대로 움직입니다.
						ArrowPos.x += -1.f * mSpeed * Time::DeltaTime();
					}

					mTransform->SetPosition(ArrowPos);

				}


			}
		}



	}
	void ProjectileScript::OnCollisionEnter(Collider2D* other)
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


		if (other->GetName() == L"PlayerGuardCol")
		{
			// 총알 반대로 가게 함 
			mIsGuard = true;


		}

		if (other->GetName() == L"StoneHitCol")
		{
			CharacterState* StoneEyeState = mOriginOwner->GetComponent<CharacterState>();
			StoneEyeScript* monScript = mOriginOwner->GetComponent<StoneEyeScript>();

			if (StoneEyeState->GetCurrentHP() > 0.f)
			{
				float test = StoneEyeState->GetCurrentHP();

				StoneEyeState->SetCurrentHP(StoneEyeState->GetCurrentHP() - 10);

				// 스톤아이의 상태를 stun으로 만들기
				StoneEyeScript* script = mOriginOwner->GetComponent<StoneEyeScript>();
				script->ChangeState(eMonsterState::STUN);

				// 총알이 스톤아이에게 부딪치면 삭제
				GetOwner()->SetState(ss::GameObject::eState::Dead);
			}


			else if (StoneEyeState->GetCurrentHP() <= 0.f)
			{
				monScript->ChangeState(eMonsterState::DEAD);
			}


		}
	}
	void ProjectileScript::OnCollisionStay(Collider2D* other)
	{
		// 공격용 충돌체를 여기서 없애기 
	}
	void ProjectileScript::OnCollisionExit(Collider2D* other)
	{
		int a = 0;
	}
}