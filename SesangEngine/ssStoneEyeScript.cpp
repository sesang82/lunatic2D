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



namespace ss
{
	StoneEyeScript::StoneEyeScript()
	{
		m_tMonsterInfo.m_fSpeed = 50.f;
		m_tMonsterInfo.m_fAttack = 10.f;
		m_tMonsterInfo.m_fDetectRange = 300.f;
	}
	StoneEyeScript::~StoneEyeScript()
	{
	}
	void StoneEyeScript::Initialize()
	{
			mTransform = GetOwner()->GetComponent<Transform>();

			mAnimator = GetOwner()->GetComponent<Animator>();


			mRigidbody = GetOwner()->GetComponent<Rigidbody2D>();


			mCollider = GetOwner()->GetComponent<Collider2D>();



			mMeshRenderer = GetOwner()->GetComponent<MeshRenderer>();
	

			mCharacterState = GetOwner()->GetComponent<CharacterState>();

		//mFSM->AddState(L"Stone_Idle", new IdleState());
		//mFSM->ChangeState(L"Stone_Idle"); // �ʱ� ���� ����

		ChangeState(eMonsterState::MOVE); // �ʱ� ���´� move 


		// �ִϸ��̼� ���⿡ ���� ����	�� ����ش�.
		mCurDir = mTransform->Right();

		mAnimator->PlayAnimation(L"StoneEye_NearAttackL", true);

		mAnimator->EndEvent(L"StoneEye_NearAttackR") = std::bind(&StoneEyeScript::NearAttackEnd, this);
		mAnimator->EndEvent(L"StoneEye_NearAttackL") = std::bind(&StoneEyeScript::NearAttackEnd, this);

		//=========

		mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"StoneEyeAttackCollider");
		mAttackColliderObj->Initialize();
		mAttackColliderObj->AddComponent<StoneEyeColScript>();

		mAttackColTr = mAttackColliderObj->GetComponent<Transform>();


		//// ===== (�̰� �׽�Ʈ��)



		////=====
		//mMonsterHpBarObj = object::Instantiate<MonsterBar>(eLayerType::Grid, L"StoneEyeBar"); // ui�� �ϸ� �ȶ� 
		//mMonsterHpBarObj->Initialize();

		//mMonsterHpBarTr = mMonsterHpBarObj->GetComponent<Transform>();
		//mMonsterHpBarTr = 

	}


	void StoneEyeScript::Update()
	{
	




		// �̵�->���º�ȯ->�ִϸ��̼�



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


		MonsterScript::Update();

	}
	void StoneEyeScript::LateUpdate()
	{
		mAttackColTr->SetPosition(mTransform->GetPosition());

		// ������ ���� �� 
		//mMonsterHpBarTr->SetPosition(mTransform->GetPosition() +Vector3(-30.f, 35.f, 1.f)); 


		MonsterScript::LateUpdate();
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


				ChangeState(eMonsterState::MOVE);

		


		}

		if (L"col_BoundaryL" == other->GetOwner()->GetName())
		{
			mCurDir.x = 1;

			L_Boundary = other->GetOwner()->GetComponent<Transform>()->GetPosition().x;
	

		

		
		}

		if (L"col_BoundaryR" == other->GetOwner()->GetName())
		{
			mCurDir.x = -1;

			R_Boundary = other->GetOwner()->GetComponent<Transform>()->GetPosition().x;
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
		Vector3 MonsterPos = mTransform->GetPosition();

		if (mCurDir.x == 1)
		{
			mCollider->SetSize(Vector2(0.16f, 0.43f));
			mCollider->SetCenter(Vector2(-35.f, 0.2f));

			MonsterPos.x += m_tMonsterInfo.m_fSpeed * Time::DeltaTime();
		}

		else if (mCurDir.x == -1)
		{
			mCollider->SetSize(Vector2(0.16f, 0.43f));
			mCollider->SetCenter(Vector2(-35.f, 0.2f));
			MonsterPos.x -= m_tMonsterInfo.m_fSpeed * Time::DeltaTime();
		}

		mTransform->SetPosition(MonsterPos);


		//// �÷��̾ ���� �ȿ� ������ ���� ���·� ��ȯ
		//Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();


		//if (PlayerPos.x > L_Boundary && PlayerPos.x < R_Boundary)
		//{
		//	Vector3 Dir = PlayerPos - MonsterPos;
		//	Dir.z = 0;
		//	Dir.Normalize();
		//
		//	ChangeState(eMonsterState::TRACER);
		//}

	}

	void StoneEyeScript::Tracer()
	{
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
		// near attack �ִϸ��̼��� ������ 
		if(mAnimator->GetCurActiveAnimation()->IsComplete())
		{

				Vector3 MonsterPos = mTransform->GetPosition();
				Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
				float distance = (PlayerPos - MonsterPos).Length();

				if (distance < 15.f) // ���� ���� �� 
				{
					ChangeState(eMonsterState::TRACER);
				}
				else
				{
					ChangeState(eMonsterState::MOVE);
				}
			
		}
	}

	void StoneEyeScript::FarAttack()
	{
		// near attack �ִϸ��̼��� ������ 
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{

			Vector3 MonsterPos = mTransform->GetPosition();
			Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
			float distance = (PlayerPos - MonsterPos).Length();

			if (distance < 2.f) // ���� ���� �� 
			{
				ChangeState(eMonsterState::TRACER);
			}
			else
			{
				ChangeState(eMonsterState::MOVE);
			}

		}
	}

	void StoneEyeScript::Dead()
	{
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
				case ss::eMonsterState::MOVE:
				{
					if (mCurDir.x > 0)
						mAnimator->PlayAnimation(L"StoneEye_IdleR", true);

					else
						mAnimator->PlayAnimation(L"StoneEye_IdleL", true);
				}
			
				break;

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
						mAttackCol->SetSize(Vector2(45.f, 38.f));
						mAttackCol->SetCenter(Vector2(-90.f, -8.f));
					}

					if (mAnimator->GetCurActiveAnimation()->GetIndex() == 9)
					{
						mAttackCol->SetSize(Vector2(45.f, 38.f));
						mAttackCol->SetCenter(Vector2(-90.f, -8.f));
					}

					if (mAnimator->GetCurActiveAnimation()->GetIndex() == 10)
					{
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
	void StoneEyeScript::NearAttackEnd()
	{
		mAttackColliderObj->RemoveComponent<Collider2D>();
	}
}
