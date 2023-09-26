#include "ssWolfScript.h"
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
	// ===== 해야할 거
	// 1. Far 반경이 매우 넓었을 때 현재 Velocity값과 일치 하지 않아서 velocity값을 거리에 따라 조정하는 방법 필요
	// 2. 추적 상태에 돌입했을 때 플레이어와 위치가 겹치지 않도록 별도 처리 필요 


	WolfScript::WolfScript()
		: mbNearAttacking(false)
		, mbFarAttacking(false)
		, mbHit(false)
		, mbJumped(false)
		, mLandingPos(Vector3::Zero)
		, mJumpDir(Vector2::Zero)
		, mVelocity(Vector2::Zero)
	{
		m_tMonsterInfo.m_fSpeed = 200.f;
		m_tMonsterInfo.m_fDetectRange = 150.f;

		m_tMonsterInfo.m_fNearAttackRange = 50.f;
		m_tMonsterInfo.m_fFarAttackRange = 100.f;

		m_tMonsterInfo.m_fCoolDown = 0.5f;
	}
	WolfScript::~WolfScript()
	{
	}
	void WolfScript::Initialize()
	{
		MonsterScript::Initialize();

		mTransform->SetScale(Vector3(102.f, 47.f, 1.f)); // backsize랑 스케일값 동일하게 주기. 


		//
		mCharacterState->SetCurrentHP(110.f);
		mCharacterState->SetMaxHP(110.f);

		mFirstPos = GetOwner()->GetComponent<Transform>()->GetPosition();

		//
		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Wolf_Idle");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Wolf_Run");
		std::shared_ptr<ss::graphics::Texture> Image3 = Resources::Find<ss::graphics::Texture>(L"Wolf_NearAttack");
		std::shared_ptr<ss::graphics::Texture> Image4 = Resources::Find<ss::graphics::Texture>(L"Wolf_FarAttack");
		std::shared_ptr<ss::graphics::Texture> Image5 = Resources::Find<ss::graphics::Texture>(L"Wolf_Hit");
		std::shared_ptr<ss::graphics::Texture> Image6 = Resources::Find<ss::graphics::Texture>(L"Wolf_Stun");
		std::shared_ptr<ss::graphics::Texture> Image7 = Resources::Find<ss::graphics::Texture>(L"Wolf_Die");


		// 플레이어 애니메이션은 좀 특수하므로, 무기 별로 오프셋 값 다르게 주되 백사이즈는	 동일하게 주기. (사이즈 값은 틀려도 됨) 
		// 애니메이션 나중에 플레이어 다 완성되면 
		//  LT, 1프레임 사를 사이즈, 자를 갯수, 오프셋, , 재생시간 뒤집기
		// === 
		// 플레이어 애니메이션은 좀 특수하므로, 무기 별로 오프셋 값 다르게 주되 백사이즈는	 동일하게 주기. (사이즈 값은 틀려도 됨) 
		// 애니메이션 나중에 플레이어 다 완성되면 
		//  LT, 1프레임 사를 사이즈, 자를 갯수(1부터 시작해서 세기), 백사이즈, 오프셋
		mAnimator->Create(L"Wolf_IdleR", Image1, Vector2(0.f, 0.f), Vector2(77.f, 47.f), 6, Vector2(102.f, 47.f));
		mAnimator->Create(L"Wolf_IdleL", Image1, Vector2(0.f, 0.f), Vector2(77.f, 47.f), 6, Vector2(102.f, 47.f), Vector2(-16.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Wolf_RunR", Image2, Vector2(0.f, 0.f), Vector2(77.f, 47.f), 6, Vector2(102.f, 47.f));
		mAnimator->Create(L"Wolf_RunL", Image2, Vector2(0.f, 0.f), Vector2(77.f, 47.f), 6, Vector2(102.f, 47.f), Vector2(-12.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Wolf_NearAttackR", Image3, Vector2(0.f, 0.f), Vector2(77.f, 47.f), 11, Vector2(102.f, 47.f), Vector2::Zero, 0.08f);
		mAnimator->Create(L"Wolf_NearAttackL", Image3, Vector2(0.f, 0.f), Vector2(77.f, 47.f), 11, Vector2(102.f, 47.f), Vector2(-12.f, 0.f), 0.08f, true);

		mAnimator->Create(L"Wolf_FarAttackR", Image4, Vector2(0.f, 0.f), Vector2(77.f, 47.f), 15, Vector2(102.f, 47.f));
		mAnimator->Create(L"Wolf_FarAttackL", Image4, Vector2(0.f, 0.f), Vector2(77.f, 47.f), 15, Vector2(102.f, 47.f), Vector2(-12.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Wolf_HitR", Image5, Vector2(0.f, 0.f), Vector2(77.f, 47.f), 1, Vector2(102.f, 47.f), Vector2::Zero, 0.3f);
		mAnimator->Create(L"Wolf_HitL", Image5, Vector2(0.f, 0.f), Vector2(77.f, 47.f), 1, Vector2(102.f, 47.f), Vector2(-12.f, 0.f), 0.3f, true);

		mAnimator->Create(L"Wolf_StunR", Image6, Vector2(0.f, 0.f), Vector2(77.f, 47.f), 6, Vector2(102.f, 47.f));
		mAnimator->Create(L"Wolf_StunL", Image6, Vector2(0.f, 0.f), Vector2(77.f, 47.f), 6, Vector2(102.f, 47.f), Vector2(-12.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Wolf_DieR", Image7, Vector2(0.f, 0.f), Vector2(77.f, 47.f), 14, Vector2(102.f, 47.f));
		mAnimator->Create(L"Wolf_DieL", Image7, Vector2(0.f, 0.f), Vector2(77.f, 47.f), 14, Vector2(102.f, 47.f), Vector2(-12.f, 0.f), 0.1f, true);




		// ======
		// 애니메이션 방향에 관한 기준	을 잡아준다.
		mDir = mTransform->Right();
		mCurDir = mTransform->Right();
		mCurState = eMonsterState::MOVE;

		// ===== 기본 충돌체 Hit 받는 용도 
		mCollider->SetName(L"Wolf_HitCol");
		mCollider->SetType(eColliderType::Rect);
		mCollider->SetSize(Vector2(0.3f, 0.9f));
		mCollider->SetCenter(Vector2(8.f, -5.f));

		// ===== 리지드바디



		//==== 근접 공격 특정 인덱스 충돌체 
		//충돌체는 여기서 바로 넣지 말고 해당 인덱스 때 넣었다가 빼는 식으로 하기 
		mAttackColliderObj = object::Instantiate<AttackCollider>(mTransform->GetPosition(), eLayerType::Collision, L"WolfAttackColObj");
		mAttackColliderObj->Initialize();
		//mAttackColliderObj->AddComponent<LizardColScript>();

		mAttackColTr = mAttackColliderObj->GetComponent<Transform>();



		mAnimator->CompleteEvent(L"Wolf_NearAttackR") = std::bind(&WolfScript::NearAttackEnd, this);
		mAnimator->CompleteEvent(L"Wolf_NearAttackL") = std::bind(&WolfScript::NearAttackEnd, this);




	}
	void WolfScript::Update()
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

		if (mCurState != eMonsterState::DEAD)
		{
			mPrevDir = mCurDir;
		}


	}
	void WolfScript::LateUpdate()
	{
		mAttackColTr->SetPosition(mTransform->GetPosition());
	}


	void WolfScript::OnCollisionEnter(Collider2D* other)
	{
		if (L"col_SpecialFloor" == other->GetOwner()->GetName())
		{
			mRigidbody->SetGround(true);

			// 플레이어의 위치를 갖고 온다.
			// 해당 충돌체의 위치를 갖고 온다. (충돌체이기 전에 게임오브젝트) 
			mRigidbody->SetVelocity(Vector2(0.0f, 0.0f)); // 점프를 마치면  속도 0으로 해두기 
		}



	}
	void WolfScript::OnCollisionStay(Collider2D* other)
	{
	}
	void WolfScript::OnCollisionExit(Collider2D* other)
	{

		if (L"col_SpecialFloor" == other->GetOwner()->GetName())
		{
			mRigidbody->SetGround(false);
			mRigidbody->SetGravity(Vector2(0.f, 1300.f));

			if (mCurDir.x > 0)
			{
				mRigidbody->SetVelocity(Vector2(50.f, 0.f));
			}

			else if (mCurDir.x < 0)
			{
				mRigidbody->SetVelocity(Vector2(-50.f, 0.f));
			}

		}
	}
	void WolfScript::FarAttackEnd()
	{
	}
	void WolfScript::StunEnd()
	{
	}
	void WolfScript::Idle()
	{
		//mbNearAttacking = false;
		//mbFarAttacking = false;
		//mbHit = false;

		//if (mCurDir.x > 0)
		//{
		//	mAnimator->PlayAnimation(L"Wolf_IdleR", true);
		//}

		//else
		//{
		//	mAnimator->PlayAnimation(L"Wolf_IdleL", true);
		//}



		//Vector3 MonsterPos = mTransform->GetPosition();
		//Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();

		//// 몬스터와 플레이어 간의 거리를 구함 
		//Vector3 vDir = MonsterPos - PlayerPos;
		//vDir.z = 0;
		//float distance = vDir.Length();

		//m_fTime += Time::DeltaTime();

		//if (distance <= m_tMonsterInfo.m_fDetectRange && m_fTime > 3.f)
		//{
		//	// 플레이어가 탐지 범위 내에 있지만 근접 공격 범위 밖에 있으면 이동 상태로 전환
		//	ChangeState(eMonsterState::MOVE);
		//	m_fTime = 0.f;
		//}


	}
	void WolfScript::Move()
	{

		mbNearAttacking = false;
		mbFarAttacking = false;
		mbHit = false;


		Vector3 MonsterPos = mTransform->GetPosition();
		Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();

		// 몬스터와 플레이어 간의 거리를 구함 
		Vector3 vDir = MonsterPos - PlayerPos;
		vDir.z = 0;
		float distance = vDir.Length();




		// 근접 공격 범위 내에 플레이어가 있으면 NearAttack 상태로 전환
		if (distance + 0.4f < m_tMonsterInfo.m_fNearAttackRange && mCurDir.x > 0)
		{
			ChangeState(eMonsterState::NEARATTACK);
			return; // 이 함수에서 추가적인 처리를 중지합니다.
		}

		// 플레이어와 겹친 상태로 공격하는거 방지 
		else if (distance + 0.4f < m_tMonsterInfo.m_fNearAttackRange && mCurDir.x < 0)
		{
			ChangeState(eMonsterState::NEARATTACK);
			return; // 이 함수에서 추가적인 처리를 중지합니다.
		}


		//// 먼거리 공격 범위 내에 플레이어가 있으면 FarAttack 상태로 전환
		//if (distance > m_tMonsterInfo.m_fNearAttackRange && distance < m_tMonsterInfo.m_fFarAttackRange)
		//{
		//	ChangeState(eMonsterState::FARATTACK);
		//	return;
		//}


		// ======================= tracer

		// PLAYER가 탐지 범위 이내에 들어오면 추적 상태로 전환 
		if (vDir.Length() < m_tMonsterInfo.m_fDetectRange)
		{

			if (mCurDir.x > 0)
			{
				mAnimator->PlayAnimation(L"Wolf_RunR", true);
			}

			else
			{
				mAnimator->PlayAnimation(L"Wolf_RunL", true);
			}


			Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
			Vector3 MonsterPos = mTransform->GetPosition();

			// 몬스터에서 플레이어까지의 거리를 구한다. 
			Vector3 vMonToPlayer = PlayerPos - MonsterPos; // 왜 여긴 플레이어부터 빼는지 gpt에게 물어보기
			vMonToPlayer.z = 0;
			vMonToPlayer.Normalize();

			float fSpeed = m_tMonsterInfo.m_fSpeed + 100.f; // 추적 중일 땐 더 빠르게 추적하게 함 

			// 몬스터의 위치를 플레이어 방향으로 이동시킨다.
			MonsterPos.x += vMonToPlayer.x * fSpeed * Time::DeltaTime();

			mTransform->SetPosition(MonsterPos);


		}

		// 탐지 거리를 벗어나면 초기 위치로 돌아간다. 
		else if (vDir.Length() > m_tMonsterInfo.m_fDetectRange)
		{

			if (mDir.x > 0)
			{
				mAnimator->PlayAnimation(L"Wolf_RunR", true);
			}

			else
			{
				mAnimator->PlayAnimation(L"Wolf_RunL", true);
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

	void WolfScript::Jump()
	{
	}
	void WolfScript::Fall()
	{
	}
	void WolfScript::Landing()
	{
	}
	void WolfScript::Stun()
	{
	}
	void WolfScript::Hit()
	{
		if (!mbHit)
		{
			mbHit = true;

			if (nullptr != mAttackColliderObj)
			{
				mAttackColliderObj->RemoveComponent<Collider2D>();
			}


			Vector3 monsterPos = mTransform->GetPosition();

			if (mCurDir.x > 0)
			{
				mRigidbody->AddForce(Vector2(-3.f, 0.f));
				mTransform->SetPosition(Vector3(monsterPos.x - 2.f, monsterPos.y, monsterPos.z));
				mAnimator->PlayAnimation(L"Wolf_HitR", false);
			}

			else
			{
				mRigidbody->AddForce(Vector2(3.f, 0.f));
				mTransform->SetPosition(Vector3(monsterPos.x + 2.f, monsterPos.y, monsterPos.z));
				mAnimator->PlayAnimation(L"Wolf_HitL", false);
			}
		}

		if (mbHit && mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eMonsterState::MOVE);
		}

	}
	void WolfScript::NearAttack()
	{
		Vector3 MonsterPos = mTransform->GetPosition();
		Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
		float distance = (PlayerPos - MonsterPos).Length();

		m_fTime += Time::DeltaTime();

		PlayerScript* playerScript = mPlayer->GetComponent<PlayerScript>();


		// 연이어 공격 애니메이션 재생하지 않고, 쿨타임 시간만큼 기다렸다가 공격 



			if (!mbNearAttacking)
			{
				mbNearAttacking = true;
				if (mCurDir.x > 0)
				{
					mAnimator->PlayAnimation(L"Wolf_NearAttackR", true);
				}
				else
				{
					mAnimator->PlayAnimation(L"Wolf_NearAttackL", true);
				}
				//m_fTime = 0.0f;
			}

		if (mAnimator->GetCurActiveAnimation()->GetIndex() == 9)
		{
			if (mCurDir.x > 0)
			{

				mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();

				mAttackCol->SetSize(Vector2(60.f, 30.f));
				mAttackCol->SetCenter(Vector2(20.f, -5.f));

				// 대쉬 중엔 아예 충돌 안되게 해버림 
				if (playerScript->IsDash())
				{
					mAttackColliderObj->RemoveComponent<Collider2D>();


				}
			}

			else if (mCurDir.x < 0)
			{
				mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();

				mAttackCol->SetSize(Vector2(60.f, 30.f));
				mAttackCol->SetCenter(Vector2(-15.f, -5.f));

				// 대쉬 중엔 아예 충돌 안되게 해버림 
				if (playerScript->IsDash())
				{
					mAttackColliderObj->RemoveComponent<Collider2D>();


				}
			}
		}

		else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 10)
		{
			mbNearAttacking = false;
			mAttackColliderObj->RemoveComponent<Collider2D>();
		}


	}
	void WolfScript::NearAttackEnd()
	{
		ChangeState(eMonsterState::MOVE);
	}
	void WolfScript::FarAttack()
	{

		Vector3 MonsterPos = mTransform->GetPosition();
		Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
		float distance = (PlayerPos - MonsterPos).Length();

		m_fTime += Time::DeltaTime();


		// 연이어 공격 애니메이션 재생하지 않고, 쿨타임 시간만큼 기다렸다가 공격 


		if (m_fTime >= m_tMonsterInfo.m_fCoolDown && !mbFarAttacking)
		{
			mbFarAttacking = true; // 애니메이션 재생이 캐릭터가 방향을 바꿔도 끝까지 유지되어야해서 넣어준 변수 


			if (mPrevDir.x > 0)
			{
				mAnimator->PlayAnimation(L"Wolf_FarAttackR", false);

			}

			else
			{
				mAnimator->PlayAnimation(L"Wolf_FarAttackL", false);
			}


			m_fTime = 0.0f;
		}


		bool isGround = mPlayer->GetComponent<Rigidbody2D>()->IsGround();


		if (mAnimator->GetCurActiveAnimation()->GetIndex() == 2 && !mbJumped && isGround)
		{

			mHitGround = object::Instantiate<Effect>(PlayerPos, eLayerType::Effect, L"WolfHitGroundObj");
			mLandingPos = PlayerPos; // 착지할 위치가 다음 프레임에서도 동일해야하므로 담아둠 

			Vector3 jumpdir = (PlayerPos - MonsterPos);
			jumpdir.Normalize();
			mJumpDir = Vector2(jumpdir.x, jumpdir.y); // 노말라이즈해서 나온 점프 방향 값도 담아둠 (플레이어에게 향할 방법) 

			HitGroundScript* script = mHitGround->AddComponent<HitGroundScript>();
			script->SetMonsterOwner((Monster*)mTransform->GetOwner());

			mbJumped = true;
		}

		// 플레이어 위치로 점프 시작 
		else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 9)
		{
			mRigidbody->SetGravity(Vector2(0.f, 1300.f));

			float JumpPower = sqrt(2 * abs(mRigidbody->GetGravity().y) * 10.f); // * 점프 높이

			mRigidbody->SetVelocity(Vector2(50.0f, -JumpPower));

			// velocity값은 방향에 따라 다르게 주기 
			mVelocity = mRigidbody->GetVelocity();


			// 점프 방향 정해주기 (플레이어 방향이 양수면
			if (mJumpDir.x >= 0.f)
				mVelocity.x = 300.f;

			else if (mJumpDir.x < 0.f)
				mVelocity.x = -300.f;


			mRigidbody->SetVelocity(mVelocity);

			mRigidbody->SetGround(false);



			//mLandingPos.y = PlayerPos.y; // y값은 플레이어 y값으로 고정
		}

		// 13 인덱스 때 충돌체 빼기 및 착지

		else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 13)
		{
			mRigidbody->SetGround(true);
			mRigidbody->SetVelocity(Vector2::Zero);
			mTransform->SetPosition(mLandingPos); // 착지할 위치로 이동
		}

		else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 14) // end event 호출 안되서 대신 씀 
		{
			mbFarAttacking = false;
		}


		if (!mbFarAttacking && mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eMonsterState::MOVE);
		}


	}
	void WolfScript::Dead()
	{
		if (mPrevDir.x > 0)
		{
			mAnimator->PlayAnimation(L"Wolf_DieR", false);

		}

		else
		{
			mAnimator->PlayAnimation(L"Wolf_DieL", false);
		}

		// 애니메이션 재생이 끝나면 
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			mAttackColliderObj->SetState(GameObject::eState::Dead);


			GetOwner()->SetState(GameObject::eState::Dead);
		}

	}
	void WolfScript::Animation()
	{
	}
}