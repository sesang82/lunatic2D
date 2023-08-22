#include "ssStoneEyeScript.h"
#include "ssTexture.h"
#include "ssResources.h"
#include "ssAnimator.h"
#include "ssFSM.h"
#include "ssIdleState.h"
#include "ssGameObject.h"
#include "ssRigidbody2D.h"
#include "ssTime.h"
#include "ssMonster.h"
#include "ssPlayer.h"
#include "ssPlayerScript.h"
#include "ssMeshRenderer.h"
#include "ssObject.h"
#include "ssAttackCollider.h"
#include "ssStoneEyeColScript.h"
#include "ssMonsterBar.h"
#include "ssCharacterState.h"
#include "ssStoneEyeProjectile.h"
#include "ssProjectileScript.h"
#include "ssStoneNearRangeScript.h"
#include "ssRangeCollider.h"



namespace ss
{
	StoneEyeScript::StoneEyeScript()
	{
		m_tMonsterInfo.m_fSpeed = 30.f;
		m_tMonsterInfo.m_fAttack = 10.f;
		m_tMonsterInfo.m_fDetectRange = 300.f;
		m_tMonsterInfo.m_fCoolDown = 0.1f;
	}
	StoneEyeScript::~StoneEyeScript()
	{
	}
	void StoneEyeScript::Initialize()
	{
		Scene* pCurScene = SceneManager::GetActiveScene();
			mPlayer = pCurScene->FindObjInActiveLayer<Player>();


			mTransform = GetOwner()->GetComponent<Transform>();

			mAnimator = GetOwner()->GetComponent<Animator>();


			mRigidbody = GetOwner()->GetComponent<Rigidbody2D>();


			mCollider = GetOwner()->GetComponent<Collider2D>();



			mMeshRenderer = GetOwner()->GetComponent<MeshRenderer>();
	

			mCharacterState = GetOwner()->GetComponent<CharacterState>();
			
			mCharacterState->SetMaxHP(100.f);
			mCharacterState->SetCurrentHP(100.f);


			mFirstPos = GetOwner()->GetComponent<Transform>()->GetPosition();

		//mFSM->AddState(L"Stone_Idle", new IdleState());
		//mFSM->ChangeState(L"Stone_Idle"); // 초기 상태 설정



		// 애니메이션 방향에 관한 기준	을 잡아준다.
		mDir = mTransform->Right();
		mCurDir = mTransform->Right();
		mCurState = eMonsterState::MOVE;
	

		mAnimator->PlayAnimation(L"StoneEye_IdleR", true);
	
		//mAnimator->EndEvent(L"StoneEye_NearAttackR") = std::bind(&StoneEyeScript::NearAttackEnd, this);
		//mAnimator->EndEvent(L"StoneEye_NearAttackL") = std::bind(&StoneEyeScript::NearAttackEnd, this);

		//==== 근접 공격 특정 인덱스 충돌체 
		//충돌체는 여기서 바로 넣지 말고 해당 인덱스 때 넣었다가 빼는 식으로 하기 
		mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"StoneEyeAttackCollider");
		mAttackColliderObj->Initialize();
		mAttackColliderObj->AddComponent<StoneEyeColScript>();

		mAttackColTr = mAttackColliderObj->GetComponent<Transform>();


		// Near 공격 판정용 충돌체
		// 공격 판별 용도는 etc 레이어에 넣기 
		mNearRangeColObj = object::Instantiate<RangeCollider>(eLayerType::Etc, L"StoneNearRangeCol");
		mNearRangeColObj->Initialize();

		mNearTr = mNearRangeColObj->GetComponent<Transform>();

		StoneNearRangeScript* nearscript = mNearRangeColObj->AddComponent<StoneNearRangeScript>();
		nearscript->SetOwner(mTransform->GetOwner()); // 스톤아이 오브젝트를 저장해둔다.


		mNearCol = mNearRangeColObj->GetComponent<Collider2D>();


		mNearCol->SetSize(Vector2(130.f, 20.f));
		mNearCol->SetCenter(Vector2(-35.f, 0.2f));



