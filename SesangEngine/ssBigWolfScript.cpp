#include "ssBigWolfScript.h"
#include "ssResources.h"
#include "ssTexture.h"
#include "ssAnimator.h"
#include "ssCharacterState.h"
#include "ssGameObject.h"
#include "ssPlayer.h"
#include "ssRigidbody2D.h"
#include "ssTime.h"
#include "ssObject.h"
#include "ssRangeCollider.h"
#include "ssPlayerScript.h"
#include "ssAttackCollider.h"
#include "ssEffect.h"
#include "ssMeshRenderer.h"
#include "ssHitGroundScript.h"
#include "ssMonster.h"
#include "ssRenderer.h"
#include "ssGameState.h"

namespace ss
{
	BigWolfScript::BigWolfScript()
		: mbStomStart(false)
		, mbStomEnd(false)
		, mbIdle(false)
		, mLandingPos(Vector3::Zero)
		, mbAppear(false)
		, mbDisappear(false)
		, mbDash(false)
		, mbBreathStart(false)
		, mbBreating(false)
		, mbStoming(false)
		, miStomCount(0)
		, miAppearCount(0)
		, mbHowling(false)
	{
		m_tMonsterInfo.m_fSpeed = 200.f;
		m_tMonsterInfo.m_fAttack = 10.f;
		m_tMonsterInfo.m_fDetectRange = 300.f;
	}
	BigWolfScript::~BigWolfScript()
	{
	}
	void BigWolfScript::Initialize()
	{

		MonsterScript::Initialize();


		

		mCharacterState->SetMaxHP(110.f);
		mCharacterState->SetCurrentHP(110.f);

		Transform* tr = GetOwner()->GetComponent<Transform>();
		tr->SetScale(272.f, 271.f, 1.f);

		

		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Idle");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Spawn");

		std::shared_ptr<ss::graphics::Texture> Image4 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_BreathEffectStart");
		std::shared_ptr<ss::graphics::Texture> Image5 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_BreathEffectEnd");
		std::shared_ptr<ss::graphics::Texture> Image6 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Dash");
		std::shared_ptr<ss::graphics::Texture> Image7 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Die");
		std::shared_ptr<ss::graphics::Texture> Image8 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Hit");

		std::shared_ptr<ss::graphics::Texture> Image21 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_HowlingStart");
		std::shared_ptr<ss::graphics::Texture> Image22 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Howling");
		std::shared_ptr<ss::graphics::Texture> Image23 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_HowlingEnd");

		std::shared_ptr<ss::graphics::Texture> Image10 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_HowlingEffect");
		std::shared_ptr<ss::graphics::Texture> Image11 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_HowlingPostioin");
		std::shared_ptr<ss::graphics::Texture> Image12 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_MoveAppear");
		std::shared_ptr<ss::graphics::Texture> Image13 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_MoveDissappear");
		std::shared_ptr<ss::graphics::Texture> Image14 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Shadow");
		std::shared_ptr<ss::graphics::Texture> Image15 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_ShadowEffect");
		std::shared_ptr<ss::graphics::Texture> Image16 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Storm1");
		std::shared_ptr<ss::graphics::Texture> Image17 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Storm2");

		std::shared_ptr<ss::graphics::Texture> Image18 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_BreathAttackStart");
		std::shared_ptr<ss::graphics::Texture> Image19 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_BreathAttacking");
		std::shared_ptr<ss::graphics::Texture> Image20 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_BreathAttackEnd");


