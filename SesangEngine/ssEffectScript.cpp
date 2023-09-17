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
#include "ssrenderer.h"


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

		// sword 관련 
		std::shared_ptr<ss::graphics::Texture> Image4 = Resources::Find<ss::graphics::Texture>(L"S_PlayerSPEffect");
		std::shared_ptr<ss::graphics::Texture> Image5 = Resources::Find<ss::graphics::Texture>(L"S_PlayerHitEffect");



		// 피스톨 관련
		std::shared_ptr<ss::graphics::Texture> Image6 = Resources::Find<ss::graphics::Texture>(L"P_PlayerAttack1_2_Effect");
		std::shared_ptr<ss::graphics::Texture> Image7 = Resources::Find<ss::graphics::Texture>(L"P_PlayerAttack3_Effect");
		std::shared_ptr<ss::graphics::Texture> Image8 = Resources::Find<ss::graphics::Texture>(L"P_PlayerHit_Effect");
		std::shared_ptr<ss::graphics::Texture> Image9 = Resources::Find<ss::graphics::Texture>(L"P_PlayerspAttack_Effect");


		std::shared_ptr<ss::graphics::Texture> Image10 = Resources::Find<ss::graphics::Texture>(L"P_PlayerOverloading_Effect");
		std::shared_ptr<ss::graphics::Texture> Image11 = Resources::Find<ss::graphics::Texture>(L"P_PlayerOverloadEnd_Effect");
		std::shared_ptr<ss::graphics::Texture> Image12 = Resources::Find<ss::graphics::Texture>(L"P_PlayerOverloadCircle_Effect");



		// 건틀렛 관련 
		std::shared_ptr<ss::graphics::Texture> Image13 = Resources::Find<ss::graphics::Texture>(L"G_PlayerspAttack1_Effect");
		std::shared_ptr<ss::graphics::Texture> Image14 = Resources::Find<ss::graphics::Texture>(L"G_PlayerspAttack2_Effect");
		std::shared_ptr<ss::graphics::Texture> Image15 = Resources::Find<ss::graphics::Texture>(L"G_PlayerOverload_Effect");

		// 보스 늑대용
		anim->Create(L"Boss_Wolf_HowlingEffect", Image1, Vector2(0.f, 0.f), Vector2(583.f, 123.f), 6, Vector2(583.f, 123.f));
		
		anim->Create(L"Boss_Wolf_BreathStartEffectL", Image2, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 4, Vector2(663.f, 69.f), Vector2::Zero, 0.1f, true);
		anim->Create(L"Boss_Wolf_BreathEndEffectL", Image3, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f), Vector2::Zero, 0.1f, true);

		anim->Create(L"Boss_Wolf_BreathStartEffectR", Image2, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 4, Vector2(663.f, 69.f));
		anim->Create(L"Boss_Wolf_BreathEndEffectR", Image3, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f));

		// 플레이어용
		// sword
		anim->Create(L"S_PlayerSPEffectR", Image4, Vector2(0.f, 0.f), Vector2(256.f, 33.f), 8, Vector2(256.f, 33.f));
		anim->Create(L"S_PlayerSPEffectL", Image4, Vector2(0.f, 0.f), Vector2(256.f, 33.f), 8, Vector2(256.f, 33.f), Vector2::Zero, 0.1f, true);

		anim->Create(L"S_PlayerHitEffectR", Image5, Vector2(0.f, 0.f), Vector2(51.f, 51.f), 5, Vector2(51.f, 51.f));
		anim->Create(L"S_PlayerHitEffectL", Image5, Vector2(0.f, 0.f), Vector2(51.f, 51.f), 5, Vector2(51.f, 51.f), Vector2::Zero, 0.1f, true);

		// pistol
		anim->Create(L"P_PlayerAttack1_2_EffectR", Image6, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f));
		anim->Create(L"P_PlayerAttack1_2_EffectL", Image6, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f), Vector2::Zero, 0.1f, true);

		anim->Create(L"P_PlayerAttack3_EffectR", Image7, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f));
		anim->Create(L"P_PlayerAttack3_EffectL", Image7, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f), Vector2::Zero, 0.1f, true);

		anim->Create(L"P_PlayerHit_EffectR", Image8, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f));
		anim->Create(L"P_PlayerHit_EffectL", Image8, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f), Vector2::Zero, 0.1f, true);

		anim->Create(L"P_PlayerspAttack_EffectR", Image9, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f));
		anim->Create(L"P_PlayerspAttack_EffectL", Image9, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f), Vector2::Zero, 0.1f, true);

		anim->Create(L"P_PlayerOverloading_EffectR", Image10, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f));
		anim->Create(L"P_PlayerOverloading_EffectL", Image10, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f), Vector2::Zero, 0.1f, true);

		anim->Create(L"P_PlayerOverloadEnd_EffectR", Image11, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f));
		anim->Create(L"P_PlayerOverloadEnd_EffectL", Image11, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f), Vector2::Zero, 0.1f, true);


		anim->Create(L"P_PlayerOverloadCircle_EffectR", Image12, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f));
		anim->Create(L"P_PlayerOverloadCircle_EffectL", Image12, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f), Vector2::Zero, 0.1f, true);

		anim->Create(L"G_PlayerspAttack1_EffectR", Image13, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f));
		anim->Create(L"G_PlayerspAttack1_EffectL", Image13, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f), Vector2::Zero, 0.1f, true);


		anim->Create(L"G_PlayerspAttack2_EffectR", Image14, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f));
		anim->Create(L"G_PlayerspAttack2_EffectL", Image14, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f), Vector2::Zero, 0.1f, true);

		anim->Create(L"G_PlayerOverload_EffectR", Image15, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f));
		anim->Create(L"G_PlayerOverload_EffectL", Image15, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f), Vector2::Zero, 0.1f, true);

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
			//Transform* playertr = mOwnerObj->GetComponent<Transform>();

			
			mr->SetMaterial(Resources::Find<Material>(L"UILowHPMtrl"));
			tr->SetScale(Vector3(2300.f, 500.f, 0.f));
			tr->SetPosition(Vector3(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition()));
		}

		else if (GetOwner()->GetName() == L"Player_Sword_SPEffectR")
		{
			//Transform* playertr = mOwnerObj->GetComponent<Transform>();

			anim->PlayAnimation(L"S_PlayerSPEffectR", false);
			tr->SetScale(Vector3(256.f, 33.f, 0.f));

		}

		else if (GetOwner()->GetName() == L"Player_Sword_SPEffectL")
		{
			//Transform* playertr = mOwnerObj->GetComponent<Transform>();

			anim->PlayAnimation(L"S_PlayerSPEffectL", false);
			tr->SetScale(Vector3(256.f, 33.f, 0.f));

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
			Transform* playertr = mOwnerObj->GetComponent<Transform>();

			//tr->SetPosition(Vector3(playertr->GetPosition().x, playertr->GetPosition().y + 50.f, playertr->GetPosition().z));


			PlayerScript* playerscript = mOwnerObj->GetComponent<PlayerScript>();

			if (playerscript->GetState() == ePlayerState::IDLE)
			{
				GetOwner()->SetState(GameObject::eState::Dead);
			}
		


		}

		else if (GetOwner()->GetName() == L"Player_Sword_SPEffectR" ||
			GetOwner()->GetName() == L"Player_Sword_SPEffectL")
		{
			Transform* tr = GetOwner()->GetComponent<Transform>();
			Transform* playertr = mOwnerObj->GetComponent<Transform>();

			//tr->SetPosition(Vector3(playertr->GetPosition().x, playertr->GetPosition().y + 50.f, playertr->GetPosition().z));


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