		// 원거리 공격 판정용 충돌체





	}


	void StoneEyeScript::Update()
	{
		// === far 공격용 화살 
		//mArrowObj = object::Instantiate<StoneEyeProjectile>(eLayerType::Collision, L"StoneEyeFarCollider");
		//mArrowObj->Initialize();


		//mArrowTr = mArrowObj->GetComponent<Transform>();

		//mArrowScript = mArrowObj->GetComponent<ProjectileScript>();

		//mArrowScript->StoreMonsterPos(mTransform->GetPosition());




		// 이동->상태변환->애니메이션

		Transform* playerTr = mPlayer->GetComponent<Transform>();
		Vector3 playerPos = playerTr->GetPosition();


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

		case ss::eMonsterState::LADING:
			Landing();
			break;

		case ss::eMonsterState::STUN:
			Stun();
			break;

		case ss::eMonsterState::HIT:
			Hit();
			break;

		case ss::eMonsterState::GUARD:
			Guard();
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

		//Animation();

		mPrevState = mCurState;
		mPrevDir = mCurDir;

	}
	void StoneEyeScript::LateUpdate()
	{
		mAttackColTr->SetPosition(mTransform->GetPosition());
		//mArrowTr->SetPosition(mTransform->GetPosition() + Vector3(-46.f, -3.f, 1.f));
		mNearTr->SetPosition(mTransform->GetPosition());

	
	}
	void StoneEyeScript::OnCollisionEnter(Collider2D* other)
	{

		if (L"col_Floor" == other->GetOwner()->GetName())
		{


				Vector3 pos = mCollider->GetPosition();
				Vector3 groundpos = other->GetOwner()->GetComponent<Collider2D>()->GetPosition();


				float fLen = fabs(pos.y - groundpos.y);
				float fValue = (mCollider->GetScale().y / 2.f)
					+ (other->GetOwner()->GetComponent<Collider2D>()->GetScale().y / 2.f);


				if (fLen < fValue)
				{
					Vector3 pos = mTransform->GetPosition();
					Vector3 groundpos = other->GetOwner()->GetComponent<Transform>()->GetPosition();

					pos.y += (fValue - fLen) - 1.f;
					mTransform->SetPosition(pos);
				}
				
			other->GetOwner()->GetComponent<Transform>();


				mRigidbody->SetGround(true);
				mRigidbody->SetVelocity(Vector2(0.0f, 0.0f)); // 만약에 몬스터가 떨어지다가 멈추면 이거 코드 주석걸었다가 다시 원복시켜보기 


				//ChangeState(eMonsterState::MOVE);

		


		}

		



	}
	void StoneEyeScript::OnCollisionStay(Collider2D* other)
	{
	}
	void StoneEyeScript::OnCollisionExit(Collider2D* other)
	{
	}

	void StoneEyeScript::Move()
	{


		// 방향대로 애니메이션을 재생한다. 
		if (mDir.x > 0)
		{
			mAnimator->PlayAnimation(L"StoneEye_IdleR", true);
			mCollider->SetSize(Vector2(0.15f, 0.43f));
			mCollider->SetCenter(Vector2(-35.f, 0.2f));
		}

		else
		{
			mAnimator->PlayAnimation(L"StoneEye_IdleL", true);
			mCollider->SetSize(Vector2(0.15f, 0.43f));
			mCollider->SetCenter(Vector2(-35.f, 0.2f));
		}



		float minX = mFirstPos.x - 40.f;
		float maxX = mFirstPos.x + 40.f;

		Vector3 MonsterPos = mTransform->GetPosition();

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


			// === ******아래는 추적하는 것이므로 다른 몬스터에게 써먹기 
			// 	if (vDir.Length() < fDetectRange)
			//Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
			//Vector3 MonsterPos = mTransform->GetPosition();

			//// 몬스터에서 플레이어까지의 거리를 구한다. 
			//Vector3 vMonToPlayer = PlayerPos - MonsterPos; // 왜 여긴 플레이어부터 빼는지 gpt에게 물어보기
			//vMonToPlayer.z = 0;
			//vMonToPlayer.Normalize();

			//float fSpeed = m_tMonsterInfo.m_fSpeed;

			//// 몬스터의 위치를 플레이어 방향으로 이동시킨다.
			//MonsterPos.x += vMonToPlayer.x * fSpeed * Time::DeltaTime();

			//mTransform->SetPosition(MonsterPos);



		
	
	
			


		


		//// 플레이어와 몬스터 간의 거리 값을 구한다. 
		//float length = (PlayerPos - MonsterPos).Length();


		//// 플레이어가 범위 밖에 있다면 몬스터는 왔다 갔다 한다. 
		//if (length > 50.0f)
		//{

		//}

		////// 플레이어가 범위 안에 들어오면 추적 상태로 변환
		//else if (length >= 50.0f && length <= 100.0f) // length가 50~100일 때 몬스터가 player 방향대로 움직인다 (숫자가 높을수록 해당 상태 유지가 높아짐) 
		//{
		//	Vector3 Dir = PlayerPos - MonsterPos;
		//	Dir.z = 0;
		//	Dir.Normalize();

		//	MonsterPos.x += Dir.x * m_tMonsterInfo.m_fSpeed * Time::DeltaTime();
		//	mTransform->SetPosition(MonsterPos);
		//}
		//else if (length < 50.0f) // length가 50보다 작으면 nearattack 상태로 변환 (
		//{
		//	mCurState = eMonsterState::NEARATTACK;
		//}
		//else if (length > 100.0f && length <= 150.0f) // length가 100~150 이되면 farattack
		//{
		//	mCurState = eMonsterState::FARATTACK;
		//}
		//else if (length > 150.0f)
		//{
		//	float range = 100.0f;
		//	float distance = (mFirstPos - MonsterPos).Length();
		//	Vector3 dir = mFirstPos - MonsterPos;

		//	dir.Normalize();

		//	if (distance >= range)
		//	{
		//		dir.x *= -1;
		//	}

		//	MonsterPos.x += dir.x * m_tMonsterInfo.m_fSpeed * Time::DeltaTime();

		//	mTransform->SetPosition(MonsterPos);
		//}

	}

	void StoneEyeScript::Tracer()
	{
		// 방향대로 애니메이션을 재생한다. 
		if (mCurDir.x > 0)
			mAnimator->PlayAnimation(L"StoneEye_IdleR", true);

		else
			mAnimator->PlayAnimation(L"StoneEye_IdleL", true);


	


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
		//
	}

	void StoneEyeScript::Jump()
	{
	}

	void StoneEyeScript::Fall()
	{
	}

	void StoneEyeScript::Landing()
	{
	}

	void StoneEyeScript::Stun()
	{
	}

	void StoneEyeScript::Hit()
	{
	}

	void StoneEyeScript::Guard()
	{
	}

	void StoneEyeScript::NearAttack()
	{
		{
		

			if (mCurDir.x > 0)
			{

				if (mAnimator->GetCurActiveAnimation()->GetIndex() == 8)
				{
					mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();

					mAttackCol->SetSize(Vector2(45.f, 38.f));
					mAttackCol->SetCenter(Vector2(24.f, -8.f));
				}


				if (mAnimator->GetCurActiveAnimation()->GetIndex() == 9)
				{
					mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();

					mAttackCol->SetSize(Vector2(45.f, 38.f));
					mAttackCol->SetCenter(Vector2(24.f, -8.f));
				}
			

				if (mAnimator->GetCurActiveAnimation()->GetIndex() == 10)
				{
					mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();

					mAttackCol->SetSize(Vector2(45.f, 38.f));
					mAttackCol->SetCenter(Vector2(24.f, -8.f));
				}
	

				if (mAnimator->GetCurActiveAnimation()->GetIndex() == 11)
				{
					mAttackColliderObj->RemoveComponent<Collider2D>();

				}

				mAnimator->PlayAnimation(L"StoneEye_NearAttackR", true);
				

			}

			else
			{

				if (mAnimator->GetCurActiveAnimation()->GetIndex() == 8)
				{
					mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
					mAttackCol->SetSize(Vector2(45.f, 38.f));
					mAttackCol->SetCenter(Vector2(-90.f, -8.f));
				}

				if (mAnimator->GetCurActiveAnimation()->GetIndex() == 9)
				{
					mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
					mAttackCol->SetSize(Vector2(45.f, 38.f));
					mAttackCol->SetCenter(Vector2(-90.f, -8.f));
				}

				if (mAnimator->GetCurActiveAnimation()->GetIndex() == 10)
				{
					mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
					mAttackCol->SetSize(Vector2(45.f, 38.f));
					mAttackCol->SetCenter(Vector2(-90.f, -8.f));
				}

				if (mAnimator->GetCurActiveAnimation()->GetIndex() == 11)
				{
					mAttackColliderObj->RemoveComponent<Collider2D>();

				}

				mAnimator->PlayAnimation(L"StoneEye_NearAttackL", true);
		
			}
		}


		//// near attack 애니메이션이 끝나면 
		//if(mAnimator->GetCurActiveAnimation()->IsComplete())
		//{

		//		Vector3 MonsterPos = mTransform->GetPosition();
		//		Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
		//		float distance = (PlayerPos - MonsterPos).Length();

		//		if (distance < 200.f) // 추적 범위 값 
		//		{
		//			ChangeState(eMonsterState::TRACER);
		//		}
		//		else
		//		{
		//			ChangeState(eMonsterState::MOVE);
		//		}
		//	
		//}
	}

	void StoneEyeScript::FarAttack()
	{

		Vector3 pos = mTransform->GetPosition();
		pos.z -= 0.01;

		if (mAnimator->GetCurActiveAnimation()->GetIndex() == 8)
		{

			m_fTime += (float)Time::DeltaTime();

			// coolDown 초마다 발사 
			if (m_fTime >= m_tMonsterInfo.m_fCoolDown)
			{

				if (mCurDir.x == 1.f)
				{
					// 발사체 위치 조절 
					pos += Vector3(8.f, -2.5f, 0.f);
				}

				else if (mCurDir.x == -1.0f)
				{
					// 발사체 위치 조절 
					pos -= Vector3(75.f, 2.5f, 0.f);
				}
				m_fTime = 0.0f;


				mArrowObj = object::Instantiate<StoneEyeProjectile>(pos, eLayerType::Bullet, L"StoneEyeFarCollider");
				mArrowObj->GetComponent<ProjectileScript>()->SetOriginOwner(mTransform->GetOwner());
			

				// === guard 할 때 쓰면 될 듯 (거꾸로 충돌체 가는거) 
				/*mArrowObj = object::Instantiate<StoneEyeProjectile>(pos, eLayerType::Bullet, L"StoneEyeFarCollider");
				
				if (mPrevDir.x > 0)
					{
						mArrowObj->GetComponent<ProjectileScript>()->SetReserve(true);
					}*/

				

	
			}

	

		}

		if (mCurDir.x > 0)
		{
			mAnimator->PlayAnimation(L"StoneEye_FarAttackR", true);
			mCollider->SetSize(Vector2(0.19f, 0.33f));
			mCollider->SetCenter(Vector2(-32.f, 0.f));

		}

		else
		{
			mAnimator->PlayAnimation(L"StoneEye_FarAttackL", true);
			mCollider->SetSize(Vector2(0.19f, 0.33f));
			mCollider->SetCenter(Vector2(-37.f, 0.f));
		}

		//// near attack 애니메이션이 끝나면 
		//if (mAnimator->GetCurActiveAnimation()->IsComplete())/* && (mAnimator->GetCurActiveAnim() == L"StoneEye_FarAttackR" || mAnimator->GetCurActiveAnim() == L"StoneEye_FarAttackL")*/
		//{
		//	mCurState = eMonsterState::MOVE;
		//	
		//}
	}

	void StoneEyeScript::Dead()
	{
	}
	
	void StoneEyeScript::Animation()
	{

		// 애니메이션 업데이트가 계속 돌지 않도록 방어처리
	// 이전 상태와 현재 상태가 다를 때만 애니메이션 재생이 되도록 한다
	// (Update에 적었기 때문에 이전 상태로 비교해주지 않으면 1초마다 계속 play되므로 멈춰있는것처럼 보임)


		if (mCurState != mPrevState || mCurDir != mPrevDir)
		{
			switch (mCurState)
			{
			case ss::eMonsterState::TRACER:
			{
				if (mCurDir.x > 0)
					mAnimator->PlayAnimation(L"StoneEye_IdleR", true);
				else
					mAnimator->PlayAnimation(L"StoneEye_IdleL", true);

			}

			break;

			case ss::eMonsterState::STUN:

				if (mPrevDir.x > 0)
				{
					mAnimator->PlayAnimation(L"StoneEye_StunR", true);
					mCollider->SetSize(Vector2(0.37f, 0.33f));
					mCollider->SetCenter(Vector2(-33.f, 0.f));
				}

				else
					mAnimator->PlayAnimation(L"StoneEye_StunL", true);

				break;

			case ss::eMonsterState::HIT:

				if (mPrevDir.x > 0)
				{
					mAnimator->PlayAnimation(L"StoneEye_HitR", true);
					mCollider->SetSize(Vector2(0.40f, 0.40f));
					mCollider->SetCenter(Vector2(-32.f, 0.f));
				}

				else
					mAnimator->PlayAnimation(L"StoneEye_HitL", true);
				break;

			case ss::eMonsterState::NEARATTACK:
			{



				if (mPrevDir.x > 0)
				{
					if (mAnimator->GetCurActiveAnimation()->GetIndex() == 8)
					{
						mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();

						mAttackCol->SetSize(Vector2(45.f, 38.f));
						mAttackCol->SetCenter(Vector2(24.f, -8.f));
					}

					if (mAnimator->GetCurActiveAnimation()->GetIndex() == 9)
					{
						mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();

						mAttackCol->SetSize(Vector2(45.f, 38.f));
						mAttackCol->SetCenter(Vector2(24.f, -8.f));
					}

					if (mAnimator->GetCurActiveAnimation()->GetIndex() == 10)
					{
						mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();

						mAttackCol->SetSize(Vector2(45.f, 38.f));
						mAttackCol->SetCenter(Vector2(24.f, -8.f));
					}

					mAnimator->PlayAnimation(L"StoneEye_NearAttackR", false);
					mCollider->SetSize(Vector2(0.37f, 0.39f));
					mCollider->SetCenter(Vector2(-33.f, 0.f));


					// === Attack용 
				}


				else
				{
					if (mAnimator->GetCurActiveAnimation()->GetIndex() == 8)
					{
						mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
						mAttackCol->SetSize(Vector2(45.f, 38.f));
						mAttackCol->SetCenter(Vector2(-90.f, -8.f));
					}

					if (mAnimator->GetCurActiveAnimation()->GetIndex() == 9)
					{
						mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
						mAttackCol->SetSize(Vector2(45.f, 38.f));
						mAttackCol->SetCenter(Vector2(-90.f, -8.f));
					}

					if (mAnimator->GetCurActiveAnimation()->GetIndex() == 10)
					{
						mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
						mAttackCol->SetSize(Vector2(45.f, 38.f));
						mAttackCol->SetCenter(Vector2(-90.f, -8.f));
					}

					mAnimator->PlayAnimation(L"StoneEye_NearAttackL", false);

					mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();

				}

			}
			break;

			case ss::eMonsterState::FARATTACK:
				if (mPrevDir.x > 0)
				{
					mAnimator->PlayAnimation(L"StoneEye_FarAttackR", false);
					mCollider->SetSize(Vector2(0.40f, 0.33f));
					mCollider->SetCenter(Vector2(-32.f, 0.f));

				}

				else
				{
					mAnimator->PlayAnimation(L"StoneEye_FarAttackL", false);
					mCollider->SetSize(Vector2(0.40f, 0.33f));
					mCollider->SetCenter(Vector2(32.f, 0.f));
				}
				break;


			case ss::eMonsterState::DEAD:
				if (mPrevDir.x > 0)
					mAnimator->PlayAnimation(L"StoneEye_DieR", false);
				else
					mAnimator->PlayAnimation(L"StoneEye_DieL", false);
				break;

			}
		}
	}


	void StoneEyeScript::FarAttackEnd()
	{
		mCurState = eMonsterState::MOVE;
	}
}
