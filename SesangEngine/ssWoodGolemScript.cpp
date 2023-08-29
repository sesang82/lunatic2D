#include "ssWoodGolemScript.h"
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
	WoodGolemScript::WoodGolemScript()
		: mbNearAttacking(false)
		, mbFarAttacking(false)
		, mbHit(false)
		, mbPaunched(false)
	{
		m_tMonsterInfo.m_fSpeed = 50.f;
		m_tMonsterInfo.m_fNearAttackRange = 110.f;
		m_tMonsterInfo.m_fFarAttackRange = 150.f;
		m_tMonsterInfo.m_fCoolDown = 0.5f;
	}
	WoodGolemScript::~WoodGolemScript()
	{
	}
	void WoodGolemScript::Initialize()
	{
		MonsterScript::Initialize();

		mTransform->SetScale(Vector3(111.f, 83.f, 1.f)); // backsize�� �����ϰ� �����ϰ� �ֱ�. 


		//
		mCharacterState->SetCurrentHP(110.f);
		mCharacterState->SetMaxHP(110.f);

		mFirstPos = GetOwner()->GetComponent<Transform>()->GetPosition();

		//
		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Wood_Idle");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Wood_Run");
		std::shared_ptr<ss::graphics::Texture> Image3 = Resources::Find<ss::graphics::Texture>(L"Wood_NearAttack");
		std::shared_ptr<ss::graphics::Texture> Image4 = Resources::Find<ss::graphics::Texture>(L"Wood_FarAttack");
		std::shared_ptr<ss::graphics::Texture> Image5 = Resources::Find<ss::graphics::Texture>(L"Wood_Hit");
		std::shared_ptr<ss::graphics::Texture> Image6 = Resources::Find<ss::graphics::Texture>(L"Wood_Stun");
		std::shared_ptr<ss::graphics::Texture> Image7 = Resources::Find<ss::graphics::Texture>(L"Wood_Die");


		// �÷��̾� �ִϸ��̼��� �� Ư���ϹǷ�, ���� ���� ������ �� �ٸ��� �ֵ� ��������	 �����ϰ� �ֱ�. (������ ���� Ʋ���� ��) 
		// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
		//  LT, 1������ �縦 ������, �ڸ� ����, ������, , ����ð� ������
		// === 
		// �÷��̾� �ִϸ��̼��� �� Ư���ϹǷ�, ���� ���� ������ �� �ٸ��� �ֵ� ��������	 �����ϰ� �ֱ�. (������ ���� Ʋ���� ��) 
		// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
		//  LT, 1������ �縦 ������, �ڸ� ����(1���� �����ؼ� ����), �������, ������
		mAnimator->Create(L"Wood_IdleR", Image1, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 5, Vector2(111.f, 83.f));
		mAnimator->Create(L"Wood_IdleL", Image1, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 5, Vector2(111.f, 83.f), Vector2(23.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Wood_RunR", Image2, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 10, Vector2(111.f, 83.f));
		mAnimator->Create(L"Wood_RunL", Image2, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 10, Vector2(111.f, 83.f), Vector2(23.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Wood_NearAttackR", Image3, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 13, Vector2(111.f, 83.f), Vector2::Zero, 0.08f);
		mAnimator->Create(L"Wood_NearAttackL", Image3, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 13, Vector2(111.f, 83.f), Vector2(23.f, 0.f), 0.08f, true);

		mAnimator->Create(L"Wood_FarAttackR", Image4, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 13, Vector2(111.f, 83.f));
		mAnimator->Create(L"Wood_FarAttackL", Image4, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 13, Vector2(111.f, 83.f), Vector2(23.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Wood_HitR", Image5, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 2, Vector2(111.f, 83.f));
		mAnimator->Create(L"Wood_HitL", Image5, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 2, Vector2(111.f, 83.f), Vector2(23.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Wood_StunR", Image6, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 4, Vector2(111.f, 83.f));
		mAnimator->Create(L"Wood_StunL", Image6, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 4, Vector2(111.f, 83.f), Vector2(23.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Wood_DieR", Image7, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 9, Vector2(111.f, 83.f));
		mAnimator->Create(L"Wood_DieL", Image7, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 9, Vector2(111.f, 83.f), Vector2(5.f, 0.f), 0.1f, true);

		// ======
			// �ִϸ��̼� ���⿡ ���� ����	�� ����ش�.
		mDir = mTransform->Right();
		mCurDir = mTransform->Right();
		mCurState = eMonsterState::MOVE;

		// ===== �⺻ �浹ü Hit �޴� �뵵 
		mCollider->SetName(L"Wood_HitCol");
		mCollider->SetType(eColliderType::Rect);
		mCollider->SetSize(Vector2(0.3f, 0.7f));
		mCollider->SetCenter(Vector2(-12.f, 0.f));


		//==== ���� ���� Ư�� �ε��� �浹ü 
		//�浹ü�� ���⼭ �ٷ� ���� ���� �ش� �ε��� �� �־��ٰ� ���� ������ �ϱ� 
		mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"WoodAttackColObj");
		mAttackColliderObj->Initialize();
		//mAttackColliderObj->AddComponent<LizardColScript>();

		mAttackColTr = mAttackColliderObj->GetComponent<Transform>();


	}
	void WoodGolemScript::Update()
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
	void WoodGolemScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void WoodGolemScript::OnCollisionStay(Collider2D* other)
	{
	}
	void WoodGolemScript::OnCollisionExit(Collider2D* other)
	{
	}

	void WoodGolemScript::Idle()
	{
		mbNearAttacking = false;
		mbFarAttacking = false;
		mbHit = false;

		if (mCurDir.x > 0)
		{
			mAnimator->PlayAnimation(L"Wood_IdleR", true);
		}

		else
		{
			mAnimator->PlayAnimation(L"Wood_IdleL", true);
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


		// �հŸ� ���� ���� ���� �÷��̾ ������ FarAttack ���·� ��ȯ
		else if (distance < m_tMonsterInfo.m_fFarAttackRange)
		{
			ChangeState(eMonsterState::FARATTACK);
		}

	}

	// ���� ���� ���� ���� 
	void WoodGolemScript::Move()
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


			if (mDir.x > 0)
			{
				mAnimator->PlayAnimation(L"Wood_RunR", true);	
			}

			else
			{
				mAnimator->PlayAnimation(L"Wood_RunL", true);
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
	void WoodGolemScript::Tracer()
	{
	}
	void WoodGolemScript::Jump()
	{
	}
	void WoodGolemScript::Fall()
	{
	}
	void WoodGolemScript::Landing()
	{
	}
	void WoodGolemScript::Stun()
	{
	}
	void WoodGolemScript::Hit()
	{
		if (!mbHit)
		{
			mbHit = true;

			if (mCurDir.x > 0)
			{
				mAnimator->PlayAnimation(L"Wood_HitR", false);
			}

			else
			{
				mAnimator->PlayAnimation(L"Wood_HitL", false);
			}
		}

		if (mbHit && mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eMonsterState::IDLE);
		}

	}

	void WoodGolemScript::NearAttack()
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
					mAnimator->PlayAnimation(L"Wood_NearAttackR", false);

				}

				else
				{
					mAnimator->PlayAnimation(L"Wood_NearAttackL", false);
				}

			
			m_fTime = 0.0f;
		}


		if (mAnimator->GetCurActiveAnimation()->GetIndex() == 12)
		{
			mbNearAttacking = false;
		}


		if (!mbNearAttacking && mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(eMonsterState::IDLE);
		}


	}


	void WoodGolemScript::FarAttack()
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
				mAnimator->PlayAnimation(L"Wood_FarAttackR", false);

			}

			else
			{
				mAnimator->PlayAnimation(L"Wood_FarAttackL", false);
			}


			m_fTime = 0.0f;
		}


		bool isGround = mPlayer->GetComponent<Rigidbody2D>()->IsGround();

		if (mAnimator->GetCurActiveAnimation()->GetIndex() == 1 && !mbPaunched && isGround)
		{

			mHitGround = object::Instantiate<Effect>(PlayerPos, eLayerType::Effect, L"WoodHitGroundObj");

			HitGroundScript* script = mHitGround->AddComponent<HitGroundScript>();
			script->SetMonsterOwner((Monster*)mTransform->GetOwner());

			mbPaunched = true;
		}



		else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 6)
		{

			// ���ݿ� �浹ü�� ������. �÷��̾� ���. 3 �ε������� ����� playerpos�� ��Ƽ� ���� 



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
	void WoodGolemScript::Dead()
	{
	}
	void WoodGolemScript::Animation()
	{
	}
	void WoodGolemScript::FarAttackEnd()
	{
	}
	void WoodGolemScript::StunEnd()
	{
	}
}