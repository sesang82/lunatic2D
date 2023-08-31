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

namespace ss
{
	HitGroundScript::HitGroundScript()
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
		
				
			if (paunched && anim->GetCurActiveAnimation()->GetIndex() == 7)
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



	}
	void HitGroundScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void HitGroundScript::OnCollisionStay(Collider2D* other)
	{
	}
	void HitGroundScript::OnCollisionExit(Collider2D* other)
	{
	}
}