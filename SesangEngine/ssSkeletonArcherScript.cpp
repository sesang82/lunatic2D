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


namespace ss
{
	SkeletonArcherScript::SkeletonArcherScript()
		: mbNearAttack(false)
		, mbFarAttack(false)

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

		mTransform->SetScale(Vector3(73.f, 61.f, 1.f)); // backsize랑 스케일값 동일하게 주기. 

		mCollider->SetName(L"colHit_player");
		mCollider->SetType(eColliderType::Rect);
		mCollider->SetSize(Vector2(0.4f, 0.9f));
		mCollider->SetCenter(Vector2(-6.f, -0.f));



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

		// 플레이어 애니메이션은 좀 특수하므로, 무기 별로 오프셋 값 다르게 주되 백사이즈는	 동일하게 주기. (사이즈 값은 틀려도 됨) 
		// 애니메이션 나중에 플레이어 다 완성되면 
		//  LT, 1프레임 사를 사이즈, 자를 갯수, 오프셋, , 재생시간 뒤집기
		// === 
		// 플레이어 애니메이션은 좀 특수하므로, 무기 별로 오프셋 값 다르게 주되 백사이즈는	 동일하게 주기. (사이즈 값은 틀려도 됨) 
		// 애니메이션 나중에 플레이어 다 완성되면 
		//  LT, 1프레임 사를 사이즈, 자를 갯수, 백사이즈, 오프셋
		mAnimator->Create(L"Archer_IdleR", Image1, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 4, Vector2(73.f, 61.f));

		mAnimator->Create(L"Archer_RunR", Image2, Vector2(0.f, 0.f), Vector2(73.f, 61.f),8, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_RunL", Image2, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 8, Vector2(73.f, 61.f), Vector2(0.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Archer_HitR", Image3, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 1, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_HitL", Image3, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 1, Vector2(73.f, 61.f), Vector2(0.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Archer_NearAttackR", Image4, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 13, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_NearAttackL", Image4, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 13, Vector2(73.f, 61.f), Vector2(0.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Archer_FarAttackR", Image5, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 12, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_FarAttackL", Image5, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 12, Vector2(73.f, 61.f), Vector2(0.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Archer_StunR", Image6, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 5, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_StunL", Image6, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 5, Vector2(73.f, 61.f), Vector2(0.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Archer_DieR", Image7, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 17, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_DieL", Image7, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 17, Vector2(73.f, 61.f));

		// 일단 Idle 상태는 나중으로 만들기 
	

		// ======
			// 애니메이션 방향에 관한 기준	을 잡아준다.
		mDir = mTransform->Right();
		mCurDir = mTransform->Right();
		mCurState = eMonsterState::MOVE;







	}



	void SkeletonArcherScript::Update()
	{
		// 이동->상태변환->애니메이션

		Transform* playerTr = mPlayer->GetComponent<Transform>();
		Vector3 playerPos = playerTr->GetPosition();

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
			mRigidbody->SetVelocity(Vector2(0.0f, 0.0f)); // 만약에 몬스터가 떨어지다가 멈추면 이거 코드 주석걸었다가 다시 원복시켜보기 


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
			mDir = -mTransform->Right(); // 왼쪽 값 부여 
		}

		mTransform->SetPosition(MonsterPos);


		// 방향대로 애니메이션을 재생한다. 
		if (mDir.x > 0)
		{
			mAnimator->PlayAnimation(L"Archer_RunR", true);
			mCollider->SetSize(Vector2(0.15f, 0.43f));
			mCollider->SetCenter(Vector2(-35.f, 0.2f));
		}

		else
		{
			mAnimator->PlayAnimation(L"Archer_RunL", true);
			mCollider->SetSize(Vector2(0.15f, 0.43f));
			mCollider->SetCenter(Vector2(-35.f, 0.2f));
		}

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
	}
	void SkeletonArcherScript::Hit()
	{
	}
	void SkeletonArcherScript::Guard()
	{
	}
	void SkeletonArcherScript::NearAttack()
	{
	}
	void SkeletonArcherScript::FarAttack()
	{
	}
	void SkeletonArcherScript::Dead()
	{
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