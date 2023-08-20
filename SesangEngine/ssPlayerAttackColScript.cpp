#include "ssPlayerAttackColScript.h"
#include "ssGameObject.h"
#include "ssPlayerScript.h"
#include "ssPlayer.h"
#include "ssScene.h"
#include "ssSceneManager.h"
#include "ssGameState.h"
#include "ssStoneEye.h"

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
		mStoneEye = scene->FindObjInActiveLayer<StoneEye>();



		mTransform->SetScale(Vector3(1.f, 1.f, 1.f)); // 어차피 충돌체 쓸거라 최소만 해두면 됨 


	}
	void PlayerAttackColScript::Update()
	{
	

		Script::Update();
	}

	void PlayerAttackColScript::OnCollisionEnter(Collider2D* other)
	{


		if (L"StoneEye" == other->GetOwner()->GetName())
		{
			
			CharacterState* stone = other->GetOwner()->GetComponent<CharacterState>();
			stone->SetCurrentHP(stone->GetCurrentHP() - 10);


			 // === game state 테스트용 (작동 됨) 
			mState = GameState::GetInst().GetState(L"Player");
			mState->SetCurrentSP(mState->GetCurrentSP() - 10);
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