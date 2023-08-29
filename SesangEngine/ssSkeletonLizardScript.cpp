#include "ssSkeletonLizardScript.h"
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
#include "ssArcherNearRangeScript.h"
#include "ssArcherFarRangeScript.h"
#include "ssPlayerScript.h"
#include "ssAttackCollider.h"
#include "ssArcherColScript.h"
#include "ssArcherArrowScript.h"
#include "ssArcherArrow.h"
#include "ssArcherFarRangeScript.h"
#include "ssLizardColScript.h"

namespace ss
{

	// ====== 2023. 08 30 
	// 1. 근접 공격 충돌체 지우기
	// 2. 충돌체 크기 정하고 애니메이션 오프셋 위치 조정하기



	SkeletonLizardScript::SkeletonLizardScript()
		: mbNearAttack(false)
	{
		m_tMonsterInfo.m_fSpeed = 100.f;
		m_tMonsterInfo.m_fNearAttackRange = 25.f;
		m_tMonsterInfo.m_fDetectRange = 180.f;
		m_tMonsterInfo.m_fCoolDown = 0.1f;
	}
	SkeletonLizardScript::~SkeletonLizardScript()
	{
	}
	void SkeletonLizardScript::Initialize()
	{
		MonsterScript::Initialize();

		mTransform->SetScale(Vector3(96.f, 48.f, 1.f)); // backsize랑 스케일값 동일하게 주기. 


		//
		mCharacterState->SetCurrentHP(110.f);
		mCharacterState->SetMaxHP(110.f);

		mFirstPos = GetOwner()->GetComponent<Transform>()->GetPosition();

		//
		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Lizard_Idle");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Lizard_Run");
		std::shared_ptr<ss::graphics::Texture> Image3 = Resources::Find<ss::graphics::Texture>(L"Lizard_NearAttack");
		std::shared_ptr<ss::graphics::Texture> Image4 = Resources::Find<ss::graphics::Texture>(L"Lizard_Hit");
		std::shared_ptr<ss::graphics::Texture> Image5 = Resources::Find<ss::graphics::Texture>(L"Lizard_Stun");
		std::shared_ptr<ss::graphics::Texture> Image6 = Resources::Find<ss::graphics::Texture>(L"Lizard_Die");


