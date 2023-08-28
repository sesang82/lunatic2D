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


namespace ss
{
	WoodGolemScript::WoodGolemScript()
	{
		m_tMonsterInfo.m_fSpeed = 50.f;
		m_tMonsterInfo.m_fNearAttackRange = 25.f;
		m_tMonsterInfo.m_fDetectRange = 180.f;
		m_tMonsterInfo.m_fCoolDown = 0.1f;
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

		mAnimator->Create(L"Wood_NearAttackR", Image3, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 13, Vector2(111.f, 83.f));
		mAnimator->Create(L"Wood_NearAttackL", Image3, Vector2(0.f, 0.f), Vector2(111.f, 83.f), 13, Vector2(111.f, 83.f), Vector2(23.f, 0.f), 0.1f, true);

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
		mCollider->SetName(L"Archer_HitCol");
		mCollider->SetType(eColliderType::Rect);


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
	void WoodGolemScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void WoodGolemScript::OnCollisionStay(Collider2D* other)
	{
	}
	void WoodGolemScript::OnCollisionExit(Collider2D* other)
	{
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




		//// ���� ���� ���� ���� �÷��̾ ������ NearAttack ���·� ��ȯ
		//if (distance < m_tMonsterInfo.m_fNearAttackRange)
		//{
		//	ChangeState(eMonsterState::NEARATTACK);
		//	return; // �� �Լ����� �߰����� ó���� �����մϴ�.
		//}

		// Ž�� �Ÿ��� ����� �ʱ� ��ġ�� ���ư���. 


			if (mDir.x > 0)
			{
				mAnimator->PlayAnimation(L"Wood_RunR", true);
				mCollider->SetSize(Vector2(0.7f, 0.7f));
				mCollider->SetCenter(Vector2(-12.f, 0.f));
			}

			else
			{
				mAnimator->PlayAnimation(L"Wood_RunL", true);
				mCollider->SetSize(Vector2(0.7f, 0.7f));
				mCollider->SetCenter(Vector2(-12.f, 0.f));
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
	}
	void WoodGolemScript::HitAfter()
	{
	}
	void WoodGolemScript::NearAttackStart()
	{
	}
	void WoodGolemScript::NearAttack()
	{
	}
	void WoodGolemScript::NearAttackAfter()
	{
	}
	void WoodGolemScript::NearAttackEnd()
	{
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