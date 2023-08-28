#include "ssLizardColScript.h"
#include "ssMonster.h"
#include "ssGameState.h"
#include "ssTime.h"
#include "ssPlayerScript.h"
#include "ssAnimator.h"
#include "ssSkeletonLizardScript.h"



namespace ss
{
	LizardColScript::LizardColScript()
	{

	}


	LizardColScript::~LizardColScript()
	{
	}
	void LizardColScript::Initialize()
	{
		mMonster = (Monster*)GetOwner();

		mState = GetOwner()->GetComponent<CharacterState>();
		mAnimator = GetOwner()->GetComponent<Animator>();
		mTransform = GetOwner()->GetComponent<Transform>();
		mCollider = GetOwner()->GetComponent<Collider2D>();


		mTransform->SetScale(Vector3(1.f, 1.f, 1.f));

	}
	void LizardColScript::Update()
	{

	}


	void LizardColScript::OnCollisionEnter(Collider2D* other)
	{
		if (other->GetOwner()->GetName() == L"Player")
		{
			PlayerScript* script = other->GetOwner()->GetComponent<PlayerScript>();
			bool bDash = script->IsDash();

			if (!bDash)
			{
				mState = GameState::GetInst().GetState(L"Player");
				mState->SetCurrentHP(mState->GetCurrentHP() - 10);

				script->ChangeState(ePlayerState::HIT);
			}
		}

		if (other->GetName() == L"PlayerGuardCol")
		{

			// ��������� ���¸� stun���� ����� (������� ���� �浹ü�� ���� �浹ü�� �� ���̳���
			// ���� ���� ���� �浹ü�� �� �ø��ų� �ؾߵɵ� : ���߿� 
			SkeletonLizardScript* script = GetOwner()->GetComponent<SkeletonLizardScript>();
			script->ChangeState(eMonsterState::STUN);

		}
	}
	void LizardColScript::OnCollisionStay(Collider2D* other)
	{

	}
	void LizardColScript::OnCollisionExit(Collider2D* other)
	{
		if (other->GetOwner()->GetName() == L"Player")
		{

		}
	}
}