		// 플레이어 애니메이션은 좀 특수하므로, 무기 별로 오프셋 값 다르게 주되 백사이즈는	 동일하게 주기. (사이즈 값은 틀려도 됨) 
		// 애니메이션 나중에 플레이어 다 완성되면 
		//  LT, 1프레임 사를 사이즈, 자를 갯수, 오프셋, , 재생시간 뒤집기
		// === 
		// 플레이어 애니메이션은 좀 특수하므로, 무기 별로 오프셋 값 다르게 주되 백사이즈는	 동일하게 주기. (사이즈 값은 틀려도 됨) 
		// 애니메이션 나중에 플레이어 다 완성되면 
		//  LT, 1프레임 사를 사이즈, 자를 갯수(1부터 시작해서 세기), 백사이즈, 오프셋
		mAnimator->Create(L"Lizard_IdleR", Image1, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 7, Vector2(96.f, 48.f));
		mAnimator->Create(L"Lizard_IdleL", Image1, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 7, Vector2(96.f, 48.f), Vector2(33.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Lizard_RunR", Image2, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 8, Vector2(96.f, 48.f));
		mAnimator->Create(L"Lizard_RunL", Image2, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 8, Vector2(96.f, 48.f), Vector2(33.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Lizard_HitR", Image4, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 1, Vector2(96.f, 48.f), Vector2::Zero, 0.2f);
		mAnimator->Create(L"Lizard_HitL", Image4, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 1, Vector2(96.f, 48.f), Vector2(33.f, 0.f), 0.2f, true);

		mAnimator->Create(L"Lizard_NearAttackR", Image3, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 8, Vector2(96.f, 48.f), Vector2::Zero, 0.08f);
		mAnimator->Create(L"Lizard_NearAttackL", Image3, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 8, Vector2(96.f, 48.f), Vector2(33.f, 0.f), 0.08f, true);

		mAnimator->Create(L"Lizard_StunR", Image5, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 7, Vector2(96.f, 48.f));
		mAnimator->Create(L"Lizard_StunL", Image5, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 7, Vector2(96.f, 48.f), Vector2(33.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Lizard_DieR", Image6, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 12, Vector2(96.f, 48.f));
		mAnimator->Create(L"Lizard_DieL", Image6, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 12, Vector2(96.f, 48.f), Vector2(33.f, 0.f), 0.1f, true);

		// 일단 Idle 상태는 나중으로 만들기 
		// ======
			// 애니메이션 방향에 관한 기준	을 잡아준다.
		mDir = mTransform->Right();
		mCurDir = mTransform->Right();
		mCurState = eMonsterState::IDLE;

		// ===== 기본 충돌체 Hit 받는 용도 
		mCollider->SetName(L"lizard_HitCol");
		mCollider->SetType(eColliderType::Rect);
		
		mCollider->SetSize(Vector2(0.2f, 0.7f));
		mCollider->SetCenter(Vector2(-17.f, -0.f));


		//==== 근접 공격 특정 인덱스 충돌체 
		//충돌체는 여기서 바로 넣지 말고 해당 인덱스 때 넣었다가 빼는 식으로 하기 
		mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"LizardAttackColObj");
		mAttackColliderObj->Initialize();
		mAttackColliderObj->AddComponent<LizardColScript>();

		mAttackColTr = mAttackColliderObj->GetComponent<Transform>();
	}
	void SkeletonLizardScript::Update()
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

		case ss::eMonsterState::HIT_AFTER:
			HitAfter();
			break;
			
		case ss::eMonsterState::NEARATTACK:
			NearAttack();
			break;

		case ss::eMonsterState::NEARATTACK_AFTER:
			NearAttackAfter();
			break;


		case ss::eMonsterState::DEAD:
			Dead();
			break;
		}


		mPrevState = mCurState;
		mPrevDir = mCurDir;
	}
	void SkeletonLizardScript::LateUpdate()
	{
		mAttackColTr->SetPosition(mTransform->GetPosition());
	}
	void SkeletonLizardScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void SkeletonLizardScript::OnCollisionStay(Collider2D* other)
	{
	}
	void SkeletonLizardScript::OnCollisionExit(Collider2D* other)
	{
	}
	void SkeletonLizardScript::FarAttackEnd()
	{
	}
	void SkeletonLizardScript::StunEnd()
	{
	}
	void SkeletonLizardScript::Idle()
	{
		mbNearAttacking = false;
	
		mbHit = false;

		if (mCurDir.x > 0)
		{
			mAnimator->PlayAnimation(L"Lizard_IdleR", true);
		}

		else
		{
			mAnimator->PlayAnimation(L"Lizard_IdleL", true);
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
	}
	void SkeletonLizardScript::Move()
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
			return; // 이 함수에서 추가적인 처리를 중지합니다.
		}


		// ======================= tracer

		// PLAYER가 탐지 범위 이내에 들어오면 추적 상태로 전환 
		if (vDir.Length() < m_tMonsterInfo.m_fDetectRange)
		{

			if (mCurDir.x > 0)
			{
				mAnimator->PlayAnimation(L"Lizard_RunR", true);
				mCollider->SetSize(Vector2(0.2f, 0.7f));
				mCollider->SetCenter(Vector2(-8.f, 0.f));
			}

			else
			{
				mAnimator->PlayAnimation(L"Lizard_RunL", true);
				mCollider->SetSize(Vector2(0.2f, 0.7f));
				mCollider->SetCenter(Vector2(-8.f, 0.f));
			}


			Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
			Vector3 MonsterPos = mTransform->GetPosition();

			// 몬스터에서 플레이어까지의 거리를 구한다. 
			Vector3 vMonToPlayer = PlayerPos - MonsterPos; // 왜 여긴 플레이어부터 빼는지 gpt에게 물어보기
			vMonToPlayer.z = 0;
			vMonToPlayer.Normalize();

			float fSpeed = m_tMonsterInfo.m_fSpeed + 200.f; // 추적 중일 땐 더 빠르게 추적하게 함 

			// 몬스터의 위치를 플레이어 방향으로 이동시킨다.
			MonsterPos.x += vMonToPlayer.x * fSpeed * Time::DeltaTime();

			mTransform->SetPosition(MonsterPos);


		}

		// 탐지 거리를 벗어나면 초기 위치로 돌아간다. 
		else if (vDir.Length() > m_tMonsterInfo.m_fDetectRange)
		{

			if (mDir.x > 0)
			{
				mAnimator->PlayAnimation(L"Lizard_RunR", true);
				mCollider->SetSize(Vector2(0.2f, 0.7f));
				mCollider->SetCenter(Vector2(-8.f, -0.f));
			}

			else
			{
				mAnimator->PlayAnimation(L"Lizard_RunL", true);
				mCollider->SetSize(Vector2(0.2f, 0.7f));
				mCollider->SetCenter(Vector2(-8.f, -0.f));
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


	}
	void SkeletonLizardScript::Tracer()
	{
	
	}
	void SkeletonLizardScript::Jump()
	{
	}
	void SkeletonLizardScript::Fall()
	{
	}
	void SkeletonLizardScript::Landing()
	{
	}
	void SkeletonLizardScript::Stun()
	{
	}

	void SkeletonLizardScript::Hit()
	{
		if (!mbHit)
		{
			mbHit = true;

			if (mCurDir.x > 0)
			{
				mAnimator->PlayAnimation(L"Lizard_HitR", false);
			}

			else
			{
				mAnimator->PlayAnimation(L"Lizard_HitL", false);
			}
		}

		if (mbHit && mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eMonsterState::IDLE);
		}


	}



	void SkeletonLizardScript::NearAttack()
	{
			Vector3 MonsterPos = mTransform->GetPosition();
			Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
			float distance = (PlayerPos - MonsterPos).Length();

			PlayerScript* playerScript = mPlayer->GetComponent<PlayerScript>();





			if (mCurDir.x > 0)
			{
				if (mAnimator->GetCurActiveAnimation()->GetIndex() == 7)
				{
					mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();

					mAttackCol->SetSize(Vector2(23.f, 30.f));
					mAttackCol->SetCenter(Vector2(20.f, -5.f));

					mbNearAttacking = false;

					// 대쉬 중엔 아예 충돌 안되게 해버림 
					if (playerScript->IsDash())
					{
						mAttackColliderObj->RemoveComponent<Collider2D>();
					}
				}
			}


			else if (mCurDir.x < 0)
			{
				if (mAnimator->GetCurActiveAnimation()->GetIndex() == 7)
				{
					mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();

					mAttackCol->SetSize(Vector2(23.f, 30.f));
					mAttackCol->SetCenter(Vector2(20.f, -5.f));

					mbNearAttacking = false;

					// 대쉬 중엔 아예 충돌 안되게 해버림 
					if (playerScript->IsDash())
					{
						mAttackColliderObj->RemoveComponent<Collider2D>();
					}
				}
			}

			m_fTime += Time::DeltaTime();

			if (m_fTime >= m_tMonsterInfo.m_fCoolDown && !mbNearAttacking)
			{
				mbNearAttacking = true;


				if (mCurDir.x > 0)
				{
					mAnimator->PlayAnimation(L"Lizard_NearAttackR", false);
					mCollider->SetSize(Vector2(0.2f, 0.7f));
					mCollider->SetCenter(Vector2(-8.f, 0.f));

				}

				else
				{
					mAnimator->PlayAnimation(L"Lizard_NearAttackL", false);
					mCollider->SetSize(Vector2(0.2f, 0.7f));
					mCollider->SetCenter(Vector2(-8.f, 0.f));
				}


				m_fTime = 0.0f;
			}



			if (!mbNearAttacking && mAnimator->GetCurActiveAnimation()->IsComplete())
			{
				ChangeState(eMonsterState::IDLE);
			}


		

	}

	

	void SkeletonLizardScript::Dead()
	{
		if (mCurDir.x > 0)
		{
			mAnimator->PlayAnimation(L"Lizard_DieR", false);
			mCollider->SetSize(Vector2(0.19f, 0.33f));
			mCollider->SetCenter(Vector2(-32.f, 0.f));

		}

		else
		{
			mAnimator->PlayAnimation(L"Lizard_DieL", false);
			mCollider->SetSize(Vector2(0.19f, 0.33f));
			mCollider->SetCenter(Vector2(-37.f, 0.f));
		}

		// 애니메이션 재생이 끝나면 
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			mAttackColliderObj->SetState(GameObject::eState::Dead);
		

			GetOwner()->SetState(GameObject::eState::Dead);
		}
	}
	void SkeletonLizardScript::Animation()
	{
	}
}