		mAnimator->Create(L"Boss_Wolf_IdleR", Image1, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 9, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_IdleL", Image1, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 9, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);

		//mAnimator->Create(L"Boss_Wolf_SpawnR", Image2, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 12, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_SpawnL", Image2, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 12, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Boss_Wolf_BreathAttackStartR", Image18, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 9, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_BreathAttackStartL", Image18, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 9, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Boss_Wolf_BreathAttackingR", Image19, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 2, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_BreathAttackingL", Image19, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 2, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);


		mAnimator->Create(L"Boss_Wolf_BreathAttackEndR", Image20, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 3, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_BreathAttackEndL", Image20, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 3, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);

		//mAnimator->Create(L"Boss_Wolf_BreathEffectStartR", Image4, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 4, Vector2(272.f, 271.f));
		//mAnimator->Create(L"Boss_Wolf_BreathEffectStartL", Image4, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 4, Vector2(272.f, 271.f), Vector2(-16.f, 0.f), 0.1f, true);

		//mAnimator->Create(L"Boss_Wolf_BreathEffectEndR", Image5, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 5, Vector2(272.f, 271.f));
		//mAnimator->Create(L"Boss_Wolf_BreathEffectEndL", Image5, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 5, Vector2(272.f, 271.f), Vector2(-16.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Boss_Wolf_DashR", Image6, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 25, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_DashL", Image6, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 25, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Boss_Wolf_DieR", Image7, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 18, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_DieL", Image7, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 18, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);


		mAnimator->Create(L"Boss_Wolf_HitR", Image8, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 2, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_HitL", Image8, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 2, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);

		// 하울링 맨 마지막 인덱스 뭔가 이상함 
		mAnimator->Create(L"Boss_Wolf_HowlingStartR", Image21, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 6, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_HowlingStartL", Image21, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 6, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Boss_Wolf_HowlingR", Image22, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 8, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_HowlingL", Image22, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 8, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Boss_Wolf_HowlingEndR", Image23, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 11, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_HowlingEndL", Image23, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 11, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);


		//mAnimator->Create(L"Boss_Wolf_HowlingEffectR", Image10, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 5, Vector2(272.f, 271.f));
		//mAnimator->Create(L"Boss_Wolf_HowlingEffectL", Image10, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 5, Vector2(272.f, 271.f), Vector2(-16.f, 0.f), 0.1f, true);

		//mAnimator->Create(L"Boss_Wolf_HowlingPostioinR", Image11, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 5, Vector2(272.f, 271.f));
		//mAnimator->Create(L"Boss_Wolf_HowlingPostioinL", Image11, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 5, Vector2(272.f, 271.f), Vector2(-16.f, 0.f), 0.1f, true);
		//
		mAnimator->Create(L"Boss_Wolf_MoveAppearR", Image12, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 13, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_MoveAppearL", Image12, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 13, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);
		
		mAnimator->Create(L"Boss_Wolf_MoveDissappearR", Image13, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 11, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_MoveDissappearL", Image13, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 11, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);
	
	/*	mAnimator->Create(L"Boss_Wolf_ShadowR", Image14, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 5, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_ShadowL", Image14, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 5, Vector2(272.f, 271.f), Vector2(-16.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Boss_Wolf_ShadowR", Image15, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 5, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_ShadowL", Image15, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 5, Vector2(272.f, 271.f), Vector2(-16.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Boss_Wolf_ShadowEffectR", Image16, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 5, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_ShadowEffectL", Image16, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 5, Vector2(272.f, 271.f), Vector2(-16.f, 0.f), 0.1f, true);*/

		mAnimator->Create(L"Boss_Wolf_StormStartR", Image16, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 13, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_StormStartL", Image16, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 13, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Boss_Wolf_StormLandingR", Image17, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 8, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_StormLandingL", Image17, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 8, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);



		// ======
	// 애니메이션 방향에 관한 기준	을 잡아준다.


		mDir = -mTransform->Right();



		// 몬스터의 초기 상태는 부여해주지 않는다. (보스만) 
		

		// ===== 기본 충돌체 Hit 받는 용도 
		mCollider->SetName(L"BigWolf_HitCol");
		mCollider->SetType(eColliderType::Rect);
		mCollider->SetSize(Vector2(0.6f, 0.36f));
		mCollider->SetCenter(Vector2(-4.f, -84.f));

		// ===== 리지드바디



		//==== 근접 공격 특정 인덱스 충돌체 
		//충돌체는 여기서 바로 넣지 말고 해당 인덱스 때 넣었다가 빼는 식으로 하기 
		mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"BigWolf_AttackColObj");
		mAttackColliderObj->Initialize();
		//mAttackColliderObj->AddComponent<LizardColScript>();

		mAttackColTr = mAttackColliderObj->GetComponent<Transform>();


		// ==== 메쉬 렌덛
		mMeshRenderer->SetMaterial(Resources::Find<Material>(L"tempMtrl"));


		mAnimator->CompleteEvent(L"Boss_Wolf_BreathAttackingR") = std::bind(&BigWolfScript::Breathing_Event, this);
		mAnimator->CompleteEvent(L"Boss_Wolf_BreathAttackingL") = std::bind(&BigWolfScript::Breathing_Event, this);

	}
	void BigWolfScript::Update()
	{

		Vector3 MonsterPos = mTransform->GetPosition();
		Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();

		mCurDir = (PlayerPos - MonsterPos);
		mCurDir.Normalize();
		mCurDir = Vector3(mCurDir.x, mCurDir.y, 0.f); 

	
		

		// 이동->상태변환->애니메이션

		switch (mCurWolfBossState)
		{
		case ss::eWolfBossState::SPAWN:
			Spawn();
			break;

		case ss::eWolfBossState::IDLE:
			Idle();
			break;
	
		case ss::eWolfBossState::HIT:
			Hit();
			break;

		case ss::eWolfBossState::APPEAR:
			Appear();
			break;

		case ss::eWolfBossState::DISAPPEAR:
			Disappear();
			break;

		case ss::eWolfBossState::BREATH_START:
			Breath_start();
			break;
			
		case ss::eWolfBossState::BREATHING:
			Breathing();
			break;

		case ss::eWolfBossState::BREATH_END:
			Breath_end();
			break;

		case ss::eWolfBossState::HOWLING_START:
			Howling_start();
			break;

		case ss::eWolfBossState::HOWLING:
			Howling();
			break;

		case ss::eWolfBossState::HOWLING_END:
			Howling_end();
			break;
	
		case ss::eWolfBossState::DASH:
			Dash();
			break;

		case ss::eWolfBossState::STOM_START:
			Stom_start();
			break;

		case ss::eWolfBossState::STOMING:
			Stoming();
			break;


		case ss::eWolfBossState::STOM_END:
			Stom_end();
			break;

		case ss::eWolfBossState::DEAD:
			Dead();
			break;

		}




	}
	void BigWolfScript::LateUpdate()
	{
	}
	void BigWolfScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void BigWolfScript::OnCollisionStay(Collider2D* other)
	{
	}
	void BigWolfScript::OnCollisionExit(Collider2D* other)
	{
	}
	void BigWolfScript::Spawn()
	{

	

	}

	// true false 조작은 한 프레임씩 밀려서 조작해야한다. 
	void BigWolfScript::Idle()
	{
	
		mbHit = false;


		if (mPrevWolfBossState == eWolfBossState::DASH)
		{
			if (mDir.x > 0 && !mbIdle)
			{
				mAnimator->PlayAnimation(L"Boss_Wolf_IdleL", false);
				mDir = Vector3(1.f, 0.f, 0.f); // disapper이랑 appear 할 떄의 기준으로 삼기 	
				mbIdle = true;
			}

			else if (mDir.x < 0 && !mbIdle)
			{
				mAnimator->PlayAnimation(L"Boss_Wolf_IdleR", false);
				mDir = Vector3(-1.f, 0.f, 0.f); // disapper이랑 appear 할 떄의 기준으로 삼기 	
				mbIdle = true;
			}
		}

		

		if (mCurDir.x > 0 && !mbIdle)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_IdleR", true);
			mDir = Vector3(1.f, 0.f, 0.f); // disapper이랑 appear 할 떄의 기준으로 삼기 	
			mbIdle = true;
		}

		else if (mCurDir.x < 0 && !mbIdle)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_IdleL", true);
			mDir = Vector3(-1.f, 0.f, 0.f);
			mbIdle = true;
		}
	
		// ======
		m_fTime += Time::DeltaTime();

		float BossHP = mCharacterState->GetCurrentHP();
		
		//// 3초 뒤에 appear 상태로 간다. 
		//if (mPrevWolfBossState == eWolfBossState::STOM_END && m_fTime > 1.f)
		//{
		//	ChangeState(eWolfBossState::DISAPPEAR);
		//	mPrevWolfBossState = eWolfBossState::IDLE;
		//	mbIdle = false;
		//	m_fTime = 0.f;


		//}

	
		if (mPrevWolfBossState == eWolfBossState::STOM_END && m_fTime > 1.f)// && BossHP <= 50.f)
			{
				ChangeState(eWolfBossState::HOWLING_START);
				mPrevWolfBossState = eWolfBossState::IDLE;
				mbIdle = false;
				m_fTime = 0.f;
			}

		else if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			if (m_fTime > 1.f)
			{
				ChangeState(eWolfBossState::BREATH_START);
				mPrevWolfBossState = eWolfBossState::IDLE;

				mbIdle = false;
				m_fTime = 0.f;
			}
		}
	}

	void BigWolfScript::Hit()
	{
	}
	void BigWolfScript::Appear()
	{

		// disappearr의 방향과 반대 방향의 애니메이션을 재생시켜야함 
		if (mDir.x > 0 && !mbAppear)
		{
			mTransform->SetPosition(Vector3(320.f, -183.f, 500.f));

			mAnimator->PlayAnimation(L"Boss_Wolf_MoveAppearL", false);
			mDir = Vector3(1.f, 0.f, 0.f); // disapper이랑 appear 할 떄의 기준으로 삼기 
			mbAppear = true;

		}

		else if (mDir.x < 0 && !mbAppear)
		{
			mTransform->SetPosition(Vector3(-310.f, -183.f, 500.f));
		
			mAnimator->PlayAnimation(L"Boss_Wolf_MoveAppearR", false);
			mDir = Vector3(-1.f, 0.f, 0.f);
			mbAppear = true;

		}



		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{

			renderer::mainCamera->SetTargetSize(1.6f);
			ChangeState(eWolfBossState::DASH);
			mPrevWolfBossState = eWolfBossState::APPEAR;
			mbAppear = false;
		}

	}


	void BigWolfScript::Disappear()
	{

		if (mDir.x > 0 && !mbDisappear)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_MoveDissappearR", false);
			mDir = Vector3(1.f, 0.f, 0.f); // disapper이랑 appear 할 떄의 기준으로 삼기 	
			mbDisappear = true;
		}

		else if (mDir.x < 0 && !mbDisappear)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_MoveDissappearL", false);
			mDir = Vector3(-1.f, 0.f, 0.f);
			mbDisappear = true;
		}

	

		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eWolfBossState::APPEAR); 

