#include "ssArcherColScript.h"
#include "ssStoneEye.h"
#include "ssGameState.h"
#include "ssTime.h"
#include "ssPlayerScript.h"
#include "ssAnimator.h"
#include "ssStoneEyeScript.h"



namespace ss
{
	ArcherColScript::ArcherColScript()
	{

	}


	ArcherColScript::~ArcherColScript()
	{
	}
	void ArcherColScript::Initialize()
	{
		mMonster = (StoneEye*)GetOwner();

		mState = GetOwner()->GetComponent<CharacterState>();
		mAnimator = GetOwner()->GetComponent<Animator>();
		mTransform = GetOwner()->GetComponent<Transform>();
		mCollider = GetOwner()->GetComponent<Collider2D>();


		mTransform->SetScale(Vector3(1.f, 1.f, 1.f));

	}
	void ArcherColScript::Update()
	{

	}


	void ArcherColScript::OnCollisionEnter(Collider2D* other)
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

			// 스톤아이의 상태를 stun으로 만들기 (스톤아이 근접 충돌체랑 가드 충돌체랑 넘 차이나서
			// 근접 공격 범위 충돌체를 좀 늘리거나 해야될듯 : 나중에 
			StoneEyeScript* script = GetOwner()->GetComponent<StoneEyeScript>();
			script->ChangeState(eMonsterState::STUN);

		}
	}
	void ArcherColScript::OnCollisionStay(Collider2D* other)
	{

	}
	void ArcherColScript::OnCollisionExit(Collider2D* other)
	{
		if (other->GetOwner()->GetName() == L"Player")
		{
		
		}
	}
}