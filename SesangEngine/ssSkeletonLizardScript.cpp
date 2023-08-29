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
	// 1. ���� ���� �浹ü �����
	// 2. �浹ü ũ�� ���ϰ� �ִϸ��̼� ������ ��ġ �����ϱ�



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

		// �ϴ� Idle ���´� �������� ����� 
		// ======
			// �ִϸ��̼� ���⿡ ���� ����	�� ����ش�.
		mDir = mTransform->Right();
		mCurDir = mTransform->Right();
		mCurState = eMonsterState::IDLE;

		// ===== �⺻ �浹ü Hit �޴� �뵵 
		mCollider->SetName(L"lizard_HitCol");
		mCollider->SetType(eColliderType::Rect);
		
		mCollider->SetSize(Vector2(0.2f, 0.7f));
		mCollider->SetCenter(Vector2(-17.f, -0.f));


		//==== ���� ���� Ư�� �ε��� �浹ü 
		//�浹ü�� ���⼭ �ٷ� ���� ���� �ش� �ε��� �� �־��ٰ� ���� ������ �ϱ� 
		mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"LizardAttackColObj");
		mAttackColliderObj->Initialize();
		mAttackColliderObj->AddComponent<LizardColScript>();

		mAttackColTr = mAttackColliderObj->GetComponent<Transform>();
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

		// ���Ϳ� �÷��̾� ���� �Ÿ��� ���� 
		Vector3 vDir = MonsterPos - PlayerPos;
		vDir.z = 0;
		float distance = vDir.Length();

		if (distance <= m_tMonsterInfo.m_fDetectRange)
		{
			// �÷��̾ Ž�� ���� ���� ������ ���� ���� ���� �ۿ� ������ �̵� ���·� ��ȯ
			ChangeState(eMonsterState::MOVE);
		}

		// ���� ���� ���� ���� �÷��̾ ������ NearAttack ���·� ��ȯ
		else if (distance < m_tMonsterInfo.m_fNearAttackRange)
		{
			ChangeState(eMonsterState::NEARATTACK);
		}
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
				mCollider->SetCenter(Vector2(-8.f, -0.f));
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

					// �뽬 �߿� �ƿ� �浹 �ȵǰ� �ع��� 
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

					// �뽬 �߿� �ƿ� �浹 �ȵǰ� �ع��� 
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

		// �ִϸ��̼� ����� ������ 
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