			mPrevWolfBossState = eWolfBossState::DISAPPEAR;
			mbDisappear = false;
		}



	}

	void BigWolfScript::Breath_start()
	{
		if (mDir.x > 0)// && !mbBreathStart)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_BreathAttackStartL", false);		
			mbBreathStart = true;

		}

		else if (mDir.x < 0)// && !mbBreathStart)
		{

			mAnimator->PlayAnimation(L"Boss_Wolf_BreathAttackStartR", false);
			mbBreathStart = true;

		}

		// mbBreath true는 breath 다음에 넘어가는 곳에서 true로 바꿔주기 . false는 true로 바꾼 상태에서 또 다른 상태 넘어간 곳에서 하고...

		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eWolfBossState::BREATHING);
			mPrevWolfBossState = eWolfBossState::BREATH_START;
			mbBreathStart = false;
		}		


	}

	void BigWolfScript::Breathing()
	{



		if (mDir.x > 0 && !mbBreating)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_BreathAttackingL", true);
		
			mbBreating = true;

		}

		else if (mDir.x < 0 && !mbBreating)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_BreathAttackingR", true);
			mbBreating = true;

		}

		//// ======
		m_fTime += Time::DeltaTime();

		// 몇 초 뒤에 끝낸다. 
		if (mbBreating && m_fTime >= 2.f)
		{
			mbBreating = false; 
			ChangeState(eWolfBossState::BREATH_END);
			mPrevWolfBossState = eWolfBossState::BREATHING;
			m_fTime = 0.f;

		}

	}

	void BigWolfScript::Breathing_Event()
	{
	/*	m_fTime += Time::DeltaTime();

		if (m_fTime > 4.f)
		{
			mAnimator->SetLoop(false);
		}
*/




	}

	void BigWolfScript::Breath_end()
	{

		if (mDir.x > 0)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_BreathAttackEndL", false);
			mDir = Vector3(1.f, 0.f, 0.f); // disapper이랑 appear 할 떄의 기준으로 삼기 

		}

		else if (mDir.x < 0)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_BreathAttackEndR", false);
			mDir = Vector3(-1.f, 0.f, 0.f);

		}

		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eWolfBossState::STOM_START);
			mPrevWolfBossState = eWolfBossState::BREATH_END;
			mbBreating = false;
			
		}


	}

	void BigWolfScript::Dash()
	{

		if (mDir.x > 0 && !mbDash)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_DashL", false);
			mDir = Vector3(1.f, 0.f, 0.f); // disapper이랑 appear 할 떄의 기준으로 삼기 
			mbDash = true;
	
		}

		else if (mDir.x < 0 && !mbDash)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_DashR", false);
			mDir = Vector3(-1.f, 0.f, 0.f);
			mbDash = true;

		}	

		Vector3 BossPos = mTransform->GetPosition();

		// ||로 묶어도 잘 호출됨 
		if (mAnimator->GetCurActiveAnimation()->GetIndex() == 7
			|| mAnimator->GetCurActiveAnimation()->GetIndex() == 8)
		{
			float fSpeed = 2800.f; 

			if (mDir.x > 0 && BossPos.x > -310) // 오른쪽 끝에 도달하면 멈춤
			{
				mRigidbody->SetVelocity(Vector2(-fSpeed, 0));

			}

			else if (mDir.x < 0 && BossPos.x < 320) // 왼쪽 끝에 도달하면 멈춤)
			{
				mRigidbody->SetVelocity(Vector2(fSpeed, 0));
	
			}
		}

		else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 9)
		{
			mRigidbody->SetVelocity(Vector2(0, 0));

			if (mDir.x > 0)
			{
				mDir = Vector3(-1.f, 0.f, 0.f);
			}
		
			else if (mDir.x < 0)
			{
				mDir = Vector3(1.f, 0.f, 0.f);
			}

		}

		mTransform->SetPosition(BossPos);

	

		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eWolfBossState::IDLE);
			mPrevWolfBossState = eWolfBossState::DASH;
			mbDash = false;

		}



	}


	void BigWolfScript::Howling_start()
	{
		if (mDir.x > 0)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_HowlingStartR", false);
			mDir = Vector3(1.f, 0.f, 0.f); // disapper이랑 appear 할 떄의 기준으로 삼기 


		}

		else if (mDir.x < 0)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_HowlingStartL", false);
			mDir = Vector3(-1.f, 0.f, 0.f);
	

		}

		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eWolfBossState::HOWLING);
			mPrevWolfBossState = eWolfBossState::HOWLING_START;
		}


	}

	void BigWolfScript::Howling()
	{


		if (mDir.x > 0 && !mbHowling)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_HowlingR", true);
			mDir = Vector3(1.f, 0.f, 0.f); // disapper이랑 appear 할 떄의 기준으로 삼기 


		}

		else if (mDir.x < 0 && !mbHowling)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_HowlingL", true);
			mDir = Vector3(-1.f, 0.f, 0.f);


		}


		if (!mbHowling && mAnimator->GetCurActiveAnimation()->GetIndex() == 0)
		{
			mHitGround = object::Instantiate<Effect>(eLayerType::Effect, L"HowlingHitGroundObj");
			HitGroundScript* script = mHitGround->AddComponent<HitGroundScript>();
			script->SetMonsterOwner((Monster*)mTransform->GetOwner());
			mbHowling = true; // false는 hitground에서 해줌 

		}



		mHitGround->SetEffectOwner(mTransform->GetOwner());

		Vector3 WolfPos = mTransform->GetPosition();
		mHitGround->GetComponent<Transform>()->SetPosition(WolfPos.x, -260.f, 500.f);



		m_fTime += Time::DeltaTime();

		// 몇 초 뒤에 끝낸다. 
		if (m_fTime >= 5.f && mbHowling)
		{

			ChangeState(eWolfBossState::HOWLING_END);
			mPrevWolfBossState = eWolfBossState::HOWLING;
			m_fTime = 0.f;
		

		}

	}

	void BigWolfScript::Howling_end()
	{
		if (mDir.x > 0)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_HowlingEndR", false);
			mDir = Vector3(1.f, 0.f, 0.f); // disapper이랑 appear 할 떄의 기준으로 삼기 


		}

		else if (mDir.x < 0)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_HowlingEndL", false);
			mDir = Vector3(-1.f, 0.f, 0.f);


		}

		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eWolfBossState::IDLE);
			mPrevWolfBossState = eWolfBossState::HOWLING_END;
		}


	}

	void BigWolfScript::Stom_start()
	{

		if (mPrevWolfBossState == eWolfBossState::BREATH_END)
		{
			if (mDir.x > 0 && !mbStomStart)
			{
				mAnimator->PlayAnimation(L"Boss_Wolf_StormStartL", false);
				mbStomStart = true;
			}

			else if (mDir.x < 0 && !mbStomStart)
			{
				mAnimator->PlayAnimation(L"Boss_Wolf_StormStartR", false);
				mbStomStart = true;
			}
		}

		else
		{
			if (mCurDir.x > 0 && !mbStomStart)
			{
				mAnimator->PlayAnimation(L"Boss_Wolf_StormStartR", false);
				mbStomStart = true;
			}

			else if (mCurDir.x < 0 && !mbStomStart)
			{
				mAnimator->PlayAnimation(L"Boss_Wolf_StormStartL", false);
				mbStomStart = true;
			}

		}

			if (mAnimator->GetCurActiveAnimation()->IsComplete())
			{
				ChangeState(eWolfBossState::STOMING);
				mPrevWolfBossState = eWolfBossState::STOM_START;
				mbStomStart = false;
				
			}


	}
	void BigWolfScript::Stoming()
	{


		if (miStomCount == 0)
		{

			// 몇 초동안 플레이어의 위치를 실시간으로 따라다니다가 착지한다. 
			Vector3 playerPos = mPlayer->GetComponent<Transform>()->GetPosition();

			if (!mbStoming)
			{
				mHitGround = object::Instantiate<Effect>(playerPos, eLayerType::Effect, L"StomingHitGroundObj");
				HitGroundScript* script = mHitGround->AddComponent<HitGroundScript>();
				script->SetMonsterOwner((Monster*)mTransform->GetOwner());
				mbStoming = true;
			}

			mHitGround->SetEffectOwner(mTransform->GetOwner());

			// 3초가 지나면 상태를 바꾼다. 
			m_fTime += Time::DeltaTime();


			// mHitGround가 null이 아니면, 객체의 위치만 실시간 업데이트
			if (m_fTime < 3.f)
			{
				mHitGround->GetComponent<Transform>()->SetPosition(playerPos);
				mLandingPos = playerPos; // 마지막 값만 저장해둔다. 
			}

		}

		else if (miStomCount == 1)
		{
			if (miStomCount == 2)
				return;

			if (!mbStoming)
			{
				mHitGround = object::Instantiate<Effect>(eLayerType::Effect, L"StomingHitGroundObj2");
				HitGroundScript* script = mHitGround->AddComponent<HitGroundScript>();
				script->SetMonsterOwner((Monster*)mTransform->GetOwner());
				mbStoming = true; // false처리는 hitGroundScrip에서 해주고 있음

			}

			mHitGround->SetEffectOwner(mTransform->GetOwner());

			mHitGround->GetComponent<Transform>()->SetPosition(0.f, 0.f, 500.f);

			// 3초가 지나면 상태를 바꾼다. 
			m_fTime += Time::DeltaTime();

		}

		if (m_fTime > 3.5f)
		{
			//mHitGround->SetState(GameObject::eState::Dead);
			ChangeState(eWolfBossState::STOM_END);
			mPrevWolfBossState = eWolfBossState::STOMING;
		

			m_fTime = 0.f; // 3초 되고나서 안에서 초기화버리면 여기 if문에 못 오므로 이렇게 해준다. 

		}




	}
	void BigWolfScript::Stom_end()
	{
	
		if (mCurDir.x > 0 && !mbStomEnd)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_StormLandingR", false);
			mbStomEnd = true;
		
			
		}

		else if (mCurDir.x < 0 && !mbStomEnd)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_StormLandingL", false);
			mbStomEnd = true;
		}


		// 처음때만 실시간 위치 반영되게 해둠 
		if (miStomCount == 0)
		{
			mTransform->SetPosition(Vector3(mLandingPos.x, -183.f, 500.f)); // 나머지는 보스 위치 그대로 가져다 씀 
		}

		else if (miStomCount == 1)
		{			
			miStomCount = 0; // 0으로 초기화
			mTransform->SetPosition(Vector3(10.f, -183.f, 500.f)); // 나머지는 보스 위치 그대로 가져다 씀 

		}

		mPrevWolfBossState = eWolfBossState::STOM_END;
		


		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eWolfBossState::IDLE);

			mPrevWolfBossState = eWolfBossState::STOM_END;

			++miStomCount;
			mbStomEnd = false;
		}
	



	}

	void BigWolfScript::Dead()
	{
	}
	void BigWolfScript::Animation()
	{
	}

}