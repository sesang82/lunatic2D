#include "ssPlayerAttackColScript.h"
#include "ssGameObject.h"
#include "ssPlayerScript.h"
#include "ssPlayer.h"
#include "ssScene.h"
#include "ssSceneManager.h"
#include "ssGameState.h"

namespace ss
{
	PlayerAttackColScript::PlayerAttackColScript()
	{
	}
	PlayerAttackColScript::~PlayerAttackColScript()
	{
	}
	void PlayerAttackColScript::Initialize()
	{
		mState = GetOwner()->GetComponent<CharacterState>();
		mAnimator = GetOwner()->GetComponent<Animator>();
		mTransform = GetOwner()->GetComponent<Transform>();
		mCollider = GetOwner()->GetComponent<Collider2D>();

		Scene* scene = SceneManager::GetActiveScene();
		mPlayer = scene->FindGameObjectByName(L"Player");


		mTransform->SetScale(Vector3(10.f, 10.f, 1.f));


	}
	void PlayerAttackColScript::Update()
	{
		Vector2 test = mCollider->GetSize();

		Script::Update();
	}

	void PlayerAttackColScript::OnCollisionEnter(Collider2D* other)
	{


		Vector2 test = mCollider->GetSize();

		if (L"StoneEye" == other->GetOwner()->GetName())
		{
			// 프로그래스바 시험용 (작동됨) 
			mState = GameState::GetInst().GetState(L"Player");
			mState->SetCurrentHP(mState->GetCurrentHP() - 10);
			mState->SetTexType(eTextureType::PLYAERHP);

		}






	}
	void PlayerAttackColScript::OnCollisionStay(Collider2D* other)
	{
		//Vector2 test = mCollider->GetSize();

		//if (L"StoneEye" == other->GetOwner()->GetName())
		//{
		//	// 프로그래스바 시험용 (작동됨) 
		//	mState = GameState::GetInst().GetState(L"Player");
		//	mState->SetCurrentHP(mState->GetCurrentHP() - 10);

		//}
	}
	void PlayerAttackColScript::OnCollisionExit(Collider2D* other)
	{
	}
}