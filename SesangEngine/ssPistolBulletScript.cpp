#include "ssPistolBulletScript.h"
#include "ssGameObject.h"
#include "ssCollider2D.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssTime.h"
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
#include "ssGoddnessScript.h"

namespace ss
{
	PistolBulletScript::PistolBulletScript()
		: mReverse(false)
		, mSpeed(350.f)
		, mIsGuard(false)
		, mAnimator(nullptr)
		, mTransform(nullptr)
		, mCollider(nullptr)
		, mState(nullptr)
		, mPlayer(nullptr)
		, mMonster(nullptr)
		, mMr(nullptr)
	{
	}
	PistolBulletScript::~PistolBulletScript()
	{
	}
	void PistolBulletScript::Initialize()
	{
		//(StoneEye*)mOriginOwner;

		mState = GetOwner()->GetComponent<CharacterState>();
		mAnimator = GetOwner()->GetComponent<Animator>();
		mTransform = GetOwner()->GetComponent<Transform>();
		mCollider = GetOwner()->GetComponent<Collider2D>();
		mMr = GetOwner()->GetComponent<MeshRenderer>();



		if (GetOwner()->GetName() == L"Pistolbullet_Big_ObjR"
			|| GetOwner()->GetName() == L"Pistolbullet_Big_ObjL")
		{
			mMr->SetMaterial(Resources::Find<Material>(L"Pistol_BulletBigMtrl"));
			mTransform->SetScale(Vector3(12.f, 5.f, 0.f));;

			// 충돌체 크기도 여기서 정해주기 

		}


		else if 
			(GetOwner()->GetName() == L"Pistolbullet_Small_ObjR"
				|| GetOwner()->GetName() == L"Pistolbullet_Small_ObjL")
		{
			mMr->SetMaterial(Resources::Find<Material>(L"Pistol_BulletSmallMtrl"));
			mTransform->SetScale(Vector3(9.f, 3.f, 0.f));;

		}





	}
	void PistolBulletScript::Update()
	{


		if (GetOwner()->GetName() == L"Pistolbullet_Big_ObjR")
		{
			Vector3 BulletPos = mTransform->GetPosition();

			BulletPos.x += 1.f * mSpeed * Time::DeltaTime();

			mTransform->SetPosition(BulletPos);
		}

		else if (GetOwner()->GetName() == L"Pistolbullet_Big_ObjL")
			{
				Vector3 BulletPos = mTransform->GetPosition();

				BulletPos.x += -1.f * mSpeed * Time::DeltaTime();

				mTransform->SetPosition(BulletPos);
			}

		else if
			(GetOwner()->GetName() == L"Pistolbullet_Small_ObjR")
		{
			Vector3 BulletPos = mTransform->GetPosition();

			BulletPos.x += 1.f * mSpeed * Time::DeltaTime();

			mTransform->SetPosition(BulletPos);
		}


		else if
			(GetOwner()->GetName() == L"Pistolbullet_Small_ObjL")
		{
			Vector3 BulletPos = mTransform->GetPosition();

			BulletPos.x += -1.f * mSpeed * Time::DeltaTime();

			mTransform->SetPosition(BulletPos);
		}



	}


		//StoneEyeScript* monScript = mOriginOwner->GetComponent<StoneEyeScript>();
		//CharacterState* StoneEyeState = mOriginOwner->GetComponent<CharacterState>();

		//if (StoneEyeState->GetCurrentHP() > 0.f)
		//{
		//	Vector3 PlayerDir = monScript->GetCurDir();
		//	Vector3 ArrowPos = mTransform->GetPosition();

		//	if (mIsGuard)
		//	{
		//		// 충돌이 발생했을 경우, 원래의 방향과 반대로 움직입니다.
		//		ArrowPos.x += (PlayerDir.x == 1.0f ? -1 : 1) * mSpeed * Time::DeltaTime();
		//	}
		//	else
		//	{
		//		// 정상적인 움직임
		//		ArrowPos.x += (PlayerDir.x == 1.0f ? 1 : -1) * mSpeed * Time::DeltaTime();
		//	}

