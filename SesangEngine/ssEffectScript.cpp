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

// 아이템도 그냥 여기다가 같이 넣기 


namespace ss
{
	EffectScript::EffectScript()
		: mOwnerObj(nullptr)
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

		// effect 관련
		std::shared_ptr<ss::graphics::Texture> Image16 = Resources::Find<ss::graphics::Texture>(L"PlayerOverloadingStartEffect");
		std::shared_ptr<ss::graphics::Texture> Image17 = Resources::Find<ss::graphics::Texture>(L"PlayerOverloadingEffect");

		// overloading 꾸미는 용도 
		std::shared_ptr<ss::graphics::Texture> Image18 = Resources::Find<ss::graphics::Texture>(L"Overload_LB");
		std::shared_ptr<ss::graphics::Texture> Image19 = Resources::Find<ss::graphics::Texture>(L"Overload_LT");
		std::shared_ptr<ss::graphics::Texture> Image20 = Resources::Find<ss::graphics::Texture>(L"Overload_RT");
		std::shared_ptr<ss::graphics::Texture> Image21 = Resources::Find<ss::graphics::Texture>(L"Overload_RB");
		std::shared_ptr<ss::graphics::Texture> Image22 = Resources::Find<ss::graphics::Texture>(L"Overload_BottomHorizion");
		std::shared_ptr<ss::graphics::Texture> Image23 = Resources::Find<ss::graphics::Texture>(L"Overload_TopHorizion");

		// 플레이어용 기타 효과
		std::shared_ptr<ss::graphics::Texture> Image24 = Resources::Find<ss::graphics::Texture>(L"Player_GuardEffect");


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

		anim->Create(L"G_PlayerspAttack1_EffectR", Image13, Vector2(0.f, 0.f), Vector2(194.f, 50.f), 6, Vector2(500.f, 50.f), Vector2(85.f, -3.f), 0.08f);
		anim->Create(L"G_PlayerspAttack1_EffectL", Image13, Vector2(0.f, 0.f), Vector2(194.f, 50.f), 6, Vector2(500.f, 50.f), Vector2(78.f, -3.f), 0.08f, true);

		anim->Create(L"G_PlayerspAttack2_EffectR", Image14, Vector2(0.f, 0.f), Vector2(74.f, 49.f), 7, Vector2(130.f, 49.f), Vector2(-15.f, -3.f));
		anim->Create(L"G_PlayerspAttack2_EffectL", Image14, Vector2(0.f, 0.f), Vector2(74.f, 49.f), 7, Vector2(130.f, 49.f), Vector2(-30.f, -3.f), 0.1f, true);

