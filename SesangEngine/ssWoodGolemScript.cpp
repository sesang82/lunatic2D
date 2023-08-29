#include "ssWoodGolemScript.h"
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
	WoodGolemScript::WoodGolemScript()
		: mbNearAttacking(false)
		, mbFarAttacking(false)
		, mbHit(false)
		, mbPaunched(false)
	{
		m_tMonsterInfo.m_fSpeed = 50.f;
		m_tMonsterInfo.m_fNearAttackRange = 110.f;
		m_tMonsterInfo.m_fFarAttackRange = 150.f;
		m_tMonsterInfo.m_fCoolDown = 0.5f;
	}
	WoodGolemScript::~WoodGolemScript()
	{
	}
	void WoodGolemScript::Initialize()
	{
		MonsterScript::Initialize();

		mTransform->SetScale(Vector3(111.f, 83.f, 1.f)); // backsize랑 스케일값 동일하게 주기. 


		//
		mCharacterState->SetCurrentHP(110.f);
		mCharacterState->SetMaxHP(110.f);

		mFirstPos = GetOwner()->GetComponent<Transform>()->GetPosition();

		//
		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Wood_Idle");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Wood_Run");
		std::shared_ptr<ss::graphics::Texture> Image3 = Resources::Find<ss::graphics::Texture>(L"Wood_NearAttack");
		std::shared_ptr<ss::graphics::Texture> Image4 = Resources::Find<ss::graphics::Texture>(L"Wood_FarAttack");
		std::shared_ptr<ss::graphics::Texture> Image5 = Resources::Find<ss::graphics::Texture>(L"Wood_Hit");
		std::shared_ptr<ss::graphics::Texture> Image6 = Resources::Find<ss::graphics::Texture>(L"Wood_Stun");
		std::shared_ptr<ss::graphics::Texture> Image7 = Resources::Find<ss::graphics::Texture>(L"Wood_Die");


		// 플레이어 애니메이션은 좀 특수하므로, 무기 별로 오프셋 값 다르게 주되 백사이즈는	 동일하게 주기. (사이즈 값은 틀려도 됨) 
		// 애니메이션 나중에 플레이어 다 완성되면 
		//  LT, 1프레임 사를 사이즈, 자를 갯수, 오프셋, , 재생시간 뒤집기
		// === 
		// 플레이어 애니메이션은 좀 특수하므로, 무기 별로 오프셋 값 다르게 주되 백사이즈는	 동일하게 주기. (사이즈 값은 틀려도 됨) 
		// 애니메이션 나중에 플레이어 다 완성되면 
		//  LT, 1프레임 사를 사이즈, 자를 갯수(1부터 시작해서 세기), 백사이즈, 오프셋
		mAnimator->Create(L"Wood_IdleR", Image1, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 5, Vector2(111.f, 83.f));
		mAnimator->Create(L"Wood_IdleL", Image1, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 5, Vector2(111.f, 83.f), Vector2(23.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Wood_RunR", Image2, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 10, Vector2(111.f, 83.f));
		mAnimator->Create(L"Wood_RunL", Image2, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 10, Vector2(111.f, 83.f), Vector2(23.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Wood_NearAttackR", Image3, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 13, Vector2(111.f, 83.f), Vector2::Zero, 0.08f);
		mAnimator->Create(L"Wood_NearAttackL", Image3, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 13, Vector2(111.f, 83.f), Vector2(23.f, 0.f), 0.08f, true);

		mAnimator->Create(L"Wood_FarAttackR", Image4, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 13, Vector2(111.f, 83.f));
		mAnimator->Create(L"Wood_FarAttackL", Image4, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 13, Vector2(111.f, 83.f), Vector2(23.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Wood_HitR", Image5, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 2, Vector2(111.f, 83.f));
		mAnimator->Create(L"Wood_HitL", Image5, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 2, Vector2(111.f, 83.f), Vector2(23.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Wood_StunR", Image6, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 4, Vector2(111.f, 83.f));
		mAnimator->Create(L"Wood_StunL", Image6, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 4, Vector2(111.f, 83.f), Vector2(23.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Wood_DieR", Image7, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 9, Vector2(111.f, 83.f));
		mAnimator->Create(L"Wood_DieL", Image7, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 9, Vector2(111.f, 83.f), Vector2(5.f, 0.f), 0.1f, true);

		// ======
			// 애니메이션 방향에 관한 기준	을 잡아준다.
		mDir = mTransform->Right();
		mCurDir = mTransform->Right();
		mCurState = eMonsterState::MOVE;

		// ===== 기본 충돌체 Hit 받는 용도 
		mCollider->SetName(L"Wood_HitCol");
		mCollider->SetType(eColliderType::Rect);
		mCollider->SetSize(Vector2(0.3f, 0.7f));
		mCollider->SetCenter(Vector2(-12.f, 0.f));


		//==== 근접 공격 특정 인덱스 충돌체 
		//충돌체는 여기서 바로 넣지 말고 해당 인덱스 때 넣었다가 빼는 식으로 하기 
		mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"WoodAttackColObj");
		mAttackColliderObj->Initialize();
		//mAttackColliderObj->AddComponent<LizardColScript>();

		mAttackColTr = mAttackColliderObj->GetComponent<Transform>();


	}
	void WoodGolemScript::Update()
	{
		// 이동->상태변환->애니메이션

		Transform* playerTr = mPlayer->GetComponent<Transform>();
		Vector3 playerPos = playerTr->GetPosition();


		// 공격 중일 때는 위치 고정시키기 위해서 (몬스터가 플레이어의 위치가 변경되면 애니메이션이 리셋되는 현상 방지)
			// 방향의 기준을 잡아준다. (몬스터의 위치 값보다 X값이 크면 오른쪽이므로  1, 왼쪽에 있으면 -1)
		if (playerPos.x >= mTransform->GetPosition().x)
		{
			mCurDir.x = 1.0f;
		}
		else
		{
			mCurDir.x = -1.0f;
		}



		switch (mCurState)
		{
		case ss::eMonsterState::IDLE:
			Idle();
			break;

		case ss::eMonsterState::MOVE:
			Move();
			break;

		case ss::eMonsterState::TRACER:
			Tracer();
			break;

		case ss::eMonsterState::JUMP:
			Jump();
			break;

		case ss::eMonsterState::FALL:
			Fall();
			break;

		case ss::eMonsterState::STUN:
			Stun();
			break;

		case ss::eMonsterState::HIT:
			Hit();
			break;

		case ss::eMonsterState::NEARATTACK:
			NearAttack();
			break;

		case ss::eMonsterState::FARATTACK:
			FarAttack();
			break;

		case ss::eMonsterState::DEAD:
			Dead();
			break;
		}


		mPrevState = mCurState;
		mPrevDir = mCurDir;
	}
	void WoodGolemScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void WoodGolemScript::OnCollisionStay(Collider2D* other)
	{
	}
	void WoodGolemScript::OnCollisionExit(Collider2D* other)
	{
	}

	void WoodGolemScript::Idle()
	{
		mbNearAttacking = false;
		mbFarAttacking = false;
		mbHit = false;

		if (mCurDir.x > 0)
		{
			mAnimator->PlayAnimation(L"Wood_IdleR", true);
		}

		else
		{
			mAnimator->PlayAnimation(L"Wood_IdleL", true);
		}



		Vector3 MonsterPos = mTransform->GetPosition();
		Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();

		// 몬스터와 플레이어 간의 거리를 구함 
		Vector3 vDir = MonsterPos - PlayerPos;
		vDir.z = 0;
		float distance = vDir.Length();

		if (distance <= m_tMonsterInfo.m_fDetectRange)
		{
			// 플레이어가 탐지 범위 내에 있지만 근접 공격 범위 밖에 있으면 이동 상태로 전환
			ChangeState(eMonsterState::MOVE);
		}


		// 근접 공격 범위 내에 플레이어가 있으면 NearAttack 상태로 전환
		else if (distance < m_tMonsterInfo.m_fNearAttackRange)
		{
			ChangeState(eMonsterState::NEARATTACK);
		}


		// 먼거리 공격 범위 내에 플레이어가 있으면 FarAttack 상태로 전환
		else if (distance < m_tMonsterInfo.m_fFarAttackRange)
		{
			ChangeState(eMonsterState::FARATTACK);
		}

	}

	// 골렘은 추적 상태 없음 
	void WoodGolemScript::Move()
	{



		Vector3 MonsterPos = mTransform->GetPosition();
		Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();

		// 몬스터와 플레이어 간의 거리를 구함 
		Vector3 vDir = MonsterPos - PlayerPos;
		vDir.z = 0;
		float distance = vDir.Length();




		// 근접 공격 범위 내에 플레이어가 있으면 NearAttack 상태로 전환
		if (distance < m_tMonsterInfo.m_fNearAttackRange)
		{
			ChangeState(eMonsterState::NEARATTACK);
			return; // 밑에 run관련 작동 안되도록 
		}


		// 먼거리 공격 범위 내에 플레이어가 있으면 FarAttack 상태로 전환
		else if (distance < m_tMonsterInfo.m_fFarAttackRange)
		{
			ChangeState(eMonsterState::FARATTACK);
			return;
		}


			if (mDir.x > 0)
			{
				mAnimator->PlayAnimation(L"Wood_RunR", true);	
			}

			else
			{
				mAnimator->PlayAnimation(L"Wood_RunL", true);
			}




			// 몬스터가 초기 위치로부터 min~max값 까지만 움직이게 한다. 
			float minX = mFirstPos.x - 40.f;
			float maxX = mFirstPos.x + 40.f;



			MonsterPos.x += mDir.x * m_tMonsterInfo.m_fSpeed * Time::DeltaTime();

			if (MonsterPos.x < minX)
			{
				MonsterPos.x = minX;
				mDir = mTransform->Right();
			}

			else if (MonsterPos.x > maxX)
			{
				MonsterPos.x = maxX;
				mDir = -mTransform->Right();
			}

			mTransform->SetPosition(MonsterPos);


	}
	void WoodGolemScript::Tracer()
	{
	}
	void WoodGolemScript::Jump()
	{
	}
	void WoodGolemScript::Fall()
	{
	}
	void WoodGolemScript::Landing()
	{
	}
	void WoodGolemScript::Stun()
	{
	}
	void WoodGolemScript::Hit()
	{
		if (!mbHit)
		{
			mbHit = true;

			if (mCurDir.x > 0)
			{
				mAnimator->PlayAnimation(L"Wood_HitR", false);
			}

			else
			{
				mAnimator->PlayAnimation(L"Wood_HitL", false);
			}
		}

		if (mbHit && mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eMonsterState::IDLE);
		}

	}

	void WoodGolemScript::NearAttack()
	{
		Vector3 MonsterPos = mTransform->GetPosition();
		Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
		float distance = (PlayerPos - MonsterPos).Length();

		m_fTime += Time::DeltaTime();

		
		// 연이어 공격 애니메이션 재생하지 않고, 쿨타임 시간만큼 기다렸다가 공격 


		if (m_fTime >= m_tMonsterInfo.m_fCoolDown && !mbNearAttacking)
		{
			mbNearAttacking = true;


				if (mCurDir.x > 0)
				{
					mAnimator->PlayAnimation(L"Wood_NearAttackR", false);

				}

				else
				{
					mAnimator->PlayAnimation(L"Wood_NearAttackL", false);
				}

			
			m_fTime = 0.0f;
		}


		if (mAnimator->GetCurActiveAnimation()->GetIndex() == 12)
		{
			mbNearAttacking = false;
		}


		if (!mbNearAttacking && mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eMonsterState::IDLE);
		}


	}


	void WoodGolemScript::FarAttack()
	{

		Vector3 MonsterPos = mTransform->GetPosition();
		Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
		float distance = (PlayerPos - MonsterPos).Length();

		m_fTime += Time::DeltaTime();


		// 연이어 공격 애니메이션 재생하지 않고, 쿨타임 시간만큼 기다렸다가 공격 


		if (m_fTime >= m_tMonsterInfo.m_fCoolDown && !mbFarAttacking)
		{
			mbFarAttacking = true; // 애니메이션 재생이 캐릭터가 방향을 바꿔도 끝까지 유지되어야해서 넣어준 변수 


			if (mCurDir.x > 0)
			{
				mAnimator->PlayAnimation(L"Wood_FarAttackR", false);

			}

			else
			{
				mAnimator->PlayAnimation(L"Wood_FarAttackL", false);
			}


			m_fTime = 0.0f;
		}


		bool isGround = mPlayer->GetComponent<Rigidbody2D>()->IsGround();

		if (mAnimator->GetCurActiveAnimation()->GetIndex() == 1 && !mbPaunched && isGround)
		{

			mHitGround = object::Instantiate<Effect>(PlayerPos, eLayerType::Effect, L"WoodHitGroundObj");

			HitGroundScript* script = mHitGround->AddComponent<HitGroundScript>();
			script->SetMonsterOwner((Monster*)mTransform->GetOwner());

			mbPaunched = true;
		}



		else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 6)
		{

			// 공격용 충돌체가 나간다. 플레이어 포즈에. 3 인덱스에서 띄워준 playerpos값 담아서 쓰기 



		}


		else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 12) // end event 호출 안되서 대신 씀 
		{
			mbFarAttacking = false;
		}


		if (!mbFarAttacking && mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eMonsterState::IDLE);
		}

	}
	void WoodGolemScript::Dead()
	{
	}
	void WoodGolemScript::Animation()
	{
	}
	void WoodGolemScript::FarAttackEnd()
	{
	}
	void WoodGolemScript::StunEnd()
	{
	}
}