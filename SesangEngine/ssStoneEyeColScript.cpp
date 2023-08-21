#include "ssStoneEyeColScript.h"
#include "ssStoneEye.h"
#include "ssGameState.h"
#include "ssTime.h"



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
			mState = GameState::GetInst().GetState(L"Player");
			mState->SetCurrentHP(mState->GetCurrentHP() - 10);
		}
	}
	void StoneEyeColScript::OnCollisionStay(Collider2D* other)
	{
	}
	void StoneEyeColScript::OnCollisionExit(Collider2D* other)
	{
	}
}