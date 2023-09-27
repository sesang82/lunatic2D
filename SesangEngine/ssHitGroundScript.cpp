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



		// �ϴ� �̷��� �� ������ hit ground�� ũ�Ⱑ �ٸ� ���� �־ �ϴ� �̷��� �س��µ� ���߿� �׷� �ʿ������ �����丵 �ϱ�
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


		else if (GetOwner()->GetName() == L"StompHitGroundObj") // �̰� �̹��� �������� 
		{
			mr->SetMaterial(Resources::Find<Material>(L"stompHitGroundt_Mtrl"));
			tr->SetScale(Vector3(141.f, 32.f, 0.f));

		}


	}
	void HitGroundScript::Update()
	{
		// ����ϴ� ������ �ִϸ��̼� ������ �����Ͽ� �ε����� ����. 

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

				// �뽬 �߿� ���� ����ȭ 
				if (!bDash)
				{
					CharacterState* State = GameState::GetInst().GetState(L"Player");
					State->SetCurrentHP(State->GetCurrentHP() - 10);

					// ���� ������ �� HIT ���·� ���� 
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