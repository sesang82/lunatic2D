#include "ssEffectScript.h"
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


namespace ss
{
	EffectScript::EffectScript()
	{

	}
	EffectScript::~EffectScript()
	{
	}
	void EffectScript::Initialize()
	{
		MeshRenderer* mr = GetOwner()->GetComponent<MeshRenderer>();
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Animator* anim = GetOwner()->GetComponent<Animator>();

		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_HowlingEffect");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_BreathEffectStart");
		std::shared_ptr<ss::graphics::Texture> Image3 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_BreathEffectEnd");

		anim->Create(L"Boss_Wolf_HowlingEffect", Image1, Vector2(0.f, 0.f), Vector2(583.f, 123.f), 6, Vector2(583.f, 123.f));
		
		anim->Create(L"Boss_Wolf_BreathStartEffectL", Image2, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 4, Vector2(663.f, 69.f), Vector2::Zero, 0.1f, true);
		anim->Create(L"Boss_Wolf_BreathEndEffectL", Image3, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f), Vector2::Zero, 0.1f, true);

		anim->Create(L"Boss_Wolf_BreathStartEffectR", Image2, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 4, Vector2(663.f, 69.f));
		anim->Create(L"Boss_Wolf_BreathEndEffectR", Image3, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f));


		if (GetOwner()->GetName() == L"HowlingEffectObj")
		{
			tr->SetScale(Vector3(583.f, 123.f, 0.f));
			anim->PlayAnimation(L"Boss_Wolf_HowlingEffect", true);
			
		}

		else if (GetOwner()->GetName() == L"BreathingObjL")
		{
			anim->PlayAnimation(L"Boss_Wolf_BreathStartEffectL", true);
		}

		else if (GetOwner()->GetName() == L"BreathingObjR")
		{
			anim->PlayAnimation(L"Boss_Wolf_BreathStartEffectR", true);
		}

		else if (GetOwner()->GetName() == L"PlayerHitUI")
		{
			mr->SetMaterial(Resources::Find<Material>(L"UILowHPMtrl"));
			tr->SetScale(Vector3(512.f, 288.f, 0.f));
			tr->SetPosition(Vector3(0.f, -350.f, 150.f));
		}

	}


	void EffectScript::Update()
	{



		//if (SceneManager::GetActiveScene()->GetName() == L"Stage1Scene")
		if (GetOwner()->GetName() == L"HowlingEffectObj")
		{
			BigWolfScript* bosswolfScript = mOwnerObj->GetComponent<BigWolfScript>();
			eWolfBossState state = bosswolfScript->GetCurWolfState();

			if (bosswolfScript->GetCurWolfState() == eWolfBossState::HOWLING_END
				|| bosswolfScript->GetCurWolfState() == eWolfBossState::DEAD)
			{
				GetOwner()->SetState(GameObject::eState::Dead); // 하울링 이펙트를 제거한다.
			}

		}

		else if (GetOwner()->GetName() == L"BreathingObjL")
		{
			BigWolfScript* bosswolfScript = mOwnerObj->GetComponent<BigWolfScript>();
			eWolfBossState state = bosswolfScript->GetCurWolfState();

			Animator* anim = GetOwner()->GetComponent<Animator>();
			Transform* tr = GetOwner()->GetComponent<Transform>();

			if (bosswolfScript->GetCurWolfState() == eWolfBossState::BREATHING)
			{
				anim->PlayAnimation(L"Boss_Wolf_BreathStartEffectL", true);
				tr->SetScale(Vector3(2652.f, 69.f, 0.f));
			}

			else if (bosswolfScript->GetCurWolfState() == eWolfBossState::BREATH_END)
			{
				anim->PlayAnimation(L"Boss_Wolf_BreathEndEffectL", false);
				tr->SetScale(Vector3(1989.f, 69.f, 0.f));
			}


			else if (bosswolfScript->GetCurWolfState() == eWolfBossState::STOM_START
				|| bosswolfScript->GetCurWolfState() == eWolfBossState::DEAD)
			{
				GetOwner()->SetState(GameObject::eState::Dead); // breath 이펙트를 제거한다.
			}

		}

		else if (GetOwner()->GetName() == L"BreathingObjR")
		{
			BigWolfScript* bosswolfScript = mOwnerObj->GetComponent<BigWolfScript>();
			eWolfBossState state = bosswolfScript->GetCurWolfState();

			Animator* anim = GetOwner()->GetComponent<Animator>();
			Transform* tr = GetOwner()->GetComponent<Transform>();

			if (bosswolfScript->GetCurWolfState() == eWolfBossState::BREATHING)
			{
				anim->PlayAnimation(L"Boss_Wolf_BreathStartEffectR", true);
				tr->SetScale(Vector3(1989.f, 69.f, 0.f));
			}

			else if (bosswolfScript->GetCurWolfState() == eWolfBossState::BREATH_END)
			{
				anim->PlayAnimation(L"Boss_Wolf_BreathEndEffectR", false);
				tr->SetScale(Vector3(1989.f, 69.f, 0.f));
			}


			else if (bosswolfScript->GetCurWolfState() == eWolfBossState::STOM_START
				|| bosswolfScript->GetCurWolfState() == eWolfBossState::DEAD)
			{
				GetOwner()->SetState(GameObject::eState::Dead); // breath 이펙트를 제거한다.
			}

		}


		else if (GetOwner()->GetName() == L"PlayerHitUI")
		{
			Transform* tr = GetOwner()->GetComponent<Transform>();

			PlayerScript* playerscript = mOwnerObj->GetComponent<PlayerScript>();

			if (playerscript->GetState() == ePlayerState::IDLE)
			{
				GetOwner()->SetState(GameObject::eState::Dead);
			}
		


		}

	}
	void EffectScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void EffectScript::OnCollisionStay(Collider2D* other)
	{
	}
	void EffectScript::OnCollisionExit(Collider2D* other)
	{
	}
}