#include "ssSkeletonArcherScript.h"
#include "ssFSM.h"
#include "ssIdleState.h"
#include "ssTraceState.h"
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

	// ���� �ű� �� ������ �� 
	// 1. HIT�� �ݶ��̴� ũ�⸦ �ϳ��� �����Ѵ�. (�¿� ����)
	// 2. �ִϸ��̼��� �ش� �ݶ��̴��� �°� �������� �����ؼ� �����.
	// 3. �׷��� ���ϸ� ���α׷����� �������� �������� ������

	SkeletonArcherScript::SkeletonArcherScript()
		: mbNearAttack(false)
		, mbFarAttack(false)
		, mbAttacking(false)

	{
		m_tMonsterInfo.m_fSpeed = 30.f;
		m_tMonsterInfo.m_fAttack = 10.f;
		m_tMonsterInfo.m_fDetectRange = 300.f;
		m_tMonsterInfo.m_fCoolDown = 0.1f;
	}
	SkeletonArcherScript::~SkeletonArcherScript()
	{
	}
	void SkeletonArcherScript::Initialize()
	{
		MonsterScript::Initialize();

		mTransform->SetScale(Vector3(73.f, 61.f, 1.f)); // backsize�� �����ϰ� �����ϰ� �ֱ�. 


		//
		mCharacterState->SetCurrentHP(110.f);
		mCharacterState->SetMaxHP(110.f);

		mFirstPos = GetOwner()->GetComponent<Transform>()->GetPosition();

		//
		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Archer_Idle");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Archer_Run");
		std::shared_ptr<ss::graphics::Texture> Image3 = Resources::Find<ss::graphics::Texture>(L"Archer_Hit");
		std::shared_ptr<ss::graphics::Texture> Image4 = Resources::Find<ss::graphics::Texture>(L"Archer_NearAttack");
		std::shared_ptr<ss::graphics::Texture> Image5 = Resources::Find<ss::graphics::Texture>(L"Archer_FarAttack");
		std::shared_ptr<ss::graphics::Texture> Image6 = Resources::Find<ss::graphics::Texture>(L"Archer_Stun");
		std::shared_ptr<ss::graphics::Texture> Image7 = Resources::Find<ss::graphics::Texture>(L"Archer_Die");

		// �÷��̾� �ִϸ��̼��� �� Ư���ϹǷ�, ���� ���� ������ �� �ٸ��� �ֵ� ��������	 �����ϰ� �ֱ�. (������ ���� Ʋ���� ��) 
		// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
		//  LT, 1������ �縦 ������, �ڸ� ����, ������, , ����ð� ������
		// === 
		// �÷��̾� �ִϸ��̼��� �� Ư���ϹǷ�, ���� ���� ������ �� �ٸ��� �ֵ� ��������	 �����ϰ� �ֱ�. (������ ���� Ʋ���� ��) 
		// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
		//  LT, 1������ �縦 ������, �ڸ� ����, �������, ������
		mAnimator->Create(L"Archer_IdleR", Image1, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 4, Vector2(73.f, 61.f));

		mAnimator->Create(L"Archer_RunR", Image2, Vector2(0.f, 0.f), Vector2(73.f, 61.f),8, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_RunL", Image2, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 8, Vector2(73.f, 61.f), Vector2(0.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Archer_HitR", Image3, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 1, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_HitL", Image3, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 1, Vector2(73.f, 61.f), Vector2(0.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Archer_NearAttackR", Image4, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 12, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_NearAttackL", Image4, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 12, Vector2(73.f, 61.f), Vector2(0.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Archer_FarAttackR", Image5, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 12, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_FarAttackL", Image5, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 12, Vector2(73.f, 61.f), Vector2(0.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Archer_StunR", Image6, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 5, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_StunL", Image6, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 5, Vector2(73.f, 61.f), Vector2(0.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Archer_DieR", Image7, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 17, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_DieL", Image7, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 17, Vector2(73.f, 61.f));

		// �ϴ� Idle ���´� �������� ����� 
	
		// ======
			// �ִϸ��̼� ���⿡ ���� ����	�� ����ش�.
		mDir = mTransform->Right();
		mCurDir = mTransform->Right();
		mCurState = eMonsterState::MOVE;

		// ===== �⺻ �浹ü Hit �޴� �뵵 
		mCollider->SetName(L"Archer_HitCol");
		mCollider->SetType(eColliderType::Rect);

		mCollider->SetSize(Vector2(0.4f, 0.9f));
		mCollider->SetCenter(Vector2(-6.f, -0.f));
		// === idle ������ ���� �浹ü (���߿� ��ġ Ȯ���ؼ� �̰ɷ� ��ġ��)
		//mCollider->SetSize(Vector2(0.4f, 0.9f));
		//mCollider->SetCenter(Vector2(-6.f, -0.f));



		//==== ���� ���� Ư�� �ε��� �浹ü 
		//�浹ü�� ���⼭ �ٷ� ���� ���� �ش� �ε��� �� �־��ٰ� ���� ������ �ϱ� 
		mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"ArcherAttackCollider");
		mAttackColliderObj->Initialize();
		mAttackColliderObj->AddComponent<ArcherColScript>();

		mAttackColTr = mAttackColliderObj->GetComponent<Transform>();



		// =====
		// Near ���� ������ �浹ü
		// ���� �Ǻ� �뵵�� etc ���̾ �ֱ� 
		mNearRangeColObj = object::Instantiate<RangeCollider>(eLayerType::Etc, L"ArcherNearRangeCol");
		mNearRangeColObj->Initialize();

		mNearTr = mNearRangeColObj->GetComponent<Transform>();

		ArcherNearRangeScript* nearscript = mNearRangeColObj->AddComponent<ArcherNearRangeScript>();
		nearscript->SetOwner(mTransform->GetOwner()); // ������� ������Ʈ�� �����صд�.


		mNearCol = mNearRangeColObj->GetComponent<Collider2D>();


		mNearCol->SetSize(Vector2(130.f, 20.f));
		mNearCol->SetCenter(Vector2(-1.f, 0.2f));



		// ���Ÿ� ���� ������ �浹ü
		mFarRangeColObj = object::Instantiate<RangeCollider>(eLayerType::Etc, L"ArcherFarRangeCol");
		mFarRangeColObj->Initialize();

		mFarTr = mFarRangeColObj->GetComponent<Transform>();

		ArcherFarRangeScript* farScript = mFarRangeColObj->AddComponent<ArcherFarRangeScript>();
		farScript->SetOwner(mTransform->GetOwner()); // ������� ������Ʈ�� �����صд�.


		mFarCol = mFarRangeColObj->GetComponent<Collider2D>();


		mFarCol->SetSize(Vector2(200.f, 20.f));
		mFarCol->SetCenter(Vector2(0.f, 0.2f));




	}



	void SkeletonArcherScript::Update()
	{
		// �̵�->���º�ȯ->�ִϸ��̼�



		// ���� ���� ���� ��ġ ������Ű�� ���ؼ� (���Ͱ� �÷��̾��� ��ġ�� ����Ǹ� �ִϸ��̼��� ���µǴ� ���� ����)
		if (!mbAttacking)
		{
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
		mPrevDir = mCurDir;


	}
	void SkeletonArcherScript::LateUpdate()
	{
		mAttackColTr->SetPosition(mTransform->GetPosition());
		mNearTr->SetPosition(mTransform->GetPosition());
		mFarTr->SetPosition(mTransform->GetPosition());


	}
	void SkeletonArcherScript::OnCollisionEnter(Collider2D* other)
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
	void SkeletonArcherScript::OnCollisionStay(Collider2D* other)
	{
	}
	void SkeletonArcherScript::OnCollisionExit(Collider2D* other)
	{
	}
	void SkeletonArcherScript::Move()
	{

		// ������ �ִϸ��̼��� ����Ѵ�. 
		if (mDir.x > 0)
		{
			mAnimator->PlayAnimation(L"Archer_RunR", true);
			mCollider->SetSize(Vector2(0.4f, 0.9f));
			mCollider->SetCenter(Vector2(-6.f, -0.f));
		}

		else if (mDir.x < 0)
		{
			mAnimator->PlayAnimation(L"Archer_RunL", true);
			mCollider->SetSize(Vector2(0.4f, 0.9f));
			mCollider->SetCenter(Vector2(6.f, -0.f));
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
			mDir = -mTransform->Right(); // ���� �� �ο� 
		}

		mTransform->SetPosition(MonsterPos);


	

	}
	void SkeletonArcherScript::Tracer()
	{
	}
	void SkeletonArcherScript::Jump()
	{
	}
	void SkeletonArcherScript::Fall()
	{
	}
	void SkeletonArcherScript::Landing()
	{
	}
	void SkeletonArcherScript::Stun()
	{
		// ������ �ִϸ��̼��� ����Ѵ�. 
		{
			if (mCurDir.x > 0)
				mAnimator->PlayAnimation(L"Archer_StunR", false);

			else
				mAnimator->PlayAnimation(L"Archer_StunL", false);
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
	void SkeletonArcherScript::Hit()
	{
		// ������ �ִϸ��̼��� ����Ѵ�. 
		if (mCurDir.x > 0)
			mAnimator->PlayAnimation(L"Archer_HitR", false);

		else
			mAnimator->PlayAnimation(L"Archer_HitL", false);


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
		}
	}
	void SkeletonArcherScript::Guard()
	{
	}
	void SkeletonArcherScript::NearAttack()
	{
		{
			PlayerScript* playerScript = mPlayer->GetComponent<PlayerScript>();

			if (mCurDir.x > 0)
			{

				if (mAnimator->GetCurActiveAnimation()->GetIndex() == 9)
				{
					mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();

					mAttackCol->SetSize(Vector2(23.f, 30.f));
					mAttackCol->SetCenter(Vector2(20.f, -5.f));

					// �뽬 �߿� �ƿ� �浹 �ȵǰ� �ع��� 
					if (playerScript->IsDash())
					{
						mAttackColliderObj->RemoveComponent<Collider2D>();
					}
				}


				if (mAnimator->GetCurActiveAnimation()->GetIndex() == 10)
				{
					mAttackColliderObj->RemoveComponent<Collider2D>();

				}


				mAnimator->PlayAnimation(L"Archer_NearAttackR", true);


			}

			else if (mCurDir.x < 0)
			{

				if (mAnimator->GetCurActiveAnimation()->GetIndex() == 9)
				{
					mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
					mAttackCol->SetSize(Vector2(-23.f, 30.f));
					mAttackCol->SetCenter(Vector2(-22.f, -5.f));

					// �뽬 �߿� �ƿ� �浹 �ȵǰ� �ع��� 
					if (playerScript->IsDash())
					{
						mAttackColliderObj->RemoveComponent<Collider2D>();
					}
				}

				if (mAnimator->GetCurActiveAnimation()->GetIndex() == 10)
				{
					mAttackColliderObj->RemoveComponent<Collider2D>();

				}

				mAnimator->PlayAnimation(L"Archer_NearAttackL", true);

			}
		}


	}
	void SkeletonArcherScript::FarAttack()
	{
		Vector3 pos = mTransform->GetPosition();
		pos.z -= 0.01;

		{
			if (mAnimator->GetCurActiveAnimation()->GetIndex() == 8)
			{

				m_fTime += (float)Time::DeltaTime();

				// coolDown �ʸ��� �߻� 
				if (m_fTime >= m_tMonsterInfo.m_fCoolDown)
				{

					if (mCurDir.x == 1.0f)
					{
						// �߻�ü ��ġ ���� 
						pos += Vector3(35.f, -2.5f, 0.f);
						mArrowObj = object::Instantiate<ArcherArrow>(pos, eLayerType::Mon_Bullet, L"ArcherArrowObj_R");
						mArrowObj->GetComponent<ArcherArrowScript>()->SetOriginOwner(mTransform->GetOwner());

					}

					else if (mCurDir.x == -1.0f)
					{
						// �߻�ü ��ġ ���� 
						pos -= Vector3(35.f, 2.5f, 0.f);

						mArrowObj = object::Instantiate<ArcherArrow>(pos, eLayerType::Mon_Bullet, L"ArcherArrowObj_L");
						mArrowObj->GetComponent<ArcherArrowScript>()->SetOriginOwner(mTransform->GetOwner());
						mArrowTr = mArrowObj->GetComponent<Transform>();
						mArrowTr->SetScale(Vector3(-25.f, 8.f, 0.f));
					}
					m_fTime = 0.0f;

				}

			}

			if (mCurDir.x > 0)
			{
				mAnimator->PlayAnimation(L"Archer_FarAttackR", true);
				mCollider->SetSize(Vector2(0.4f, 0.9f));
				mCollider->SetCenter(Vector2(6.f, -0.f));

			}

			else
			{
				mAnimator->PlayAnimation(L"Archer_FarAttackL", true);
				mCollider->SetSize(Vector2(0.4f, 0.9f));
				mCollider->SetCenter(Vector2(6.f, -0.f));
			}

		}
	}
	void SkeletonArcherScript::Dead()
	{
		if (mCurDir.x > 0)
		{
			mAnimator->PlayAnimation(L"Archer_DieR", false);
			mCollider->SetSize(Vector2(0.19f, 0.33f));
			mCollider->SetCenter(Vector2(-32.f, 0.f));

		}

		else
		{
			mAnimator->PlayAnimation(L"Archer_DieL", false);
			mCollider->SetSize(Vector2(0.19f, 0.33f));
			mCollider->SetCenter(Vector2(-37.f, 0.f));
		}

		// �ִϸ��̼� ����� ������ 
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			mNearRangeColObj->SetState(GameObject::eState::Dead);
			mAttackColliderObj->SetState(GameObject::eState::Dead);
			mFarRangeColObj->SetState(GameObject::eState::Dead);

			GetOwner()->SetState(GameObject::eState::Dead);
		}
	}
	void SkeletonArcherScript::Animation()
	{
	}
	void SkeletonArcherScript::FarAttackEnd()
	{
	}
	void SkeletonArcherScript::StunEnd()
	{
	}
}