		anim->Create(L"G_PlayerOverload_EffectR", Image15, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f));
		anim->Create(L"G_PlayerOverload_EffectL", Image15, Vector2(0.f, 0.f), Vector2(663.f, 69.f), 3, Vector2(663.f, 69.f), Vector2::Zero, 0.1f, true);

		anim->Create(L"PlayerOverloadStart_EffectR", Image16, Vector2(0.f, 0.f), Vector2(107.f, 78.f), 9, Vector2(107.f, 78.f), Vector2(4.0f, -10.f));
		anim->Create(L"PlayerOverloadStart_EffectL", Image16, Vector2(0.f, 0.f), Vector2(107.f, 78.f), 9, Vector2(107.f, 78.f), Vector2(-7.0f, -10.f), 0.1f, true);

		anim->Create(L"PlayerOverloading_EffectR", Image17, Vector2(0.f, 0.f), Vector2(64.f, 40.f), 13, Vector2(64.f, 40.f), Vector2(0.0f, 7.f));
		anim->Create(L"PlayerOverloading_EffectL", Image17, Vector2(0.f, 0.f), Vector2(64.f, 40.f), 13, Vector2(64.f, 40.f), Vector2(0.0f, 7.f), 0.1f, true);

		anim->Create(L"Over_LB", Image18, Vector2(0.f, 0.f), Vector2(142.f, 136.f), 8, Vector2(142.f, 136.f));
		anim->Create(L"Over_LT", Image19, Vector2(0.f, 0.f), Vector2(142.f, 136.f), 8, Vector2(142.f, 136.f));
		anim->Create(L"Over_RT", Image20, Vector2(0.f, 0.f), Vector2(142.f, 136.f), 8, Vector2(142.f, 136.f));
		anim->Create(L"Over_RB", Image21, Vector2(0.f, 0.f), Vector2(142.f, 136.f), 8, Vector2(142.f, 136.f));
		anim->Create(L"Over_TopHorizion", Image22, Vector2(0.f, 0.f), Vector2(215.f, 44.f), 6, Vector2(215.f, 44.f));
		anim->Create(L"Over_BottomHorizion", Image23, Vector2(0.f, 0.f), Vector2(215.f, 44.f), 6, Vector2(215.f, 44.f));

		// 
		anim->Create(L"PlayerGuardEffectR", Image24, Vector2(0.f, 0.f), Vector2(24.f, 55.f), 5, Vector2(70.f, 55.f), Vector2(17.f, 0.f));
		anim->Create(L"PlayerGuardEffectL", Image24, Vector2(0.f, 0.f), Vector2(24.f, 55.f), 5, Vector2(70.f, 55.f), Vector2(7, 0.f), 0.1f,  true);


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



		// =========== 오버로드 UI관련 

		else if (GetOwner()->GetName() == L"OverloadStratEffectR")
		{
			//Transform* playertr = mOwnerObj->GetComponent<Transform>();

			anim->PlayAnimation(L"PlayerOverloadStart_EffectR", false);
			tr->SetScale(Vector3(107.f, 78.f, 0.f));

		}

		else if (GetOwner()->GetName() == L"OverloadStratEffectL")
		{
			//Transform* playertr = mOwnerObj->GetComponent<Transform>();

			anim->PlayAnimation(L"PlayerOverloadStart_EffectL", false);
			tr->SetScale(Vector3(107.f, 78.f, 0.f));

		}


		else if (GetOwner()->GetName() == L"PlayerOverloadingEffectR")
		{
			//Transform* playertr = mOwnerObj->GetComponent<Transform>();

			anim->PlayAnimation(L"PlayerOverloading_EffectR", true);
			tr->SetScale(Vector3(64.f, 40.f, 0.f));

		}


		else if (GetOwner()->GetName() == L"PlayerOverloadingEffectL")
		{
			//Transform* playertr = mOwnerObj->GetComponent<Transform>();

			anim->PlayAnimation(L"PlayerOverloading_EffectL", true);
			tr->SetScale(Vector3(64.f, 40.f, 0.f));

		}


		else if (GetOwner()->GetName() == L"OverloadUI")
		{
			mr->SetMaterial(Resources::Find<Material>(L"UI_OverloadScreenMtrl"));
			tr->SetScale(Vector3(700.f, 390.f, 0.f));
			tr->SetPosition(Vector3(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition()));

		}


		// ====== 아래에 추가하기 
		else if (GetOwner()->GetName() == L"OverloadLTUI")
		{
			anim->PlayAnimation(L"Over_LT", true);
			tr->SetScale(Vector3(142.f, 136.f, 0.f));

			tr->SetPosition(Vector3(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().x - 500.f,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().y,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().z));
		}

		else if (GetOwner()->GetName() == L"OverloadLBUI")
		{
			anim->PlayAnimation(L"Over_LB", true);
			tr->SetScale(Vector3(142.f, 136.f, 0.f));

			tr->SetPosition(Vector3(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().x - 500.f,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().y,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().z));
		}

		else if (GetOwner()->GetName() == L"OverloadRTUI")
		{
			anim->PlayAnimation(L"Over_RT", true);
			tr->SetScale(Vector3(142.f, 136.f, 0.f));

			tr->SetPosition(Vector3(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().x - 500.f,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().y,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().z));
				}

		else if (GetOwner()->GetName() == L"OverloadRBUI")
		{
			anim->PlayAnimation(L"Over_RB", true);
			tr->SetScale(Vector3(142.f, 136.f, 0.f));

			tr->SetPosition(Vector3(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().x - 500.f,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().y,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().z));
				}

		else if (GetOwner()->GetName() == L"OverloadTopHorizionUI")
		{
			anim->PlayAnimation(L"Over_TopHorizion", true);
				tr->SetScale(Vector3(480.f, 45.f, 0.f));

			tr->SetPosition(Vector3(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().x,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().y,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().z));
				}

		else if (GetOwner()->GetName() == L"OverloadBottomHorizionUI")
		{
			anim->PlayAnimation(L"Over_BottomHorizion", true);
			tr->SetScale(Vector3(480.f, 45.f, 0.f));

			tr->SetPosition(Vector3(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().x,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().y,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().z));
		}


		// =========================
		// 플레이어 건틀렛 관련
		else if (GetOwner()->GetName() == L"Player_Gauntlet_SPEffect_DustR")
		{
			anim->PlayAnimation(L"G_PlayerspAttack2_EffectR", false);
			tr->SetScale(Vector3(130.f, 49.f, 0.f));

		
		}

		else if (GetOwner()->GetName() == L"Player_Gauntlet_SPEffect_DustL")
		{
			anim->PlayAnimation(L"G_PlayerspAttack2_EffectL", false);
			tr->SetScale(Vector3(130.f, 49.f, 0.f));

		}


		else if (GetOwner()->GetName() == L"Player_Gauntlet_SPEffect_PushR")
		{
			anim->PlayAnimation(L"G_PlayerspAttack1_EffectR", false);
			tr->SetScale(Vector3(500.f, 50.f, 0.f));

		}


		else if (GetOwner()->GetName() == L"Player_Gauntlet_SPEffect_PushL")
		{
			anim->PlayAnimation(L"G_PlayerspAttack1_EffectL", false);
			tr->SetScale(Vector3(500.f, 50.f, 0.f));

		}
		

		// ===== 플레이어 기타 효과 
		else if (GetOwner()->GetName() == L"PlayerGuardEffectObjR")
		{
			anim->PlayAnimation(L"PlayerGuardEffectR", false);
			tr->SetScale(Vector3(70.f, 55.f, 0.f));

		}

		else if (GetOwner()->GetName() == L"PlayerGuardEffectObjL")
		{
			anim->PlayAnimation(L"PlayerGuardEffectL", false);
			tr->SetScale(Vector3(70.f, 55.f, 0.f));

		}
	}


	void EffectScript::Update()
	{

		Animator* anim = GetOwner()->GetComponent<Animator>();

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

		else if (GetOwner()->GetName() == L"OverloadStratEffectR"
			|| GetOwner()->GetName() == L"OverloadStratEffectL")
		{
			//Transform* playertr = mOwnerObj->GetComponent<Transform>();
			PlayerScript* playerscript = mOwnerObj->GetComponent<PlayerScript>();

			Transform* Effecttr = GetOwner()->GetComponent<Transform>();

			// 이펙트가 플레이어 위치를 실시간으로 따라가게 한다. 
			Effecttr->SetPosition(mOwnerObj->GetComponent<Transform>()->GetPosition());

	/*		Animator* EffectAnim = GetOwner()->GetComponent<Animator>();

			if (EffectAnim->GetCurActiveAnimation()->IsComplete())
			{

			}*/
		

			if (playerscript->GetState() == ePlayerState::OVERLOAD_START 
				|| playerscript->GetState() == ePlayerState::OVERLOADING)
			{
				GetOwner()->SetState(GameObject::eState::Dead);
			}

		}

		else if (GetOwner()->GetName() == L"PlayerOverloadingEffectR")
		{
			//Transform* playertr = mOwnerObj->GetComponent<Transform>();
			PlayerScript* playerscript = mOwnerObj->GetComponent<PlayerScript>();

			// 플레이어 위치를 따라다닌다.
			Transform* Effecttr = GetOwner()->GetComponent<Transform>();

			// 이펙트가 플레이어 위치를 실시간으로 따라가게 한다. 

			if (playerscript->GetCurDir().x == 1.0)
			{
				Effecttr->SetPosition(mOwnerObj->GetComponent<Transform>()->GetPosition());
			}

			if (playerscript->GetCurDir().x == -1.0)
			{
				Effecttr->SetPosition(mOwnerObj->GetComponent<Transform>()->GetPosition().x + 14.f,
					mOwnerObj->GetComponent<Transform>()->GetPosition().y,
					mOwnerObj->GetComponent<Transform>()->GetPosition().z);
			}


		}

		else if (GetOwner()->GetName() == L"PlayerOverloadingEffectL")
		{
			PlayerScript* playerscript = mOwnerObj->GetComponent<PlayerScript>();

			// 플레이어 위치를 따라다닌다.
			Transform* Effecttr = GetOwner()->GetComponent<Transform>();

			// 이펙트가 플레이어 위치를 실시간으로 따라가게 한다. 

			if (playerscript->GetCurDir().x == 1.0)
			{
				Effecttr->SetPosition(mOwnerObj->GetComponent<Transform>()->GetPosition().x - 14.f,
					mOwnerObj->GetComponent<Transform>()->GetPosition().y,
					mOwnerObj->GetComponent<Transform>()->GetPosition().z);
			}

			if (playerscript->GetCurDir().x == -1.0)
			{
				Effecttr->SetPosition(mOwnerObj->GetComponent<Transform>()->GetPosition());

		
			}
		}

		else if (GetOwner()->GetName() == L"OverloadUI")
		{
			Transform* Effecttr = GetOwner()->GetComponent<Transform>();

			Effecttr->SetPosition(Vector3(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition()));
			//Effecttr->SetPosition(mOwnerObj->GetComponent<Transform>()->GetPosition());

	

		}
		

		// ======================================================
		else if (GetOwner()->GetName() == L"OverloadLBUI")
		{
			Transform* Effecttr = GetOwner()->GetComponent<Transform>();

			Effecttr->SetPosition(Vector3(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().x - 275.f,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().y - 127.f,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().z));

		}


		else if (GetOwner()->GetName() == L"OverloadLTUI")
		{
			Transform* Effecttr = GetOwner()->GetComponent<Transform>();

			Effecttr->SetPosition(Vector3(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().x - 275.f,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().y + 127.f,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().z));
			//Effecttr->SetPosition(mOwnerObj->GetComponent<Transform>()->GetPosition());



		}

		else if (GetOwner()->GetName() == L"OverloadRBUI")
		{
			Transform* Effecttr = GetOwner()->GetComponent<Transform>();

			Effecttr->SetPosition(Vector3(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().x + 275.f,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().y - 127.f,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().z));
			//Effecttr->SetPosition(mOwnerObj->GetComponent<Transform>()->GetPosition());


		}

		else if (GetOwner()->GetName() == L"OverloadRTUI")
		{
			Transform* Effecttr = GetOwner()->GetComponent<Transform>();

			Effecttr->SetPosition(Vector3(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().x + 275.f,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().y + 127.f,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().z));
			//Effecttr->SetPosition(mOwnerObj->GetComponent<Transform>()->GetPosition());



		}

		else if (GetOwner()->GetName() == L"OverloadTopHorizionUI")
		{
			Transform* Effecttr = GetOwner()->GetComponent<Transform>();

			Effecttr->SetPosition(Vector3(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().x,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().y + 174.f,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().z));
			//Effecttr->SetPosition(mOwnerObj->GetComponent<Transform>()->GetPosition());



		}

		else if (GetOwner()->GetName() == L"OverloadBottomHorizionUI")
		{
			Transform* Effecttr = GetOwner()->GetComponent<Transform>();

			Effecttr->SetPosition(Vector3(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().x ,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().y - 174.f,
				renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition().z));
			//Effecttr->SetPosition(mOwnerObj->GetComponent<Transform>()->GetPosition());



		}


		// ============= 건틀렛

		else if (GetOwner()->GetName() == L"Player_Gauntlet_SPEffect_DustR")
		{
			//Transform* playertr = mOwnerObj->GetComponent<Transform>();
			PlayerScript* playerscript = mOwnerObj->GetComponent<PlayerScript>();

			if (anim->GetCurActiveAnimation()->IsComplete())
			{
				GetOwner()->SetState(GameObject::eState::Dead);
			

			}

		}


		else if (GetOwner()->GetName() == L"Player_Gauntlet_SPEffect_DustL")
		{
			//Transform* playertr = mOwnerObj->GetComponent<Transform>();
			PlayerScript* playerscript = mOwnerObj->GetComponent<PlayerScript>();

			if (anim->GetCurActiveAnimation()->IsComplete())
			{
				GetOwner()->SetState(GameObject::eState::Dead);
			}
		}


		else if (GetOwner()->GetName() == L"Player_Gauntlet_SPEffect_PushR")
		{
			//Transform* playertr = mOwnerObj->GetComponent<Transform>();
			PlayerScript* playerscript = mOwnerObj->GetComponent<PlayerScript>();

			if (anim->GetCurActiveAnimation()->IsComplete())
			{
				GetOwner()->SetState(GameObject::eState::Dead);

			}

		}

		else if (GetOwner()->GetName() == L"Player_Gauntlet_SPEffect_PushL")
		{
			//Transform* playertr = mOwnerObj->GetComponent<Transform>();
			PlayerScript* playerscript = mOwnerObj->GetComponent<PlayerScript>();

			if (anim->GetCurActiveAnimation()->IsComplete())
			{
				GetOwner()->SetState(GameObject::eState::Dead);
		
			}

		}


		else if (GetOwner()->GetName() == L"PlayerGuardEffectObjR"
				|| GetOwner()->GetName() == L"PlayerGuardEffectObjL")
		{
			//PlayerScript* playerscript = mOwnerObj->GetComponent<PlayerScript>();

			if (anim->GetCurActiveAnimation()->IsComplete())
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