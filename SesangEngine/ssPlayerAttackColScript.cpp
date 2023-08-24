#include "ssPlayerAttackColScript.h"
#include "ssGameObject.h"
#include "ssPlayerScript.h"
#include "ssPlayer.h"
#include "ssScene.h"
#include "ssSceneManager.h"
#include "ssGameState.h"
#include "ssStoneEye.h"
#include "ssStoneEyeScript.h"

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
			StoneEyeScript* script = other->GetOwner()->GetComponent<StoneEyeScript>();

			if (stone->GetCurrentHP() > 0.f)
			{

				stone->SetCurrentHP(stone->GetCurrentHP() - 10);

				
				script->ChangeState(eMonsterState::HIT);
			}

			else if (stone->GetCurrentHP() <= 0.f)
			{
				script->ChangeState(eMonsterState::DEAD);
			}

		
		}






	}
	void PlayerAttackColScript::OnCollisionStay(Collider2D* other)
	{
	
	}
	void PlayerAttackColScript::OnCollisionExit(Collider2D* other)
	{
		if(L"StoneEye" == other->GetOwner()->GetName())
		{


			StoneEyeScript* script = other->GetOwner()->GetComponent<StoneEyeScript>();
			script->ChangeState(eMonsterState::HIT);

		}
	}
}