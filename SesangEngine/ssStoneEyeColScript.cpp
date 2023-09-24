#include "ssStoneEyeColScript.h"
#include "ssStoneEye.h"
#include "ssGameState.h"
#include "ssTime.h"
#include "ssPlayerScript.h"
#include "ssAnimator.h"
#include "ssStoneEyeScript.h"



namespace ss
{
	StoneEyeColScript::StoneEyeColScript()
	{
		
	}


	StoneEyeColScript::~StoneEyeColScript()
	{
	}
	void StoneEyeColScript::Initialize()
	{
		mMonster = (StoneEye*)GetOwner();

		mState = GetOwner()->GetComponent<CharacterState>();
		mAnimator = GetOwner()->GetComponent<Animator>();
		mTransform = GetOwner()->GetComponent<Transform>();
		mCollider = GetOwner()->GetComponent<Collider2D>();


		mTransform->SetScale(Vector3(1.f, 1.f, 1.f));

	}
	void StoneEyeColScript::Update()
	{
		
	}


	void StoneEyeColScript::OnCollisionEnter(Collider2D* other)
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

		else if (other->GetName() == L"PlayerGuardCol")
		{

			// ��������� ���¸� stun���� ����� (������� ���� �浹ü�� ���� �浹ü�� �� ���̳���
			// ���� ���� ���� �浹ü�� �� �ø��ų� �ؾߵɵ� : ���߿� 
			StoneEyeScript* script = GetOwner()->GetComponent<StoneEyeScript>();
			script->ChangeState(eMonsterState::STUN);

		}
	}
	void StoneEyeColScript::OnCollisionStay(Collider2D* other)
	{
	
	}
	void StoneEyeColScript::OnCollisionExit(Collider2D* other)
	{
		if (other->GetOwner()->GetName() == L"Player")
		{
			//Animator* ani = other->GetOwner()->GetComponent<Animator>();
			//PlayerScript* script = other->GetOwner()->GetComponent<PlayerScript>();

			//// �ִϸ��̼� ����� ������ 
			//if (ani->GetCurActiveAnimation()->IsComplete())
			//{
			//	script->ChangeState(ePlayerState::IDLE);
			//}
		}
	}
}