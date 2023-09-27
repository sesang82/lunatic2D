#include "ssHitGroundScript.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssGameObject.h"
#include "ssAnimator.h"
#include "ssMonster.h"
#include "ssSceneManager.h"
#include "ssWoodGolemScript.h"
#include "ssWolfScript.h"
#include "ssZombieScript.h"
#include "ssBigWolfScript.h"
#include "ssPlayerScript.h"
#include "ssGameState.h"
#include "ssCharacterState.h"

namespace ss
{
	HitGroundScript::HitGroundScript()
		: mCol(nullptr)
	{
	}
	HitGroundScript::~HitGroundScript()
	{
	}
	void HitGroundScript::Initialize()
	{
		MeshRenderer* mr = GetOwner()->GetComponent<MeshRenderer>();
		Transform* tr = GetOwner()->GetComponent<Transform>();



		// 일단 이렇게 한 이유는 hit ground의 크기가 다를 수도 있어서 일단 이렇게 해놨는데 나중에 그럴 필요없으면 리팩토링 하기
		if (GetOwner()->GetName() == L"WoodHitGroundObj"
			|| GetOwner()->GetName() == L"WolfHitGroundObj"
			|| GetOwner()->GetName() == L"ZombieHitGroundObj")
		{
			mr->SetMaterial(Resources::Find<Material>(L"hitGroundMtrl"));			
			tr->SetScale(Vector3(34.f, 39.f, 0.f));

		

		}

		else if (GetOwner()->GetName() == L"StomingHitGroundObj")
		{
			mr->SetMaterial(Resources::Find<Material>(L"Boss_Wolf_LandingHitMtrl"));
			tr->SetScale(Vector3(208.f, 702.f, 0.f));
	
		}

		
		else if (GetOwner()->GetName() == L"StomingHitGroundObj2")
		{
			mr->SetMaterial(Resources::Find<Material>(L"Boss_Wolf_HitFullMtrl"));
			tr->SetScale(Vector3(1600.f, 896.f, 0.f));

		}

		else if (GetOwner()->GetName() == L"HowlingHitGroundObj")
		{
			mr->SetMaterial(Resources::Find<Material>(L"Boss_Wolf_PoistionHitMtrl"));
			tr->SetScale(Vector3(583.f, 123.f, 0.f));

		}


		else if (GetOwner()->GetName() == L"StompHitGroundObj") // 이거 이미지 만들어야함 
		{
			mr->SetMaterial(Resources::Find<Material>(L"stompHitGroundt_Mtrl"));
			tr->SetScale(Vector3(141.f, 32.f, 0.f));

		}


	}
	void HitGroundScript::Update()
	{
		// 사용하는 몬스터의 애니메이션 정보에 접근하여 인덱스를 얻어낸다. 

		Animator* anim = mMonster->GetComponent<Animator>();

		//if (SceneManager::GetActiveScene()->GetName() == L"Stage1Scene")
		
		
	  if(GetOwner()->GetName() == L"WoodHitGroundObj")
		{
			WoodGolemScript* script = mMonster->GetComponent<WoodGolemScript>();
			bool paunched = script->IsPaunched();
			
			if (paunched && anim->GetCurActiveAnimation()->GetIndex() == 6)
			{
				mCol = GetOwner()->AddComponent<Collider2D>();
				mCol->SetSize(Vector2(1.f, 0.5f));
				mCol->SetCenter(Vector2(0.f, -12.f));
			}

			else if (paunched && anim->GetCurActiveAnimation()->GetIndex() == 7)
			{
				script->SetPaunched(false);
				GetOwner()->SetState(GameObject::eState::Dead);
			}

		}

		else if (GetOwner()->GetName() == L"WolfHitGroundObj")
		{
			WolfScript* script = mMonster->GetComponent<WolfScript>();
			bool jumped = script->IsJumped();


			if (jumped && anim->GetCurActiveAnimation()->GetIndex() == 13)
			{
				script->SetJumped(false);
				GetOwner()->SetState(GameObject::eState::Dead);
			}

		}

		// ZombieHitGroundObj

		else if (GetOwner()->GetName() == L"ZombieHitGroundObj")
		{
			ZombieScript* script = mMonster->GetComponent<ZombieScript>();
			bool jumped = script->IsJumped();


			if (jumped && anim->GetCurActiveAnimation()->GetIndex() == 12)
			{
				script->SetJumped(false);
				GetOwner()->SetState(GameObject::eState::Dead);
			}

		}

		else if (GetOwner()->GetName() == L"StomingHitGroundObj")
		{
			BigWolfScript* script = mMonster->GetComponent<BigWolfScript>();

			if (nullptr != script)
			{
				bool stoming = script->IsStoming();

				eWolfBossState state = script->GetCurWolfState();


				if (stoming && script->GetCurWolfState() == eWolfBossState::STOM_END)
				{
					script->SetStoming(false);
					GetOwner()->SetState(GameObject::eState::Dead);
				}
			}

		}

		else if (GetOwner()->GetName() == L"StomingHitGroundObj2")
		{
			BigWolfScript* script = mMonster->GetComponent<BigWolfScript>();

			if (nullptr != script)
			{
				bool stoming = script->IsStoming();

				eWolfBossState state = script->GetCurWolfState();


				if (stoming && script->GetCurWolfState() == eWolfBossState::STOM_END)
				{
					script->SetStoming(false);
					GetOwner()->SetState(GameObject::eState::Dead);
				}
			}
		}


		else if (GetOwner()->GetName() == L"HowlingHitGroundObj")
		{
			BigWolfScript* script = mMonster->GetComponent<BigWolfScript>();

			if (nullptr != script)
			{
				bool howling = script->IsHowling();

				eWolfBossState state = script->GetCurWolfState();


				if (howling && script->GetCurWolfState() == eWolfBossState::HOWLING_END)
				{
					script->SetHowling(false);
					GetOwner()->SetState(GameObject::eState::Dead);
				}

			}
		}

		else if (GetOwner()->GetName() == L"StompHitGroundObj")
		{
		/*	BigWolfScript* script = mMonster->GetComponent<BigWolfScript>();

			if (nullptr != script)
			{
				bool howling = script->IsHowling();

				eWolfBossState state = script->GetCurWolfState();


				if (howling && script->GetCurWolfState() == eWolfBossState::HOWLING_END)
				{
					script->SetHowling(false);
					GetOwner()->SetState(GameObject::eState::Dead);
				}

			}*/
		}








	}
	void HitGroundScript::OnCollisionEnter(Collider2D* other)
	{
		if (GetOwner()->GetName() == L"WoodHitGroundObj"
			|| GetOwner()->GetName() == L"WolfHitGroundObj"
			|| GetOwner()->GetName() == L"ZombieHitGroundObj")
		{

			if (other->GetName() == L"colHit_player")
			{
				PlayerScript* script = other->GetOwner()->GetComponent<PlayerScript>();
				bool bDash = script->IsDash();

				// 대쉬 중엔 공격 무력화 
				if (!bDash)
				{
					CharacterState* State = GameState::GetInst().GetState(L"Player");
					State->SetCurrentHP(State->GetCurrentHP() - 10);

					// 공격 당했을 시 HIT 상태로 변경 
					script->ChangeState(ePlayerState::HIT);


				}
			}


		}
	}
	void HitGroundScript::OnCollisionStay(Collider2D* other)
	{
	}
	void HitGroundScript::OnCollisionExit(Collider2D* other)
	{
	}
}