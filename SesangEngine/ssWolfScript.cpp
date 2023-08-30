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
	// ===== �ؾ��� ��
	// 1. Far �ݰ��� �ſ� �о��� �� ���� Velocity���� ��ġ ���� �ʾƼ� velocity���� �Ÿ��� ���� �����ϴ� ��� �ʿ�


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
		m_tMonsterInfo.m_fDetectRange = 200.f;

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

		mTransform->SetScale(Vector3(102.f, 47.f, 1.f)); // backsize�� �����ϰ� �����ϰ� �ֱ�. 


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


		// �÷��̾� �ִϸ��̼��� �� Ư���ϹǷ�, ���� ���� ������ �� �ٸ��� �ֵ� ��������	 �����ϰ� �ֱ�. (������ ���� Ʋ���� ��) 
		// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
		//  LT, 1������ �縦 ������, �ڸ� ����, ������, , ����ð� ������
		// === 
		// �÷��̾� �ִϸ��̼��� �� Ư���ϹǷ�, ���� ���� ������ �� �ٸ��� �ֵ� ��������	 �����ϰ� �ֱ�. (������ ���� Ʋ���� ��) 
		// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
		//  LT, 1������ �縦 ������, �ڸ� ����(1���� �����ؼ� ����), �������, ������
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
		// �ִϸ��̼� ���⿡ ���� ����	�� ����ش�.
		mDir = mTransform->Right();
		mCurDir = mTransform->Right();
		mCurState = eMonsterState::IDLE;

		// ===== �⺻ �浹ü Hit �޴� �뵵 
		mCollider->SetName(L"Wolf_HitCol");
		mCollider->SetType(eColliderType::Rect);
		mCollider->SetSize(Vector2(0.3f, 0.9f));
		mCollider->SetCenter(Vector2(8.f, -5.f));

		// ===== ������ٵ�



		//==== ���� ���� Ư�� �ε��� �浹ü 
		//�浹ü�� ���⼭ �ٷ� ���� ���� �ش� �ε��� �� �־��ٰ� ���� ������ �ϱ� 
		mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"WolfAttackColObj");
		mAttackColliderObj->Initialize();
		//mAttackColliderObj->AddComponent<LizardColScript>();

		mAttackColTr = mAttackColliderObj->GetComponent<Transform>();




	}
	void WolfScript::Update()
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
	void WolfScript::LateUpdate()
	{
	}
	void WolfScript::OnCollisionEnter(Collider2D* other)
	{
		if (L"col_SpecialFloor" == other->GetOwner()->GetName())
		{
			mRigidbody->SetGround(true);

			// �÷��̾��� ��ġ�� ���� �´�.
			// �ش� �浹ü�� ��ġ�� ���� �´�. (�浹ü�̱� ���� ���ӿ�����Ʈ) 
			mRigidbody->SetVelocity(Vector2(0.0f, 0.0f)); // ������ ��ġ��  �ӵ� 0���� �صα� 
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
		mbNearAttacking = false;
		mbFarAttacking = false;
		mbHit = false;

		if (mCurDir.x > 0)
		{
			mAnimator->PlayAnimation(L"Wolf_IdleR", true);
		}

		else
		{
			mAnimator->PlayAnimation(L"Wolf_IdleL", true);
		}



		Vector3 MonsterPos = mTransform->GetPosition();
		Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();

		// ���Ϳ� �÷��̾� ���� �Ÿ��� ���� 
		Vector3 vDir = MonsterPos - PlayerPos;
		vDir.z = 0;
		float distance = vDir.Length();


		if (distance < m_tMonsterInfo.m_fNearAttackRange)
		{
			ChangeState(eMonsterState::NEARATTACK);
		}



		else if (distance < m_tMonsterInfo.m_fFarAttackRange)
		{
			ChangeState(eMonsterState::FARATTACK);
		}


		// �հŸ� ���� ���� ���� �÷��̾ ������ FarAttack ���·� ��ȯ 
		else if (distance <= m_tMonsterInfo.m_fDetectRange)
		{
			// �÷��̾ Ž�� ���� ���� ������ ���� ���� ���� �ۿ� ������ �̵� ���·� ��ȯ
			ChangeState(eMonsterState::MOVE);
		}


	}
	void WolfScript::Move()
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
			return; // �ؿ� run���� �۵� �ȵǵ��� 
		}


		// �հŸ� ���� ���� ���� �÷��̾ ������ FarAttack ���·� ��ȯ
		else if (distance < m_tMonsterInfo.m_fFarAttackRange)
		{
			ChangeState(eMonsterState::FARATTACK);
			return;
		}


		// ======================= tracer

		// PLAYER�� Ž�� ���� �̳��� ������ ���� ���·� ��ȯ 
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

			// ���Ϳ��� �÷��̾������ �Ÿ��� ���Ѵ�. 
			Vector3 vMonToPlayer = PlayerPos - MonsterPos; // �� ���� �÷��̾���� ������ gpt���� �����
			vMonToPlayer.z = 0;
			vMonToPlayer.Normalize();

			float fSpeed = m_tMonsterInfo.m_fSpeed + 100.f; // ���� ���� �� �� ������ �����ϰ� �� 

			// ������ ��ġ�� �÷��̾� �������� �̵���Ų��.
			MonsterPos.x += vMonToPlayer.x * fSpeed * Time::DeltaTime();

			mTransform->SetPosition(MonsterPos);


		}

		// Ž�� �Ÿ��� ����� �ʱ� ��ġ�� ���ư���. 
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
	}
	void WolfScript::NearAttack()
	{
		Vector3 MonsterPos = mTransform->GetPosition();
		Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
		float distance = (PlayerPos - MonsterPos).Length();

		m_fTime += Time::DeltaTime();


		// ���̾� ���� �ִϸ��̼� ������� �ʰ�, ��Ÿ�� �ð���ŭ ��ٷȴٰ� ���� 


		if (m_fTime >= m_tMonsterInfo.m_fCoolDown && !mbNearAttacking)
		{
			mbNearAttacking = true;


			if (mCurDir.x > 0)
			{
				mAnimator->PlayAnimation(L"Wolf_NearAttackR", false);

			}

			else
			{
				mAnimator->PlayAnimation(L"Wolf_NearAttackL", false);
			}


			m_fTime = 0.0f;
		}


		if (mAnimator->GetCurActiveAnimation()->GetIndex() == 10)
		{
			mbNearAttacking = false;
		}


		if (!mbNearAttacking && mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eMonsterState::IDLE);
		}

	}
	void WolfScript::FarAttack()
	{

		Vector3 MonsterPos = mTransform->GetPosition();
		Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
		float distance = (PlayerPos - MonsterPos).Length();

		m_fTime += Time::DeltaTime();


		// ���̾� ���� �ִϸ��̼� ������� �ʰ�, ��Ÿ�� �ð���ŭ ��ٷȴٰ� ���� 


		if (m_fTime >= m_tMonsterInfo.m_fCoolDown && !mbFarAttacking)
		{
			mbFarAttacking = true; // �ִϸ��̼� ����� ĳ���Ͱ� ������ �ٲ㵵 ������ �����Ǿ���ؼ� �־��� ���� 


			if (mCurDir.x > 0)
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
			mLandingPos = PlayerPos; // ������ ��ġ�� ���� �����ӿ����� �����ؾ��ϹǷ� ��Ƶ� 
			
			Vector3 jumpdir = (PlayerPos - MonsterPos); 
			jumpdir.Normalize();
			mJumpDir = Vector2(jumpdir.x, jumpdir.y); // �븻�������ؼ� ���� ���� ���� ���� ��Ƶ� (�÷��̾�� ���� ���) 

			HitGroundScript* script = mHitGround->AddComponent<HitGroundScript>();
			script->SetMonsterOwner((Monster*)mTransform->GetOwner());

			mbJumped = true;
		}

		// �÷��̾� ��ġ�� ���� ���� 
		else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 9)
		{
			mRigidbody->SetGravity(Vector2(0.f, 1300.f));

			float JumpPower = sqrt(2 * abs(mRigidbody->GetGravity().y) * 10.f); // * ���� ����

			mRigidbody->SetVelocity(Vector2(50.0f, -JumpPower));

			// velocity���� ���⿡ ���� �ٸ��� �ֱ� 
			mVelocity = mRigidbody->GetVelocity();


			// ���� ���� �����ֱ� (�÷��̾� ������ �����
			if (mJumpDir.x >= 0.f)
				mVelocity.x = 300.f;

			else if (mJumpDir.x < 0.f)
				mVelocity.x = -300.f;


			mRigidbody->SetVelocity(mVelocity);

			mRigidbody->SetGround(false);

		

			//mLandingPos.y = PlayerPos.y; // y���� �÷��̾� y������ ����
		}



		// 10�� ���ݿ� �浹ü �޾��ֱ� 
		else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 10)
		{

			// ���ݿ� �浹ü�� ������. �÷��̾� ���. 2 �ε������� ����� playerpos�� ��Ƽ� ���� 



		}

		// 13 �ε��� �� �浹ü ���� �� ����
	
		else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 13)
		{
			mRigidbody->SetGround(true);
			mRigidbody->SetVelocity(Vector2::Zero);
			mTransform->SetPosition(mLandingPos); // ������ ��ġ�� �̵�
		}

		else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 14) // end event ȣ�� �ȵǼ� ��� �� 
		{
			mbFarAttacking = false;
		}


		if (!mbFarAttacking && mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eMonsterState::IDLE);
		}


	}
	void WolfScript::Dead()
	{
	}
	void WolfScript::Animation()
	{
	}
}