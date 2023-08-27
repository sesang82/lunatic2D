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

namespace ss
{
	SkeletonLizardScript::SkeletonLizardScript()
		: mbAttacking(false)
		, mbNearAttack(false)
	{
		m_tMonsterInfo.m_fSpeed = 100.f;
		m_tMonsterInfo.m_fNearAttackRange = 15.f;
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
		mAnimator->Create(L"Lizard_IdleL", Image1, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 7, Vector2(96.f, 48.f), Vector2(0.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Lizard_RunR", Image2, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 8, Vector2(96.f, 48.f));
		mAnimator->Create(L"Lizard_RunL", Image2, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 8, Vector2(96.f, 48.f), Vector2(0.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Lizard_HitR", Image4, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 1, Vector2(96.f, 48.f));
		mAnimator->Create(L"Lizard_HitL", Image4, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 1, Vector2(96.f, 48.f), Vector2(0.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Lizard_NearAttackR", Image3, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 8, Vector2(96.f, 48.f));
		mAnimator->Create(L"Lizard_NearAttackL", Image3, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 8, Vector2(96.f, 48.f), Vector2(0.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Lizard_StunR", Image5, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 7, Vector2(96.f, 48.f));
		mAnimator->Create(L"Lizard_StunL", Image5, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 7, Vector2(96.f, 48.f), Vector2(0.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Lizard_DieR", Image6, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 12, Vector2(96.f, 48.f));
		mAnimator->Create(L"Lizard_DieL", Image6, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 12, Vector2(96.f, 48.f));



		mAnimator->StartEvent(L"Lizard_NearAttackR") = std::bind(&SkeletonLizardScript::NearAttackStart, this);
		mAnimator->StartEvent(L"Lizard_NearAttackL") = std::bind(&SkeletonLizardScript::NearAttackStart, this);

	//	mAnimator->EndEvent(L"Lizard_NearAttackR") = std::bind(&SkeletonLizardScript::NearAttackEnd, this);
	//	mAnimator->EndEvent(L"Lizard_NearAttackL") = std::bind(&SkeletonLizardScript::NearAttackEnd, this);

		// 일단 Idle 상태는 나중으로 만들기 
		// ======
			// 애니메이션 방향에 관한 기준	을 잡아준다.
		mDir = mTransform->Right();
		mCurDir = mTransform->Right();
		mCurState = eMonsterState::MOVE;

		// ===== 기본 충돌체 Hit 받는 용도 
		mCollider->SetName(L"lizard_HitCol");
		mCollider->SetType(eColliderType::Rect);
		
		mCollider->SetSize(Vector2(0.2f, 0.7f));
		mCollider->SetCenter(Vector2(-8.f, -0.f));
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
				mCollider->SetCenter(Vector2(-8.f, -0.f));
			}

			else
			{
				mAnimator->PlayAnimation(L"Lizard_RunL", true);
				mCollider->SetSize(Vector2(0.2f, 0.7f));
				mCollider->SetCenter(Vector2(8.f, -0.f));
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
				mCollider->SetCenter(Vector2(8.f, -0.f));
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
		//// 방향대로 애니메이션을 재생한다. 
		//if (mCurDir.x > 0)
		//	mAnimator->PlayAnimation(L"Lizard_RunR", true);

		//else
		//	mAnimator->PlayAnimation(L"Lizard_RunL", true);





		//Vector3 MonsterPos = mTransform->GetPosition();
		//Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();

		//Vector3 Dir = PlayerPos - MonsterPos;
		//float Distance = Dir.Length(); // 먼저 거리를 계산

		//Dir.z = 0;
		//Dir.Normalize(); // 정규화 (거리 계산하기전에 정규화하면 계속 1이라는 값만 들어가게 됨) 
		//MonsterPos.x += Dir.x * m_tMonsterInfo.m_fSpeed * Time::DeltaTime();

		//mTransform->SetPosition(MonsterPos);

		//float closeRangeDistance = 25.f;


		//	if (Distance <= closeRangeDistance)
		//	{
		//		ChangeState(eMonsterState::NEARATTACK);
		//	}
		//	else
		//	{
		//		ChangeState(eMonsterState::FARATTACK);
		//	}
		
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
		// 방향대로 애니메이션을 재생한다. 
		if (mCurDir.x > 0)
			mAnimator->PlayAnimation(L"Lizard_HitR", false);

		else
			mAnimator->PlayAnimation(L"Lizard_HitL", false);


		// 애니메이션 재생이 끝나면 
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			if (mbNearAttack)
			{
				mbNearAttack = false;
				mCurState = eMonsterState::NEARATTACK;
			}
		}
	}


	void SkeletonLizardScript::NearAttack()
	{





			Vector3 MonsterPos = mTransform->GetPosition();
			Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
			float distance = (PlayerPos - MonsterPos).Length();


				if (!mbAttacking && distance <= m_tMonsterInfo.m_fNearAttackRange)
				{
					mbAttacking = true; 
					mbNearAttack = true;

					if (mCurDir.x > 0)
					{
						mAnimator->PlayAnimation(L"Lizard_NearAttackR", false);
					

						mCollider->SetSize(Vector2(0.40f, 0.33f));
						mCollider->SetCenter(Vector2(-32.f, 0.f));

					}

					else
					{
						mAnimator->PlayAnimation(L"Lizard_NearAttackL", false);

						mCollider->SetSize(Vector2(0.40f, 0.33f));
						mCollider->SetCenter(Vector2(32.f, 0.f));
					}


				}


				if (mAnimator->GetCurActiveAnimation()->IsComplete())
				{
					mbAttacking = false;	
					
					mAnimator->SetAgainAttack(false);
		

					mCurState = eMonsterState::NEARATTACK_AFTER;
			

				}

		

	}

	void SkeletonLizardScript::NearAttackAfter()
	{
		Vector3 MonsterPos = mTransform->GetPosition();
		Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
		float distance = (PlayerPos - MonsterPos).Length();

		if (distance <= m_tMonsterInfo.m_fNearAttackRange)
		{
			mAnimator->SetAgainAttack(true);
			mCurState = eMonsterState::NEARATTACK;
		}

		else if (distance <= m_tMonsterInfo.m_fDetectRange)
		{
			// 플레이어가 탐지 범위 내에 있지만 근접 공격 범위 밖에 있으면 이동 상태로 전환
			ChangeState(eMonsterState::MOVE);
		}
	}

	void SkeletonLizardScript::NearAttackStart()
	{
		mbAttacking = true;
	}

	void SkeletonLizardScript::NearAttackEnd()
	{
		mbAttacking = false;
	}

	void SkeletonLizardScript::Dead()
	{
	}
	void SkeletonLizardScript::Animation()
	{
	}
}