#include "ssPlayerAttackColScript.h"
#include "ssGameObject.h"
#include "ssPlayerScript.h"
#include "ssPlayer.h"
#include "ssScene.h"
#include "ssSceneManager.h"
#include "ssGameState.h"
#include "ssStoneEye.h"
#include "ssStoneEyeScript.h"
#include "ssSkeletonArcherScript.h"
#include "ssSkeletonLizardScript.h"
#include "ssBigWolfScript.h"
#include "ssWoodGolemScript.h"
#include "ssWolfScript.h"


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

		else if (L"Archer" == other->GetOwner()->GetName())
		{
			CharacterState* stone = other->GetOwner()->GetComponent<CharacterState>();
			SkeletonArcherScript* script = other->GetOwner()->GetComponent<SkeletonArcherScript>();

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
	
		else if (L"WoodGolemObj" == other->GetOwner()->GetName())
		{
			CharacterState* stone = other->GetOwner()->GetComponent<CharacterState>();
			WoodGolemScript* script = other->GetOwner()->GetComponent<WoodGolemScript>();

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

		else if (L"WolfObj" == other->GetOwner()->GetName())
		{
			CharacterState* stone = other->GetOwner()->GetComponent<CharacterState>();
			WolfScript* script = other->GetOwner()->GetComponent<WolfScript>();

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


		else if (L"Lizard" == other->GetOwner()->GetName())
		{
			CharacterState* stone = other->GetOwner()->GetComponent<CharacterState>();
			SkeletonLizardScript* script = other->GetOwner()->GetComponent<SkeletonLizardScript>();

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

		else if (L"ZombiObj" == other->GetOwner()->GetName())
		{
			CharacterState* stone = other->GetOwner()->GetComponent<CharacterState>();
			SkeletonLizardScript* script = other->GetOwner()->GetComponent<SkeletonLizardScript>();

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


		else if (L"B_WolfObj" == other->GetOwner()->GetName())
		{
			
			CharacterState* B_wolf = other->GetOwner()->GetComponent<CharacterState>();
			BigWolfScript* script = other->GetOwner()->GetComponent<BigWolfScript>();

			if (B_wolf->GetCurrentHP() > 0.f)
			{

				B_wolf->SetCurrentHP(B_wolf->GetCurrentHP() - 10);
				script->SetHit(true);
				//script->BindConstantBuffer();

			}

			else if (B_wolf->GetCurrentHP() <= 0.f)
			{
				script->ChangeState(eWolfBossState::DEAD);
			}


		}


	}
	void PlayerAttackColScript::OnCollisionStay(Collider2D* other)
	{

		PlayerScript* playerScript = mOwnerObj->GetComponent<PlayerScript>();


		if (playerScript->GetState() == ePlayerState::OVERLOAD_START && playerScript->GetWeapon() == eWeaponType::PISTOL)
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

			else if (L"Archer" == other->GetOwner()->GetName())
			{
				CharacterState* stone = other->GetOwner()->GetComponent<CharacterState>();
				SkeletonArcherScript* script = other->GetOwner()->GetComponent<SkeletonArcherScript>();

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

			else if (L"WoodGolemObj" == other->GetOwner()->GetName())
			{
				CharacterState* stone = other->GetOwner()->GetComponent<CharacterState>();
				WoodGolemScript* script = other->GetOwner()->GetComponent<WoodGolemScript>();

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

			else if (L"WolfObj" == other->GetOwner()->GetName())
			{
				CharacterState* stone = other->GetOwner()->GetComponent<CharacterState>();
				WolfScript* script = other->GetOwner()->GetComponent<WolfScript>();

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


			else if (L"Lizard" == other->GetOwner()->GetName())
			{
				CharacterState* stone = other->GetOwner()->GetComponent<CharacterState>();
				SkeletonLizardScript* script = other->GetOwner()->GetComponent<SkeletonLizardScript>();

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

			else if (L"ZombiObj" == other->GetOwner()->GetName())
			{
				CharacterState* stone = other->GetOwner()->GetComponent<CharacterState>();
				SkeletonLizardScript* script = other->GetOwner()->GetComponent<SkeletonLizardScript>();

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


			else if (L"B_WolfObj" == other->GetOwner()->GetName())
			{

				CharacterState* B_wolf = other->GetOwner()->GetComponent<CharacterState>();
				BigWolfScript* script = other->GetOwner()->GetComponent<BigWolfScript>();

				if (B_wolf->GetCurrentHP() > 0.f)
				{

					B_wolf->SetCurrentHP(B_wolf->GetCurrentHP() - 10);
					script->SetHit(true);
					//script->BindConstantBuffer();

				}

				else if (B_wolf->GetCurrentHP() <= 0.f)
				{
					script->ChangeState(eWolfBossState::DEAD);
				}


			}

		}

	}
	void PlayerAttackColScript::OnCollisionExit(Collider2D* other)
	{
		
		





	}
}