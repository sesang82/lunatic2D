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

namespace ss
{
	BigWolfScript::BigWolfScript()
		: mbStomStart(false)
		, mbStomEnd(false)
		, mLandingPos(Vector3::Zero)
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

		Transform* tr = GetOwner()->GetComponent<Transform>();
		tr->SetScale(272.f, 271.f, 1.f);



		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Idle");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Spawn");
		std::shared_ptr<ss::graphics::Texture> Image3 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_BreathAttack");
		std::shared_ptr<ss::graphics::Texture> Image4 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_BreathEffectStart");
		std::shared_ptr<ss::graphics::Texture> Image5 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_BreathEffectEnd");
		std::shared_ptr<ss::graphics::Texture> Image6 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Dash");
		std::shared_ptr<ss::graphics::Texture> Image7 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Die");
		std::shared_ptr<ss::graphics::Texture> Image8 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Hit");
		std::shared_ptr<ss::graphics::Texture> Image9 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Howling");
		std::shared_ptr<ss::graphics::Texture> Image10 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_HowlingEffect");
		std::shared_ptr<ss::graphics::Texture> Image11 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_HowlingPostioin");
		std::shared_ptr<ss::graphics::Texture> Image12 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_MoveAppear");
		std::shared_ptr<ss::graphics::Texture> Image13 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_MoveDissappear");
		std::shared_ptr<ss::graphics::Texture> Image14 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Shadow");
		std::shared_ptr<ss::graphics::Texture> Image15 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_ShadowEffect");
		std::shared_ptr<ss::graphics::Texture> Image16 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Storm1");
		std::shared_ptr<ss::graphics::Texture> Image17 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Storm2");


		mAnimator->Create(L"Boss_Wolf_IdleR", Image1, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 9, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_IdleL", Image1, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 9, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);

		//mAnimator->Create(L"Boss_Wolf_SpawnR", Image2, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 12, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_SpawnL", Image2, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 12, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Boss_Wolf_BreathAttackR", Image3, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 14, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_BreathAttackL", Image3, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 14, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);

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

		// �Ͽ︵ �� ������ �ε��� ���� �̻��� 
		mAnimator->Create(L"Boss_Wolf_HowlingR", Image9, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 28, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_HowlingL", Image9, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 28, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);


		//mAnimator->Create(L"Boss_Wolf_HowlingEffectR", Image10, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 5, Vector2(272.f, 271.f));
		//mAnimator->Create(L"Boss_Wolf_HowlingEffectL", Image10, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 5, Vector2(272.f, 271.f), Vector2(-16.f, 0.f), 0.1f, true);

		//mAnimator->Create(L"Boss_Wolf_HowlingPostioinR", Image11, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 5, Vector2(272.f, 271.f));
		//mAnimator->Create(L"Boss_Wolf_HowlingPostioinL", Image11, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 5, Vector2(272.f, 271.f), Vector2(-16.f, 0.f), 0.1f, true);
		//
		mAnimator->Create(L"Boss_Wolf_MoveAppearR", Image12, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 12, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_MoveAppearL", Image12, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 12, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);
		
		mAnimator->Create(L"Boss_Wolf_MoveDissappearR", Image13, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 10, Vector2(272.f, 271.f));
		mAnimator->Create(L"Boss_Wolf_MoveDissappearL", Image13, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 10, Vector2(272.f, 271.f), Vector2(6.f, 0.f), 0.1f, true);
	
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
	// �ִϸ��̼� ���⿡ ���� ����	�� ����ش�.


		mDir = -mTransform->Right();



		// ������ �ʱ� ���´� �ο������� �ʴ´�. (������) 
		

		// ===== �⺻ �浹ü Hit �޴� �뵵 
		mCollider->SetName(L"BigWolf_HitCol");
		mCollider->SetType(eColliderType::Rect);
		mCollider->SetSize(Vector2(0.6f, 0.36f));
		mCollider->SetCenter(Vector2(-4.f, -84.f));

		// ===== ������ٵ�



		//==== ���� ���� Ư�� �ε��� �浹ü 
		//�浹ü�� ���⼭ �ٷ� ���� ���� �ش� �ε��� �� �־��ٰ� ���� ������ �ϱ� 
		mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"BigWolf_AttackColObj");
		mAttackColliderObj->Initialize();
		//mAttackColliderObj->AddComponent<LizardColScript>();

		mAttackColTr = mAttackColliderObj->GetComponent<Transform>();


		// ==== �޽� ����
		mMeshRenderer->SetMaterial(Resources::Find<Material>(L"tempMtrl"));

	}
	void BigWolfScript::Update()
	{

		Vector3 MonsterPos = mTransform->GetPosition();
		Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();

		mCurDir = (PlayerPos - MonsterPos);
		mCurDir.Normalize();
		mCurDir = Vector3(mCurDir.x, mCurDir.y, 0.f); 


		// �̵�->���º�ȯ->�ִϸ��̼�

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

		case ss::eWolfBossState::BREATH:
			Breath();
			break;

		case ss::eWolfBossState::HOWLING:
			Howling();
			break;
	
		case ss::eWolfBossState::DASH:
			Dash();
			break;

		case ss::eWolfBossState::STOM_START:
			Stom_start();
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
	void BigWolfScript::Idle()
	{
		mbStomStart = false;
		mbStomEnd = false;


		mbHit = false;

		if (mCurDir.x > 0)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_IdleR", true);
			mDir = Vector3(1.f, 0.f, 0.f); // disapper�̶� appear �� ���� �������� ��� 
		}

		else
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_IdleL", true);
			mDir = Vector3(-1.f, 0.f, 0.f);
		}

	}

	void BigWolfScript::Hit()
	{
	}
	void BigWolfScript::Appear()
	{
	}
	void BigWolfScript::Disappear()
	{
	}
	void BigWolfScript::Breath()
	{
	}
	void BigWolfScript::Howling()
	{
	}
	void BigWolfScript::Dash()
	{
	}
	void BigWolfScript::Stom_start()
	{

			if (mCurDir.x > 0 && !mbStomStart)
			{
				mAnimator->PlayAnimation(L"Boss_Wolf_StormStartR", false);
			}

			else if (mCurDir.x < 0 && !mbStomStart)
			{
				mAnimator->PlayAnimation(L"Boss_Wolf_StormStartL", false);
			}

	

			if (mAnimator->GetCurActiveAnimation()->IsComplete())
			{
				Stoming();
			}

		


	}
	void BigWolfScript::Stoming()
	{
		mbStomStart = true; // stom �ִϸ��̼��� ���� �ٲ�� ��� ����Ǵ°� ���� 


		// �� �ʵ��� �÷��̾��� ��ġ�� �ǽð����� ����ٴϴٰ� �����Ѵ�. 
		Vector3 playerPos = mPlayer->GetComponent<Transform>()->GetPosition();

		if (nullptr == mHitGround)
		{
			mHitGround = object::Instantiate<Effect>(playerPos, eLayerType::Effect, L"StomingHitGroundObj");
			HitGroundScript* script = mHitGround->AddComponent<HitGroundScript>();
			script->SetMonsterOwner((Monster*)mTransform->GetOwner());
		}

		mHitGround->SetEffectOwner(mTransform->GetOwner());

		// 3�ʰ� ������ ���¸� �ٲ۴�. 
		m_fTime += Time::DeltaTime();

		// mHitGround�� null�� �ƴϸ�, ��ü�� ��ġ�� �ǽð� ������Ʈ
		if (m_fTime < 3.f)
		{
			mHitGround->GetComponent<Transform>()->SetPosition(playerPos);
			mLandingPos = playerPos; // ������ ���� �����صд�. 
		}


		else if (m_fTime > 3.5f)
		{
			//mHitGround->SetState(GameObject::eState::Dead);
			ChangeState(eWolfBossState::STOM_END);

			m_fTime = 0.f; // 3�� �ǰ��� �ȿ��� �ʱ�ȭ������ ���� if���� �� ���Ƿ� �̷��� ���ش�. 

		}

	}
	void BigWolfScript::Stom_end()
	{
	
		if (mCurDir.x > 0 && !mbStomEnd)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_StormLandingR", false);
			
		}

		else if (mCurDir.x < 0 && !mbStomEnd)
		{
			mAnimator->PlayAnimation(L"Boss_Wolf_StormLandingL", false);
		}

		mbStomEnd = true;
		mTransform->SetPosition(Vector3(mLandingPos.x, -183.f, 500.f)); // �������� ���� ��ġ �״�� ������ �� 
		
		


		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eWolfBossState::IDLE);
		}
	



	}

	void BigWolfScript::Dead()
	{
	}
	void BigWolfScript::Animation()
	{
	}

}