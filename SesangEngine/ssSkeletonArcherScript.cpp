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
#include "ssAudioClip.h"
#include "ssAudioListener.h"
#include "ssAudioSource.h"
#include "ssSoundMgrScript.h"

namespace ss
{

	// 몬스터 옮길 때 유의할 점 
	// 1. HIT용 콜라이더 크기를 하나로 고정한다. (좌우 포함)
	// 2. 애니메이션을 해당 콜라이더에 맞게 오프셋을 조절해서 맞춘다.
	// 3. 그렇게 안하면 프로그래스바 오프셋을 여러가지 ㅆ야함

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

		mTransform->SetScale(Vector3(73.f, 61.f, 1.f)); // backsize랑 스케일값 동일하게 주기. 


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
		mAnimator->Create(L"Archer_RunL", Image2, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 8, Vector2(73.f, 61.f), Vector2(10.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Archer_HitR", Image3, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 1, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_HitL", Image3, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 1, Vector2(73.f, 61.f), Vector2(10.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Archer_NearAttackR", Image4, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 12, Vector2(73.f, 61.f), Vector2::Zero, 0.06f);
		mAnimator->Create(L"Archer_NearAttackL", Image4, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 12, Vector2(73.f, 61.f), Vector2(10.f, 0.f), 0.06f, true);

		mAnimator->Create(L"Archer_FarAttackR", Image5, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 12, Vector2(73.f, 61.f), Vector2::Zero, 0.08f);
		mAnimator->Create(L"Archer_FarAttackL", Image5, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 12, Vector2(73.f, 61.f), Vector2(10.f, 0.f), 0.08f, true);

		mAnimator->Create(L"Archer_StunR", Image6, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 5, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_StunL", Image6, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 5, Vector2(73.f, 61.f), Vector2(10.f, 0.f), 0.1f, true);

		mAnimator->Create(L"Archer_DieR", Image7, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 17, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_DieL", Image7, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 17, Vector2(73.f, 61.f), Vector2(10.f, 0.f), 0.1f, true);

		// 일단 Idle 상태는 나중으로 만들기 
	
		// ======
			// 애니메이션 방향에 관한 기준	을 잡아준다.
		mDir = mTransform->Right();
		mCurDir = mTransform->Right();
		mCurState = eMonsterState::MOVE;

		// ===== 기본 충돌체 Hit 받는 용도 
		mCollider->SetName(L"Archer_HitCol");
		mCollider->SetType(eColliderType::Rect);

		mCollider->SetSize(Vector2(0.4f, 0.9f));
		mCollider->SetCenter(Vector2(-6.f, -0.f));
		// === idle 오른쪽 기준 충돌체 (나중에 수치 확인해서 이걸로 고치기)
		//mCollider->SetSize(Vector2(0.4f, 0.9f));
		//mCollider->SetCenter(Vector2(-6.f, -0.f));



		//==== 근접 공격 특정 인덱스 충돌체 
		//충돌체는 여기서 바로 넣지 말고 해당 인덱스 때 넣었다가 빼는 식으로 하기 
		mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"ArcherAttackCollider");
		mAttackColliderObj->Initialize();
		mAttackColliderObj->AddComponent<ArcherColScript>();

		mAttackColTr = mAttackColliderObj->GetComponent<Transform>();



		// =====
		// Near 공격 판정용 충돌체
		// 공격 판별 용도는 etc 레이어에 넣기 
		mNearRangeColObj = object::Instantiate<RangeCollider>(eLayerType::Etc, L"ArcherNearRangeCol");
		mNearRangeColObj->Initialize();

		mNearTr = mNearRangeColObj->GetComponent<Transform>();

		ArcherNearRangeScript* nearscript = mNearRangeColObj->AddComponent<ArcherNearRangeScript>();
		nearscript->SetOwner(mTransform->GetOwner()); // 스톤아이 오브젝트를 저장해둔다.


		mNearCol = mNearRangeColObj->GetComponent<Collider2D>();


		mNearCol->SetSize(Vector2(85.f, 20.f));
		mNearCol->SetCenter(Vector2(-1.f, 0.2f));



		// 원거리 공격 판정용 충돌체
		mFarRangeColObj = object::Instantiate<RangeCollider>(eLayerType::Etc, L"ArcherFarRangeCol");
		mFarRangeColObj->Initialize();

		mFarTr = mFarRangeColObj->GetComponent<Transform>();

		ArcherFarRangeScript* farScript = mFarRangeColObj->AddComponent<ArcherFarRangeScript>();
		farScript->SetOwner(mTransform->GetOwner()); // 스톤아이 오브젝트를 저장해둔다.


		mFarCol = mFarRangeColObj->GetComponent<Collider2D>();


		mFarCol->SetSize(Vector2(220.f, 20.f));
		mFarCol->SetCenter(Vector2(0.f, 0.2f));



		mAnimator->CompleteEvent(L"Archer_NearAttackR") = std::bind(&SkeletonArcherScript::NearAttackEnd, this);
		mAnimator->CompleteEvent(L"Archer_NearAttackL") = std::bind(&SkeletonArcherScript::NearAttackEnd, this);
		mAnimator->RegisterFrameEvent(L"Archer_NearAttackR", 9) = std::bind(&SkeletonArcherScript::Near_sfx, this);
		mAnimator->RegisterFrameEvent(L"Archer_NearAttackL", 9) = std::bind(&SkeletonArcherScript::Near_sfx, this);

		mAnimator->StartEvent(L"Archer_DieR") = std::bind(&SkeletonArcherScript::Dead_Start, this);
		mAnimator->StartEvent(L"Archer_DieL") = std::bind(&SkeletonArcherScript::Dead_Start, this);

		mAnimator->StartEvent(L"Archer_FarAttackR") = std::bind(&SkeletonArcherScript::Far_Ready_sfx, this);
		mAnimator->StartEvent(L"Archer_FarAttackL") = std::bind(&SkeletonArcherScript::Far_Ready_sfx, this);
		mAnimator->RegisterFrameEvent(L"Archer_FarAttackR", 8) = std::bind(&SkeletonArcherScript::Far_sfx, this);
		mAnimator->RegisterFrameEvent(L"Archer_FarAttackR", 8) = std::bind(&SkeletonArcherScript::Far_sfx, this);

		



	}



	void SkeletonArcherScript::Update()
	{
		// 이동->상태변환->애니메이션



		// 공격 중일 때는 위치 고정시키기 위해서 (몬스터가 플레이어의 위치가 변경되면 애니메이션이 리셋되는 현상 방지)
		if (!mbAttacking)
		{
			Transform* playerTr = mPlayer->GetComponent<Transform>();
			Vector3 playerPos = playerTr->GetPosition();

			// 방향의 기준을 잡아준다. (몬스터의 위치 값보다 X값이 크면 오른쪽이므로  1, 왼쪽에 있으면 -1)
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

		if (mCurState != eMonsterState::DEAD)
		{
			mPrevDir = mCurDir;
		}



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

		// 방향대로 애니메이션을 재생한다. 
		if (mDir.x > 0)
		{
			mAnimator->PlayAnimation(L"Archer_RunR", true);
		}

		else if (mDir.x < 0)
		{
			mAnimator->PlayAnimation(L"Archer_RunL", true);
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
			mDir = -mTransform->Right(); // 왼쪽 값 부여 
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
		// 방향대로 애니메이션을 재생한다. 
		{
			if (mCurDir.x > 0)
				mAnimator->PlayAnimation(L"Archer_StunR", false);

			else
				mAnimator->PlayAnimation(L"Archer_StunL", false);
		}

		// 애니메이션 재생이 끝나면 
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

			// 둘 다 공격 중인 상태가 아니라면 플레이어가 판정 범위 밖에 있었다는 것이므로
			else if (!mbNearAttack && !mbFarAttack)
			{
				mCurState = eMonsterState::MOVE;
			}
		}


	}
	void SkeletonArcherScript::Hit()
	{
		// 방향대로 애니메이션을 재생한다. 
		Vector3 monsterPos = mTransform->GetPosition();

		if (mCurDir.x > 0)
		{
			mRigidbody->AddForce(Vector2(-0.07f, 0.f));
			mTransform->SetPosition(Vector3(monsterPos.x - 0.005f, monsterPos.y, monsterPos.z));
			mAnimator->PlayAnimation(L"Archer_HitR", false);
		}

		else
		{
			mRigidbody->AddForce(Vector2(0.07f, 0.f));
			mTransform->SetPosition(Vector3(monsterPos.x + 0.005f, monsterPos.y, monsterPos.z));
			mAnimator->PlayAnimation(L"Archer_HitL", false);
		}


		// 애니메이션 재생이 끝나면 
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

					mAttackCol->SetSize(Vector2(30.f, 30.f));
					mAttackCol->SetCenter(Vector2(20.f, -5.f));

					// 대쉬 중엔 아예 충돌 안되게 해버림 
					if (playerScript->IsDash())
					{
						mAttackColliderObj->RemoveComponent<Collider2D>();
					}
				}
				
				else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 10)
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

					// 대쉬 중엔 아예 충돌 안되게 해버림 
					if (playerScript->IsDash())
					{
						mAttackColliderObj->RemoveComponent<Collider2D>();
					}


				}


				else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 10)
				{
					mAttackColliderObj->RemoveComponent<Collider2D>();
				}


				
				mAnimator->PlayAnimation(L"Archer_NearAttackL", true);

			}
		}


	}
	void SkeletonArcherScript::NearAttackEnd()
	{

		mbNearAttack = false;

		ChangeState(eMonsterState::MOVE);


	}
	void SkeletonArcherScript::FarAttack()
	{
		Vector3 pos = mTransform->GetPosition();
		pos.z -= 0.01;

		{
			if (mAnimator->GetCurActiveAnimation()->GetIndex() == 8)
			{

				m_fTime += (float)Time::DeltaTime();

				// coolDown 초마다 발사 
				if (m_fTime >= m_tMonsterInfo.m_fCoolDown)
				{

					if (mCurDir.x == 1.0f)
					{
						// 발사체 위치 조절 
						pos += Vector3(35.f, -2.5f, 0.f);
						mArrowObj = object::Instantiate<ArcherArrow>(pos, eLayerType::Mon_Bullet, L"ArcherArrowObj_R");
						mArrowObj->GetComponent<ArcherArrowScript>()->SetOriginOwner(mTransform->GetOwner());

					}

					else if (mCurDir.x == -1.0f)
					{
						// 발사체 위치 조절 
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

			}

			else
			{
				mAnimator->PlayAnimation(L"Archer_FarAttackL", true);
			}

		}
	}
	void SkeletonArcherScript::Dead()
	{
		if (mPrevDir.x > 0)
		{
			mAnimator->PlayAnimation(L"Archer_DieR", false);

		}

		else
		{
			mAnimator->PlayAnimation(L"Archer_DieL", false);
		}

		// 애니메이션 재생이 끝나면 
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			mNearRangeColObj->SetState(GameObject::eState::Dead);
			mAttackColliderObj->SetState(GameObject::eState::Dead);
			mFarRangeColObj->SetState(GameObject::eState::Dead);

			GetOwner()->SetState(GameObject::eState::Dead);


			if (mPrevState == eMonsterState::FARATTACK)
			{
				mArrowObj->SetState(GameObject::eState::Dead);
			}
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
	void SkeletonArcherScript::Dead_Start()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
		pSFX->SetClip(Resources::Find<AudioClip>(L"Enemy_Die_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.3f);
	}
	void SkeletonArcherScript::Hit_Start()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
		pSFX->SetClip(Resources::Find<AudioClip>(L"Enemy_Hit_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.3f);

	}
	void SkeletonArcherScript::Near_sfx()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();


		pSFX->SetClip(Resources::Find<AudioClip>(L"Archer_NearAttack_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.3f);


	}
	void SkeletonArcherScript::Far_Ready_sfx()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();


		pSFX->SetClip(Resources::Find<AudioClip>(L"Archer_FarAttackReady_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.3f);

	}
	void SkeletonArcherScript::Far_sfx()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
		pSFX->SetClip(Resources::Find<AudioClip>(L"Archer_FarAttackFire_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.3f);

	}

}