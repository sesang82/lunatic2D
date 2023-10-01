#include "ssArcherArrowScript.h"
#include "ssState.h"
#include "ssGameState.h"
#include "ssGameObject.h"
#include "ssTime.h"
#include "ssStoneEye.h"
#include "ssStoneEyeScript.h"
#include "ssPlayerScript.h"
#include "ssSkeletonArcherScript.h"


namespace ss
{
	ArcherArrowScript::ArcherArrowScript()
		: mReverse(false)
		, mSpeed(80.f)
		, mIsGuard(false)
	{
	}
	ArcherArrowScript::~ArcherArrowScript()
	{

	}
	void ArcherArrowScript::Initialize()
	{
		(Monster*)mOriginOwner;

		mState = GetOwner()->GetComponent<CharacterState>();
		mAnimator = GetOwner()->GetComponent<Animator>();
		mTransform = GetOwner()->GetComponent<Transform>();
		mCollider = GetOwner()->GetComponent<Collider2D>();


	}
	void ArcherArrowScript::Update()
	{



		SkeletonArcherScript* monScript = mOriginOwner->GetComponent<SkeletonArcherScript>();
		CharacterState* StoneEyeState = mOriginOwner->GetComponent<CharacterState>();


		if(nullptr != StoneEyeState)
		{ 
			if (StoneEyeState->GetCurrentHP() > 0.f)
			{
				Vector3 PlayerDir = monScript->GetCurDir();
				Vector3 ArrowPos = mTransform->GetPosition();


				if (mIsGuard)
				{

					if (GetOwner()->GetName() == L"ArcherArrowObj_R")
					{
						// �浹�� �߻����� ���, ������ ����� �ݴ�� �����Դϴ�.
						ArrowPos.x += -1.f * mSpeed * Time::DeltaTime();
					}

					else if (GetOwner()->GetName() == L"ArcherArrowObj_L")
					{
						// �浹�� �߻����� ���, ������ ����� �ݴ�� �����Դϴ�.
						ArrowPos.x += 1.f * mSpeed * Time::DeltaTime();
					}

					mTransform->SetPosition(ArrowPos);

				}


				else
				{
					if (GetOwner()->GetName() == L"ArcherArrowObj_R")
					{
						// �浹�� �߻����� ���, ������ ����� �ݴ�� �����Դϴ�.
						ArrowPos.x += 1.f * mSpeed * Time::DeltaTime();
					}

					else if (GetOwner()->GetName() == L"ArcherArrowObj_L")
					{
						// �浹�� �߻����� ���, ������ ����� �ݴ�� �����Դϴ�.
						ArrowPos.x += -1.f * mSpeed * Time::DeltaTime();
					}

					mTransform->SetPosition(ArrowPos);

				}
			}
		}
		



	}
	void ArcherArrowScript::OnCollisionEnter(Collider2D* other)
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

				// ���� ������ �� �÷��̾ HIT ���·� ���� 
				script->ChangeState(ePlayerState::HIT);


				// �Ѿ��� �÷��̾�� �ε�ġ�� ����
				GetOwner()->SetState(ss::GameObject::eState::Dead);
			}
		}


		if (other->GetName() == L"PlayerGuardCol")
		{
			// �Ѿ� �ݴ�� ���� �� 
			mIsGuard = true;


		}

		if (other->GetName() == L"Archer_HitCol")
		{
			CharacterState* ArcherState = mOriginOwner->GetComponent<CharacterState>();
			SkeletonArcherScript* monScript = mOriginOwner->GetComponent<SkeletonArcherScript>();

			if (ArcherState->GetCurrentHP() > 0.f)
			{
				float test = ArcherState->GetCurrentHP();

				ArcherState->SetCurrentHP(ArcherState->GetCurrentHP() - 10);

				// ��������� ���¸� stun���� �����
				SkeletonArcherScript* script = mOriginOwner->GetComponent<SkeletonArcherScript>();
				script->ChangeState(eMonsterState::STUN);

				// �Ѿ��� ������̿��� �ε�ġ�� ����
				GetOwner()->SetState(ss::GameObject::eState::Dead);
			}


			else if (ArcherState->GetCurrentHP() <= 0.f)
			{
				monScript->ChangeState(eMonsterState::DEAD);
			}


		}
	}
	void ArcherArrowScript::OnCollisionStay(Collider2D* other)
	{
		// ���ݿ� �浹ü�� ���⼭ ���ֱ� 
	}
	void ArcherArrowScript::OnCollisionExit(Collider2D* other)
	{
		int a = 0;
	}
}
