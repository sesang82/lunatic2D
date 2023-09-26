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
#include "ssStoneFarRangeScript.h"
#include "ssEffect.h"



namespace ss
{
	StoneEyeScript::StoneEyeScript()
		: mbNearAttack(false)
		, mbFarAttack(false)
		, mCurPos(Vector2::Zero)
		, mbAttacking(false)
	
	{
		m_tMonsterInfo.m_fSpeed = 30.f;
		m_tMonsterInfo.m_fAttack = 10.f;
		m_tMonsterInfo.m_fDetectRange = 300.f;
		m_tMonsterInfo.m_fCoolDown = 0.1f;
	}
	StoneEyeScript::~StoneEyeScript()
	{
		// �Ҹ��� �� �ƿ� ���� ������ �������ش�. (�ش� �浹ü�� �浹 ���� ��� �浹ü�� ���� �����ִ� ��찡 ����. �̰� ��� �ؾߵɱ�)
		// 
		
	}
	void StoneEyeScript::Initialize()
	{
		Scene* pCurScene = SceneManager::GetActiveScene();
			mPlayer = pCurScene->FindObjInActiveLayer<Player>(eLayerType::Player);


			mTransform = GetOwner()->GetComponent<Transform>();

			mAnimator = GetOwner()->GetComponent<Animator>();


			mRigidbody = GetOwner()->GetComponent<Rigidbody2D>();


			mCollider = GetOwner()->GetComponent<Collider2D>();



			mMeshRenderer = GetOwner()->GetComponent<MeshRenderer>();
	

			mCharacterState = GetOwner()->GetComponent<CharacterState>();
			
			// ������ �𸣰����� -10�� ���൵ ù �����ӿ� -20�� ���̰� �� ������ ���������� ����.
			// �׷��� ü���� 100�� ���ִ� �༮�� �ִٸ� 110���� ������. 
			mCharacterState->SetMaxHP(110.f);
			mCharacterState->SetCurrentHP(110.f);


			mFirstPos = GetOwner()->GetComponent<Transform>()->GetPosition();

		//mFSM->AddState(L"Stone_Idle", new IdleState());
		//mFSM->ChangeState(L"Stone_Idle"); // �ʱ� ���� ����



		// �ִϸ��̼� ���⿡ ���� ����	�� ����ش�.
		mDir = mTransform->Right();
		mCurDir = mTransform->Right();
		mCurState = eMonsterState::MOVE;
	

		mAnimator->PlayAnimation(L"StoneEye_IdleR", true);
	
		//mAnimator->EndEvent(L"StoneEye_NearAttackR") = std::bind(&StoneEyeScript::NearAttackEnd, this);
		//mAnimator->EndEvent(L"StoneEye_NearAttackL") = std::bind(&StoneEyeScript::NearAttackEnd, this);

		//==== ���� ���� Ư�� �ε��� �浹ü 
		//�浹ü�� ���⼭ �ٷ� ���� ���� �ش� �ε��� �� �־��ٰ� ���� ������ �ϱ� 
		mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"StoneEyeAttackCollider");
		mAttackColliderObj->Initialize();
		mAttackColliderObj->AddComponent<StoneEyeColScript>();

		mAttackColTr = mAttackColliderObj->GetComponent<Transform>();


		// Near ���� ������ �浹ü
		// ���� �Ǻ� �뵵�� etc ���̾ �ֱ� 
		mNearRangeColObj = object::Instantiate<RangeCollider>(eLayerType::Etc, L"StoneNearRangeCol");
		mNearRangeColObj->Initialize();

		mNearTr = mNearRangeColObj->GetComponent<Transform>();

		StoneNearRangeScript* nearscript = mNearRangeColObj->AddComponent<StoneNearRangeScript>();
		nearscript->SetOwner(mTransform->GetOwner()); // ������� ������Ʈ�� �����صд�.


		mNearCol = mNearRangeColObj->GetComponent<Collider2D>();


		mNearCol->SetSize(Vector2(130.f, 20.f));
		mNearCol->SetCenter(Vector2(-35.f, 0.2f));



