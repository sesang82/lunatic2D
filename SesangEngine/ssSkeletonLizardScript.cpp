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

		mTransform->SetScale(Vector3(96.f, 48.f, 1.f)); // backsize�� �����ϰ� �����ϰ� �ֱ�. 


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


		// �÷��̾� �ִϸ��̼��� �� Ư���ϹǷ�, ���� ���� ������ �� �ٸ��� �ֵ� ��������	 �����ϰ� �ֱ�. (������ ���� Ʋ���� ��) 
		// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
		//  LT, 1������ �縦 ������, �ڸ� ����, ������, , ����ð� ������
		// === 
		// �÷��̾� �ִϸ��̼��� �� Ư���ϹǷ�, ���� ���� ������ �� �ٸ��� �ֵ� ��������	 �����ϰ� �ֱ�. (������ ���� Ʋ���� ��) 
		// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
		//  LT, 1������ �縦 ������, �ڸ� ����(1���� �����ؼ� ����), �������, ������
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

		// �ϴ� Idle ���´� �������� ����� 
		// ======
			// �ִϸ��̼� ���⿡ ���� ����	�� ����ش�.
		mDir = mTransform->Right();
		mCurDir = mTransform->Right();
		mCurState = eMonsterState::MOVE;

		// ===== �⺻ �浹ü Hit �޴� �뵵 
		mCollider->SetName(L"lizard_HitCol");
		mCollider->SetType(eColliderType::Rect);
		
		mCollider->SetSize(Vector2(0.2f, 0.7f));
		mCollider->SetCenter(Vector2(-8.f, -0.f));
	}
	void SkeletonLizardScript::Update()
	{
		// �̵�->���º�ȯ->�ִϸ��̼�

		Transform* playerTr = mPlayer->GetComponent<Transform>();
		Vector3 playerPos = playerTr->GetPosition();


		// ���� ���� ���� ��ġ ������Ű�� ���ؼ� (���Ͱ� �÷��̾��� ��ġ�� ����Ǹ� �ִϸ��̼��� ���µǴ� ���� ����)
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

		// ���Ϳ� �÷��̾� ���� �Ÿ��� ���� 
		Vector3 vDir = MonsterPos - PlayerPos;
		vDir.z = 0;
		float distance = vDir.Length();




		// ���� ���� ���� ���� �÷��̾ ������ NearAttack ���·� ��ȯ
		if (distance < m_tMonsterInfo.m_fNearAttackRange)
		{
			ChangeState(eMonsterState::NEARATTACK);
			return; // �� �Լ����� �߰����� ó���� �����մϴ�.
		}


		// ======================= tracer

		// PLAYER�� Ž�� ���� �̳��� ������ ���� ���·� ��ȯ 
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

			// ���Ϳ��� �÷��̾������ �Ÿ��� ���Ѵ�. 
			Vector3 vMonToPlayer = PlayerPos - MonsterPos; // �� ���� �÷��̾���� ������ gpt���� �����
			vMonToPlayer.z = 0;
			vMonToPlayer.Normalize();

			float fSpeed = m_tMonsterInfo.m_fSpeed + 200.f; // ���� ���� �� �� ������ �����ϰ� �� 

			// ������ ��ġ�� �÷��̾� �������� �̵���Ų��.
			MonsterPos.x += vMonToPlayer.x * fSpeed * Time::DeltaTime();

			mTransform->SetPosition(MonsterPos);


		}

		// Ž�� �Ÿ��� ����� �ʱ� ��ġ�� ���ư���. 
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




			// ���Ͱ� �ʱ� ��ġ�κ��� min~max�� ������ �����̰� �Ѵ�. 
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
		//// ������ �ִϸ��̼��� ����Ѵ�. 
		//if (mCurDir.x > 0)
		//	mAnimator->PlayAnimation(L"Lizard_RunR", true);

		//else
		//	mAnimator->PlayAnimation(L"Lizard_RunL", true);





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
		// ������ �ִϸ��̼��� ����Ѵ�. 
		if (mCurDir.x > 0)
			mAnimator->PlayAnimation(L"Lizard_HitR", false);

		else
			mAnimator->PlayAnimation(L"Lizard_HitL", false);


		// �ִϸ��̼� ����� ������ 
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
			// �÷��̾ Ž�� ���� ���� ������ ���� ���� ���� �ۿ� ������ �̵� ���·� ��ȯ
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