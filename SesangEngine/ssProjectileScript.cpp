#include "ssProjectileScript.h"
#include "ssState.h"
#include "ssGameState.h"
#include "ssGameObject.h"
#include "ssTime.h"
#include "ssStoneEye.h"
#include "ssStoneEyeScript.h"


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

		if (StoneEyeState->GetCurrentHP() > 0.f)
		{
		Vector3 PlayerDir = monScript->GetCurDir();
		Vector3 ArrowPos = mTransform->GetPosition();

			if (mIsGuard)
			{
				// �浹�� �߻����� ���, ������ ����� �ݴ�� �����Դϴ�.
				ArrowPos.x += (PlayerDir.x == 1.0f ? -1 : 1) * mSpeed * Time::DeltaTime();
			}
			else
			{
				// �������� ������
				ArrowPos.x += (PlayerDir.x == 1.0f ? 1 : -1) * mSpeed * Time::DeltaTime();
			}

			mTransform->SetPosition(ArrowPos);
		}



	}
	void ProjectileScript::OnCollisionEnter(Collider2D* other)
	{
		if (other->GetName() == L"colHit_player")
		{
			mState = GameState::GetInst().GetState(L"Player");
			mState->SetCurrentHP(mState->GetCurrentHP() - 10);
		
			// �Ѿ��� �÷��̾�� �ε�ġ�� ����
			GetOwner()->SetState(ss::GameObject::eState::Dead);
		}


		if (other->GetName() == L"PlayerGuardCol")
		{
			// �Ѿ� �ݴ�� ���� �� 
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

				// ��������� ���¸� stun���� �����
				StoneEyeScript* script = mOriginOwner->GetComponent<StoneEyeScript>();
				script->ChangeState(eMonsterState::STUN);


				float test2 = StoneEyeState->GetCurrentHP();

				// �Ѿ��� ������̿��� �ε�ġ�� ����
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
		// ���ݿ� �浹ü�� ���⼭ ���ֱ� 
	}
	void ProjectileScript::OnCollisionExit(Collider2D* other)
	{
	}
}