		// ���Ÿ� ���� ������ �浹ü
		mFarRangeColObj = object::Instantiate<RangeCollider>(eLayerType::Etc, L"StoneNearRangeCol");
		mFarRangeColObj->Initialize();

		mFarTr = mFarRangeColObj->GetComponent<Transform>();

		StoneFarRangeScript* farScript = mFarRangeColObj->AddComponent<StoneFarRangeScript>();
		farScript->SetOwner(mTransform->GetOwner()); // ������� ������Ʈ�� �����صд�.


		mFarCol = mFarRangeColObj->GetComponent<Collider2D>();


		mFarCol->SetSize(Vector2(200.f, 20.f));
		mFarCol->SetCenter(Vector2(-35.f, 0.2f));





		mAnimator->CompleteEvent(L"StoneEye_NearAttackR") = std::bind(&StoneEyeScript::NearAttackEnd, this);

		mAnimator->CompleteEvent(L"StoneEye_NearAttackL") = std::bind(&StoneEyeScript::NearAttackEnd, this);



	}


	void StoneEyeScript::Update()
	{


		// �̵�->���º�ȯ->�ִϸ��̼�

		Transform* playerTr = mPlayer->GetComponent<Transform>();
		Vector3 playerPos = playerTr->GetPosition();


		// ������ ������ ����ش�. (������ ��ġ ������ X���� ũ�� �������̹Ƿ�  1, ���ʿ� ������ -1)
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


		mPrevState = mCurState;

		if (mCurState != eMonsterState::DEAD)
		{
			mPrevDir = mCurDir;
		}


	}
	void StoneEyeScript::LateUpdate()
	{
		mAttackColTr->SetPosition(mTransform->GetPosition());

		mNearTr->SetPosition(mTransform->GetPosition());
		mFarTr->SetPosition(mTransform->GetPosition());

	
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
				mRigidbody->SetVelocity(Vector2(0.0f, 0.0f)); // ���࿡ ���Ͱ� �������ٰ� ���߸� �̰� �ڵ� �ּ��ɾ��ٰ� �ٽ� �������Ѻ��� 


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


		// ������ �ִϸ��̼��� ����Ѵ�. 
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


			// === ******�Ʒ��� �����ϴ� ���̹Ƿ� �ٸ� ���Ϳ��� ��Ա� 
			// 	if (vDir.Length() < fDetectRange)
			//Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
			//Vector3 MonsterPos = mTransform->GetPosition();

			//// ���Ϳ��� �÷��̾������ �Ÿ��� ���Ѵ�. 
			//Vector3 vMonToPlayer = PlayerPos - MonsterPos; // �� ���� �÷��̾���� ������ gpt���� �����
			//vMonToPlayer.z = 0;
			//vMonToPlayer.Normalize();

			//float fSpeed = m_tMonsterInfo.m_fSpeed;

			//// ������ ��ġ�� �÷��̾� �������� �̵���Ų��.
			//MonsterPos.x += vMonToPlayer.x * fSpeed * Time::DeltaTime();

			//mTransform->SetPosition(MonsterPos);



		
	
	
			


		


		//// �÷��̾�� ���� ���� �Ÿ� ���� ���Ѵ�. 
		//float length = (PlayerPos - MonsterPos).Length();


		//// �÷��̾ ���� �ۿ� �ִٸ� ���ʹ� �Դ� ���� �Ѵ�. 
		//if (length > 50.0f)
		//{

		//}

		////// �÷��̾ ���� �ȿ� ������ ���� ���·� ��ȯ
		//else if (length >= 50.0f && length <= 100.0f) // length�� 50~100�� �� ���Ͱ� player ������ �����δ� (���ڰ� �������� �ش� ���� ������ ������) 
		//{
		//	Vector3 Dir = PlayerPos - MonsterPos;
		//	Dir.z = 0;
		//	Dir.Normalize();

		//	MonsterPos.x += Dir.x * m_tMonsterInfo.m_fSpeed * Time::DeltaTime();
		//	mTransform->SetPosition(MonsterPos);
		//}
		//else if (length < 50.0f) // length�� 50���� ������ nearattack ���·� ��ȯ (
		//{
		//	mCurState = eMonsterState::NEARATTACK;
		//}
		//else if (length > 100.0f && length <= 150.0f) // length�� 100~150 �̵Ǹ� farattack
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
		// ������ �ִϸ��̼��� ����Ѵ�. 
		if (mCurDir.x > 0)
			mAnimator->PlayAnimation(L"StoneEye_IdleR", true);

		else
			mAnimator->PlayAnimation(L"StoneEye_IdleL", true);


	


		//Vector3 MonsterPos = mTransform->GetPosition();
		//Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();

		//Vector3 Dir = PlayerPos - MonsterPos;
		//float Distance = Dir.Length(); // ���� �Ÿ��� ���

		//Dir.z = 0;
		//Dir.Normalize(); // ����ȭ (�Ÿ� ����ϱ����� ����ȭ�ϸ� ��� 1�̶�� ���� ���� ��) 
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
		// ������ �ִϸ��̼��� ����Ѵ�. 
		{
			if (mCurDir.x > 0)
				mAnimator->PlayAnimation(L"StoneEye_StunR", false);

			else
				mAnimator->PlayAnimation(L"StoneEye_StunL", false);
		}


		// �ִϸ��̼� ����� ������ 
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			if (mbNearAttack)
			{
				mCurState = eMonsterState::NEARATTACK;
			}

			else if (mbFarAttack)
			{
				mCurState = eMonsterState::FARATTACK;
			}

			// �� �� ���� ���� ���°� �ƴ϶�� �÷��̾ ���� ���� �ۿ� �־��ٴ� ���̹Ƿ�
			else if (!mbNearAttack && !mbFarAttack)
			{
				mCurState = eMonsterState::MOVE;
			}
		}
	}

	void StoneEyeScript::Hit()
	{

		mAttackColliderObj->RemoveComponent<Collider2D>();

		Vector3 monsterPos = mTransform->GetPosition();

		// ������ �ִϸ��̼��� ����Ѵ�. 
		if (mCurDir.x > 0)
		{
			mRigidbody->AddForce(Vector2(-0.07f, 0.f));

			mAnimator->PlayAnimation(L"StoneEye_HitR", false);

			mTransform->SetPosition(Vector3(monsterPos.x - 0.005f, monsterPos.y, monsterPos.z));
		}

		else
		{
			mRigidbody->AddForce(Vector2(0.07f, 0.f));

			mTransform->SetPosition(Vector3(monsterPos.x + 0.005f, monsterPos.y, monsterPos.z));

			mAnimator->PlayAnimation(L"StoneEye_HitL", false);


		
		}


		// �ִϸ��̼� ����� ������ 
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			if (mbNearAttack)
			{
				mCurState = eMonsterState::NEARATTACK;


			}

			else if (mbFarAttack)
			{
				mCurState = eMonsterState::FARATTACK;
			}

			else
			{
				mCurState = eMonsterState::FARATTACK;
			}
		}

	}

	void StoneEyeScript::Guard()
	{






	}

	void StoneEyeScript::NearAttack()
	{
		{
			PlayerScript* playerScript = mPlayer->GetComponent<PlayerScript>();

			if (mCurDir.x > 0)
			{
				

				if (mAnimator->GetCurActiveAnimation()->GetIndex() == 8)
				{
					mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();

					mAttackCol->SetSize(Vector2(30.f, 38.f));
					mAttackCol->SetCenter(Vector2(24.f, -8.f));

					// �뽬 �߿� �ƿ� �浹 �ȵǰ� �ع��� 
					if (playerScript->IsDash())
					{
						mAttackColliderObj->RemoveComponent<Collider2D>();
					}
				}

				else if(mAnimator->GetCurActiveAnimation()->GetIndex() == 9)
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

					// �뽬 �߿� �ƿ� �浹 �ȵǰ� �ع��� 
					if (playerScript->IsDash())
					{
						mAttackColliderObj->RemoveComponent<Collider2D>();
					}
				}

				else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 9)
				{
					mAttackColliderObj->RemoveComponent<Collider2D>();
				}


				//mAttackColliderObj->RemoveComponent<Collider2D>();


				mAnimator->PlayAnimation(L"StoneEye_NearAttackL", true);



		
			}
		}

		//// near attack �ִϸ��̼��� ������ 
		//if(mAnimator->GetCurActiveAnimation()->IsComplete())
		//{

		//		Vector3 MonsterPos = mTransform->GetPosition();
		//		Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
		//		float distance = (PlayerPos - MonsterPos).Length();

		//		if (distance < 200.f) // ���� ���� �� 
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

	void StoneEyeScript::NearAttackEnd()
	{
		mbNearAttack = false;

	


	}

	void StoneEyeScript::FarAttack()
	{

			mCurPos = mTransform->GetPosition();
			mCurPos.z -= 0.01;

		{
			if (mAnimator->GetCurActiveAnimation()->GetIndex() == 8)
			{
				mbAttacking = true;

				m_fTime += (float)Time::DeltaTime();

				// coolDown �ʸ��� �߻� 
				if (m_fTime >= m_tMonsterInfo.m_fCoolDown)
				{

					if (mCurDir.x == 1.f)
					{
						// �߻�ü ��ġ ���� 
						mCurPos += Vector3(8.f, -2.5f, 0.f);
						mArrowObj = object::Instantiate<StoneEyeProjectile>(mCurPos, eLayerType::Mon_Bullet, L"StoneEyeFarObjR");
						mArrowObj->GetComponent<ProjectileScript>()->SetOriginOwner(mTransform->GetOwner());
				
				
					}

					else if (mCurDir.x == -1.0f)
					{
						// �߻�ü ��ġ ���� 
						mCurPos -= Vector3(75.f, 2.5f, 0.f);

						mArrowObj = object::Instantiate<StoneEyeProjectile>(mCurPos, eLayerType::Mon_Bullet, L"StoneEyeFarObjL");
						mArrowObj->GetComponent<ProjectileScript>()->SetOriginOwner(mTransform->GetOwner());
						mArrowTr = mArrowObj->GetComponent<Transform>();
						mArrowTr->SetScale(Vector3(- 25.f, 10.f, 0.f));
					}
					m_fTime = 0.0f;

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

		}
		
		
	}

	void StoneEyeScript::Dead()
	{

		if (mPrevDir.x > 0)
		{
			mAnimator->PlayAnimation(L"StoneEye_DieR", false);
			mCollider->SetSize(Vector2(0.19f, 0.33f));
			mCollider->SetCenter(Vector2(-32.f, 0.f));

		}

		else
		{
			mAnimator->PlayAnimation(L"StoneEye_DieL", false);
			mCollider->SetSize(Vector2(0.19f, 0.33f));
			mCollider->SetCenter(Vector2(-37.f, 0.f));
		}

		// �ִϸ��̼� ����� ������ 
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			mNearRangeColObj->SetState(GameObject::eState::Dead);
			mAttackColliderObj->SetState(GameObject::eState::Dead);
			mFarRangeColObj->SetState(GameObject::eState::Dead);

			if (mPrevState == eMonsterState::FARATTACK)
			{
				mArrowObj->SetState(GameObject::eState::Dead);
			}

			GetOwner()->SetState(GameObject::eState::Dead);
		}

	}
	
	void StoneEyeScript::Animation()
	{

		// �ִϸ��̼� ������Ʈ�� ��� ���� �ʵ��� ���ó��
	// ���� ���¿� ���� ���°� �ٸ� ���� �ִϸ��̼� ����� �ǵ��� �Ѵ�
	// (Update�� ������ ������ ���� ���·� �������� ������ 1�ʸ��� ��� play�ǹǷ� �����ִ°�ó�� ����)


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


					// === Attack�� 
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

	void StoneEyeScript::StunEnd()
	{
		mEffectObj->SetState(GameObject::eState::Dead);
	}

}