		//	mTransform->SetPosition(ArrowPos);
		//}

	
	void PistolBulletScript::OnCollisionEnter(Collider2D* other)
	{


		if (L"StoneEye" == other->GetOwner()->GetName())
		{
			CharacterState* stone = other->GetOwner()->GetComponent<CharacterState>();
			StoneEyeScript* script = other->GetOwner()->GetComponent<StoneEyeScript>();



				if (stone->GetCurrentHP() > 0.f)
				{

					if (GetOwner()->GetName() == L"Pistolbullet_Small_ObjR"
						|| GetOwner()->GetName() == L"Pistolbullet_Small_ObjL")
					{
						stone->SetCurrentHP(stone->GetCurrentHP() - 10);
						GetOwner()->SetState(GameObject::eState::Dead);
					}

					else if (GetOwner()->GetName() == L"Pistolbullet_Big_ObjR"
						|| GetOwner()->GetName() == L"Pistolbullet_Big_ObjL")
					{
						stone->SetCurrentHP(stone->GetCurrentHP() - 20);
						GetOwner()->SetState(GameObject::eState::Dead);
					}

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

				if (GetOwner()->GetName() == L"Pistolbullet_Small_ObjR"
					|| GetOwner()->GetName() == L"Pistolbullet_Small_ObjL")
				{
					stone->SetCurrentHP(stone->GetCurrentHP() - 10);
					GetOwner()->SetState(GameObject::eState::Dead);
				}

				else if (GetOwner()->GetName() == L"Pistolbullet_Big_ObjR"
					|| GetOwner()->GetName() == L"Pistolbullet_Big_ObjL")
				{
					stone->SetCurrentHP(stone->GetCurrentHP() - 20);
					GetOwner()->SetState(GameObject::eState::Dead);
				}

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

				if (GetOwner()->GetName() == L"Pistolbullet_Small_ObjR"
					|| GetOwner()->GetName() == L"Pistolbullet_Small_ObjL")
				{
					stone->SetCurrentHP(stone->GetCurrentHP() - 10);
					GetOwner()->SetState(GameObject::eState::Dead);
				}

				else if (GetOwner()->GetName() == L"Pistolbullet_Big_ObjR"
					|| GetOwner()->GetName() == L"Pistolbullet_Big_ObjL")
				{
					stone->SetCurrentHP(stone->GetCurrentHP() - 20);
					GetOwner()->SetState(GameObject::eState::Dead);
				}

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

				if (GetOwner()->GetName() == L"Pistolbullet_Small_ObjR"
					|| GetOwner()->GetName() == L"Pistolbullet_Small_ObjL")
				{
					stone->SetCurrentHP(stone->GetCurrentHP() - 10);
					GetOwner()->SetState(GameObject::eState::Dead);
				}

				else if (GetOwner()->GetName() == L"Pistolbullet_Big_ObjR"
					|| GetOwner()->GetName() == L"Pistolbullet_Big_ObjL")
				{
					stone->SetCurrentHP(stone->GetCurrentHP() - 20);
					GetOwner()->SetState(GameObject::eState::Dead);
				}

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

				if (GetOwner()->GetName() == L"Pistolbullet_Small_ObjR"
					|| GetOwner()->GetName() == L"Pistolbullet_Small_ObjL")
				{
					stone->SetCurrentHP(stone->GetCurrentHP() - 10);
					GetOwner()->SetState(GameObject::eState::Dead);
				}

				else if (GetOwner()->GetName() == L"Pistolbullet_Big_ObjR"
					|| GetOwner()->GetName() == L"Pistolbullet_Big_ObjL")
				{
					stone->SetCurrentHP(stone->GetCurrentHP() - 20);
					GetOwner()->SetState(GameObject::eState::Dead);
				}

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

				if (GetOwner()->GetName() == L"Pistolbullet_Small_ObjR"
					|| GetOwner()->GetName() == L"Pistolbullet_Small_ObjL")
				{
					stone->SetCurrentHP(stone->GetCurrentHP() - 10);
					GetOwner()->SetState(GameObject::eState::Dead);
				}

				else if (GetOwner()->GetName() == L"Pistolbullet_Big_ObjR"
					|| GetOwner()->GetName() == L"Pistolbullet_Big_ObjL")
				{
					stone->SetCurrentHP(stone->GetCurrentHP() - 20);
					GetOwner()->SetState(GameObject::eState::Dead);
				}

				script->SetHit(true);
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

				if (GetOwner()->GetName() == L"Pistolbullet_Small_ObjR"
					|| GetOwner()->GetName() == L"Pistolbullet_Small_ObjL")
				{
					B_wolf->SetCurrentHP(B_wolf->GetCurrentHP() - 10);
					GetOwner()->SetState(GameObject::eState::Dead);
				}

				else if (GetOwner()->GetName() == L"Pistolbullet_Big_ObjR"
					|| GetOwner()->GetName() == L"Pistolbullet_Big_ObjL")
				{
					B_wolf->SetCurrentHP(B_wolf->GetCurrentHP() - 20);
					GetOwner()->SetState(GameObject::eState::Dead);
				}

				script->SetHit(true);
			}


			else if (B_wolf->GetCurrentHP() <= 0.f)
			{

				script->ChangeState(eWolfBossState::DEAD);
			}




		}



		else if (L"B_GodObj" == other->GetOwner()->GetName())
		{

			CharacterState* B_God = other->GetOwner()->GetComponent<CharacterState>();
			GoddnessScript* script = other->GetOwner()->GetComponent<GoddnessScript>();



			if (B_God->GetCurrentHP() > 0.f)
			{

				if (GetOwner()->GetName() == L"Pistolbullet_Small_ObjR"
					|| GetOwner()->GetName() == L"Pistolbullet_Small_ObjL")
				{
					B_God->SetCurrentHP(B_God->GetCurrentHP() - 10);
					GetOwner()->SetState(GameObject::eState::Dead);
				}

				else if (GetOwner()->GetName() == L"Pistolbullet_Big_ObjR"
					|| GetOwner()->GetName() == L"Pistolbullet_Big_ObjL")
				{
					B_God->SetCurrentHP(B_God->GetCurrentHP() - 10);
					GetOwner()->SetState(GameObject::eState::Dead);
				}

				script->SetHit(true);

			}


			else if (B_God->GetCurrentHP() <= 0.f)
			{

				if (script->GetBossType() == eBossType::STATUE)
				{
					script->ChangeState(eBoss2_Phase1::DIE);
				}

				else
				{
					script->ChangeState(eBoss2_Phase2::DIE);
				}
			}




		}


	}
	void PistolBulletScript::OnCollisionStay(Collider2D* other)
	{
	}
	void PistolBulletScript::OnCollisionExit(Collider2D* other)
	{
	}
}