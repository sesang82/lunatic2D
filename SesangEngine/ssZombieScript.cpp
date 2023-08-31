#include "ssZombieScript.h"
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
	ZombieScript::ZombieScript()
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
	ZombieScript::~ZombieScript()
	{
	}
	void ZombieScript::Initialize()
	{
		MonsterScript::Initialize();

		mTransform->SetScale(Vector3(89.f, 78.f, 1.f)); // backsize�� �����ϰ� �����ϰ� �ֱ�. 


		//
		mCharacterState->SetCurrentHP(110.f);
		mCharacterState->SetMaxHP(110.f);

		mFirstPos = GetOwner()->GetComponent<Transform>()->GetPosition();

		//
		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Zombie_Idle");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Zombie_Run");
		std::shared_ptr<ss::graphics::Texture> Image3 = Resources::Find<ss::graphics::Texture>(L"Zombie_NearAttack");
		std::shared_ptr<ss::graphics::Texture> Image4 = Resources::Find<ss::graphics::Texture>(L"Zombie_FarAttack");
		std::shared_ptr<ss::graphics::Texture> Image5 = Resources::Find<ss::graphics::Texture>(L"Zombie_Hit");
		std::shared_ptr<ss::graphics::Texture> Image6 = Resources::Find<ss::graphics::Texture>(L"Zombie_Stun");
		std::shared_ptr<ss::graphics::Texture> Image7 = Resources::Find<ss::graphics::Texture>(L"Zombie_Die");


		// �÷��̾� �ִϸ��̼��� �� Ư���ϹǷ�, ���� ���� ������ �� �ٸ��� �ֵ� ��������	 �����ϰ� �ֱ�. (������ ���� Ʋ���� ��) 
		// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
		//  LT, 1������ �縦 ������, �ڸ� ����, ������, , ����ð� ������
		// === 
		// �÷��̾� �ִϸ��̼��� �� Ư���ϹǷ�, ���� ���� ������ �� �ٸ��� �ֵ� ��������	 �����ϰ� �ֱ�. (������ ���� Ʋ���� ��) 
		// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
		//  LT, 1������ �縦 ������, �ڸ� ����(1���� �����ؼ� ����), �������, ������
		mAnimator->Create(L"Zombie_IdleR", Image1, Vector2(0.f, 0.f), Vector2(89.f, 78.f), 5, Vector2(89.f, 78.f));
		mAnimator->Create(L"Zombie_IdleL", Image1, Vector2(0.f, 0.f), Vector2(89.f, 78.f), 5, Vector2(89.f, 78.f), Vector2(-16.f, 0.f), 0.1f, true);
		
		mAnimator->Create(L"Zombie_RunR", Image2, Vector2(0.f, 0.f), Vector2(89.f, 78.f), 8, Vector2(89.f, 78.f));
		mAnimator->Create(L"Zombie_RunL", Image2, Vector2(0.f, 0.f), Vector2(89.f, 78.f), 8, Vector2(89.f, 78.f), Vector2(-12.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Zombie_NearAttackR", Image3, Vector2(0.f, 0.f), Vector2(89.f, 78.f), 11, Vector2(89.f, 78.f), Vector2::Zero, 0.1f);
		mAnimator->Create(L"Zombie_NearAttackL", Image3, Vector2(0.f, 0.f), Vector2(89.f, 78.f), 11, Vector2(89.f, 78.f), Vector2(0.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Zombie_FarAttackR", Image4, Vector2(0.f, 0.f), Vector2(89.f, 78.f), 13, Vector2(89.f, 78.f));
		mAnimator->Create(L"Zombie_FarAttackL", Image4, Vector2(0.f, 0.f), Vector2(89.f, 78.f), 13, Vector2(89.f, 78.f), Vector2(-12.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Zombie_HitR", Image5, Vector2(0.f, 0.f), Vector2(89.f, 78.f), 1, Vector2(89.f, 78.f), Vector2::Zero, 0.3f);
		mAnimator->Create(L"Zombie_HitL", Image5, Vector2(0.f, 0.f), Vector2(89.f, 78.f), 1, Vector2(89.f, 78.f), Vector2(-12.f, 0.f), 0.3f, true);

		mAnimator->Create(L"Zombie_StunR", Image6, Vector2(0.f, 0.f), Vector2(89.f, 78.f), 5, Vector2(89.f, 78.f));
		mAnimator->Create(L"Zombie_StunL", Image6, Vector2(0.f, 0.f), Vector2(89.f, 78.f), 5, Vector2(89.f, 78.f), Vector2(-12.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Zombie_DieR", Image7, Vector2(0.f, 0.f), Vector2(89.f, 78.f), 15, Vector2(89.f, 78.f));
		mAnimator->Create(L"Zombie_DieL", Image7, Vector2(0.f, 0.f), Vector2(89.f, 78.f), 15, Vector2(89.f, 78.f), Vector2(-12.f, 0.f), 0.1f, true);




		// ======
		// �ִϸ��̼� ���⿡ ���� ����	�� ����ش�.
		mDir = mTransform->Right();
		mCurDir = mTransform->Right();
		mCurState = eMonsterState::IDLE;

		// ===== �⺻ �浹ü Hit �޴� �뵵 
		mCollider->SetName(L"Zombie_HitCol");
		mCollider->SetType(eColliderType::Rect);
		mCollider->SetSize(Vector2(0.3f, 0.9f));
		mCollider->SetCenter(Vector2(8.f, -5.f));

		// ===== ������ٵ�



		//==== ���� ���� Ư�� �ε��� �浹ü 
		//�浹ü�� ���⼭ �ٷ� ���� ���� �ش� �ε��� �� �־��ٰ� ���� ������ �ϱ� 
		mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"ZombiAttackColObj");
		mAttackColliderObj->Initialize();
		//mAttackColliderObj->AddComponent<LizardColScript>();

		mAttackColTr = mAttackColliderObj->GetComponent<Transform>();


	}
	void ZombieScript::Update()
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
	void ZombieScript::LateUpdate()
	{
	}
	void ZombieScript::OnCollisionEnter(Collider2D* other)
	{
		if (L"col_SpecialFloor" == other->GetOwner()->GetName())
		{
			mRigidbody->SetGround(true);

			// �÷��̾��� ��ġ�� ���� �´�.
			// �ش� �浹ü�� ��ġ�� ���� �´�. (�浹ü�̱� ���� ���ӿ�����Ʈ) 
			mRigidbody->SetVelocity(Vector2(0.0f, 0.0f)); // ������ ��ġ��  �ӵ� 0���� �صα� 
		}
	}
	void ZombieScript::OnCollisionStay(Collider2D* other)
	{
	}
	void ZombieScript::OnCollisionExit(Collider2D* other)
	{
		
	}
	void ZombieScript::Idle()
	{
		mbNearAttacking = false;
		mbFarAttacking = false;
		mbHit = false;

		if (mCurDir.x > 0)
		{
			mAnimator->PlayAnimation(L"Zombie_StunR", true);
		}

		else
		{
			mAnimator->PlayAnimation(L"Zombie_StunL", true);
		}



		Vector3 MonsterPos = mTransform->GetPosition();
		Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();

		// ���Ϳ� �÷��̾� ���� �Ÿ��� ���� 
		Vector3 vDir = MonsterPos - PlayerPos;
		vDir.z = 0;
		float distance = vDir.Length();


		//if (distance < m_tMonsterInfo.m_fNearAttackRange)
		//{
		//	ChangeState(eMonsterState::NEARATTACK);
		//}



		//else if (distance < m_tMonsterInfo.m_fFarAttackRange)
		//{
		//	ChangeState(eMonsterState::FARATTACK);
		//}


		//// �հŸ� ���� ���� ���� �÷��̾ ������ FarAttack ���·� ��ȯ 
		//else if (distance <= m_tMonsterInfo.m_fDetectRange)
		//{
		//	// �÷��̾ Ž�� ���� ���� ������ ���� ���� ���� �ۿ� ������ �̵� ���·� ��ȯ
		//	ChangeState(eMonsterState::MOVE);
		//}
	}
	void ZombieScript::Move()
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
				mAnimator->PlayAnimation(L"Zombie_RunR", true);
			}

			else
			{
				mAnimator->PlayAnimation(L"Zombie_RunL", true);
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
				mAnimator->PlayAnimation(L"Zombie_RunR", true);
			}

			else
			{
				mAnimator->PlayAnimation(L"Zombie_RunL", true);
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
	void ZombieScript::Jump()
	{
	}
	void ZombieScript::Fall()
	{
	}
	void ZombieScript::Landing()
	{
	}
	void ZombieScript::Stun()
	{
	}
	void ZombieScript::Hit()
	{
	}
	void ZombieScript::NearAttack()
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
				mAnimator->PlayAnimation(L"Zombie_NearAttackR", false);

			}

			else
			{
				mAnimator->PlayAnimation(L"Zombie_NearAttackL", false);
			}


			m_fTime = 0.0f;
		}

		// 5�ε����� ���ݿ� �浹ü �ߵ�

		if (mAnimator->GetCurActiveAnimation()->GetIndex() == 10)
		{
			mbNearAttacking = false;
		}


		if (!mbNearAttacking && mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eMonsterState::IDLE);
		}
	}
	void ZombieScript::FarAttack()
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
				mAnimator->PlayAnimation(L"Zombie_FarAttackR", false);

			}

			else
			{
				mAnimator->PlayAnimation(L"Zombie_FarAttackL", false);
			}


			m_fTime = 0.0f;
		}


		bool isGround = mPlayer->GetComponent<Rigidbody2D>()->IsGround();

		// �÷��̾� ��ġ�� ���� ���� 
		if (mAnimator->GetCurActiveAnimation()->GetIndex() == 5 && !mbJumped && isGround)
		{

			mHitGround = object::Instantiate<Effect>(PlayerPos, eLayerType::Effect, L"ZombieHitGroundObj");
			mLandingPos = PlayerPos; // ������ ��ġ�� ���� �����ӿ����� �����ؾ��ϹǷ� ��Ƶ� 

			Vector3 jumpdir = (PlayerPos - MonsterPos);
			jumpdir.Normalize();
			mJumpDir = Vector2(jumpdir.x, jumpdir.y); // �븻�������ؼ� ���� ���� ���� ���� ��Ƶ� (�÷��̾�� ���� ���) 

			HitGroundScript* script = mHitGround->AddComponent<HitGroundScript>();
			script->SetMonsterOwner((Monster*)mTransform->GetOwner());


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

			mbJumped = true;
		}



		// 8�� ���ݿ� �浹ü �޾��ֱ� 
		else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 8)
		{

			// ���ݿ� �浹ü�� ������. �÷��̾� ���. 2 �ε������� ����� playerpos�� ��Ƽ� ���� 
			mRigidbody->SetGround(true);
			mRigidbody->SetVelocity(Vector2::Zero);
			mTransform->SetPosition(mLandingPos); // ������ ��ġ�� �̵�


		}

		else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 12) // end event ȣ�� �ȵǼ� ��� �� 
		{
			mbFarAttacking = false;
		}


		if (!mbFarAttacking && mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eMonsterState::IDLE);
		}
	}
	void ZombieScript::Dead()
	{
	}
	void ZombieScript::Animation()
	{
	}
	void ZombieScript::FarAttackEnd()
	{
	}
	void ZombieScript::StunEnd()
	{
	}
}