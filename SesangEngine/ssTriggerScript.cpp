#include "ssTriggerScript.h"
#include "ssMonster.h"
#include "ssBigWolfScript.h"
#include "ssResources.h"
#include "ssMeshRenderer.h"
#include "ssAnimator.h"
#include "ssPlatform.h"


namespace ss
{
	TriggerScript::TriggerScript()
	{
	}
	TriggerScript::~TriggerScript()
	{
	}
	void TriggerScript::Initialize()
	{
	}
	void TriggerScript::Update()
	{
	}
	void TriggerScript::OnCollisionEnter(Collider2D* other)
	{


		if (other->GetName() == L"colHit_player")
		{
			MeshRenderer* mr = mMonster->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimationMaterial"));

			Animator* animator = mMonster->GetComponent<Animator>();
			animator->PlayAnimation(L"Boss_Wolf_SpawnL", false);

		
		
		}
	}

	void TriggerScript::OnCollisionStay(Collider2D* other)
	{
		if (other->GetName() == L"colHit_player")
		{
			Animator* animator = mMonster->GetComponent<Animator>();

			if (animator->GetCurActiveAnimation()->IsComplete())
			{
				int i = 0;
				BigWolfScript* script = mMonster->GetComponent<BigWolfScript>();
				script->ChangeState(eWolfBossState::IDLE);
			}
		}




	}
	void TriggerScript::OnCollisionExit(Collider2D* other)
	{
	
		GetOwner()->SetState(GameObject::eState::Dead);


	}
}