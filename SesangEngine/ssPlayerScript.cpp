#include "ssPlayerScript.h"
#include "ssCameraScript.h"
#include "ssTransform.h"
#include "ssGameObject.h"
#include "ssTime.h"
#include "ssInput.h"
#include "ssAnimator.h"
#include "ssRigidbody2D.h"
#include "ssPlatform.h"
#include "ssPlayer.h"
#include "ssSceneManager.h"
#include "ssCharacterState.h"
#include "ssGameState.h"
#include "ssAttackCollider.h"
#include "ssObject.h"
#include "ssPlayerAttackColScript.h"
#include "ssPlayerGuardColScript.h"
#include "ssMeshRenderer.h"
#include "ssSceneManager.h"
#include "ssMonster.h"
#include "ssBigWolfScript.h"
#include "ssRenderer.h"
#include "ssConstantBuffer.h"
#include "ssEffect.h"
#include "ssEffectScript.h"


namespace ss
{
	PlayerScript::PlayerScript()
		: mSpeed(200.0f)
		, mCurState(ePlayerState::IDLE)
		, mVelocity(1.f, 1.f)
		, mJumpHeight(65.0f)
		, mJumpCount(0)
		, mbJump(false)
		, mbIdleJump(false)
		, mChangeFirst(false)
		, mWeaponType{}
		, mbDash(false)
		, DashDuration(0.0f)
		, mAttackCount(0)
		, mWeaponCount(0)
		, mbPrev(false)
		, mPrevColSize(Vector2(0.2f, 0.8f))
		, mPrevColCeter(Vector2(-3.5f, 2.f))
		, mTime(0.f)
		, mbChange(false)
		, mHit(false)
		, mbHitting(false)
		, mHitEffect(nullptr)
	{
	}
	PlayerScript::~PlayerScript()
	{
	}

	void PlayerScript::Initialize()
	{
	
		mPlayer = (Player*)GetOwner();

	

		mState = GetOwner()->GetComponent<CharacterState>();
		mAnimator = GetOwner()->GetComponent<Animator>();
		mTransform = GetOwner()->GetComponent<Transform>();
		mRigidbody = GetOwner()->GetComponent<Rigidbody2D>();
		mCollider = GetOwner()->GetComponent<Collider2D>();
	

		mAnimator->PlayAnimation(L"Player_S_IdleR", true);
		//mAnimator->PlayAnimation(L"Player_D_IdleR", true);

			


		

		// 애니메이션 방향에 관한 기준	을 잡아준다.
		mCurDir = mTransform->Right();


		mAnimator->EndEvent(L"Player_S_Attack1R") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_S_Attack1L") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_S_Attack2R") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_S_Attack2L") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_S_Attack3R") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_S_Attack3L") = std::bind(&PlayerScript::AttackEnd, this);

		// 가드할 때 
		mAnimator->EndEvent(L"Player_S_GuardR") = std::bind(&PlayerScript::GuardEnd, this);
		mAnimator->EndEvent(L"Player_S_GuardL") = std::bind(&PlayerScript::GuardEnd, this);

		// ==== 플레이어 클래스에 화살버전 guard는 안 만들었음. 추가해놓고 이거 주석 풀기 
		//mAnimator->EndEvent(L"Player_B_GuardR") = std::bind(&PlayerScript::GuardEnd, this);
		//mAnimator->EndEvent(L"Player_B_GuardL") = std::bind(&PlayerScript::GuardEnd, this);

		// 플레이어 스크립트의 complete를 호출하면 Idle로 전환한다.
		//mAnimator->CompleteEvent(L"Player_D_Idle") = std::bind(&PlayerScript::Complete, this);


		// 공격용 충돌체 
		mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"PlayerAttackCollider");
		mAttackColliderObj->Initialize();
		mAttackColliderObj->AddComponent<PlayerAttackColScript>();
			
		mAttackColTr = mAttackColliderObj->GetComponent<Transform>();
	

	}
	void PlayerScript::Update()
	{

		Vector3 pos = mTransform->GetPosition();
	
			mWeaponType = SceneManager::GetWeaponInfo();
			mbChange = false;
		
		//	BindConstantBuffer();

		switch (mCurState)
		{
		case ss::ePlayerState::IDLE:
			Idle();
			break;

		case ss::ePlayerState::RUN:
			Run();
			break;

		case ss::ePlayerState::JUMP:
			Jump();
			break;

		case ss::ePlayerState::FALL:
			Fall();
			break;

		case ss::ePlayerState::DASH:
			Dash();
			break;

		case ss::ePlayerState::ATTACK:
			Attack();
			break;

		case ss::ePlayerState::SPATTACK:
			SPAttack();
			break;


		case ss::ePlayerState::OVERLOADING:
			Overloading();
			break;

		case ss::ePlayerState::HIT:
			Hit();
			break;

		case ss::ePlayerState::GUARD:
			Guard();
			break;
		}

		Animation();

		mPrevState = mCurState;
		mPrevDir = mCurDir;


		// 이동->상태변환->애니메이션


		Script::Update();




	}

	void PlayerScript::LateUpdate()
	{
		mAttackColTr->SetPosition(mTransform->GetPosition());

	}

	void PlayerScript::OnCollisionEnter(Collider2D* other)
	{
		if (L"col_Floor" == other->GetOwner()->GetName())
		{

			mJumpCount = 0;
			mFallTime = 0.0f;
			mRigidbody->SetGround(true);
			mRigidbody->SetVelocity(Vector2(0.0f, 0.0f));


			ChangeState(ePlayerState::IDLE);

		}

		if (other->GetOwner()->GetLayerType() == eLayerType::Ground)
		{
			
		}
	


		// ======
		// 발판 y값 체크한 타일 
		else if (L"col_SpecialFloor" == other->GetOwner()->GetName())
		{


			// 플레이어가 발판 위에 있을 때만 물리 작동되게 함 (발판 아래에서 점프했을 때 물리작동되는거 방지) 
			if (GetOwner()->GetComponent<Collider2D>()->GetPosition().y > other->GetPosition().y)
				Enter_TreeFloor();
		

			

			// 플레이어의 위치를 갖고 온다.
			// 해당 충돌체의 위치를 갖고 온다. (충돌체이기 전에 게임오브젝트) 


		}

	
		else if (L"L_col_Door" == other->GetOwner()->GetName())
		{
			SceneManager::LoadScene(L"Stage1Scene");
		}

		else if (L"col_startBoss1StageObj" == other->GetOwner()->GetName())
		{
			// storm 시작으로 상태 바꾸기 
			mMonster->GetComponent<BigWolfScript>()->ChangeState(eWolfBossState::STOM_START);
	
		}


	}

	void PlayerScript::OnCollisionStay(Collider2D* other)
	{


		// ==== 일단 이미지는 화살이지만, 검 애니메이션 나오게 해보기 
		if (L"col_TreeBorrow" == other->GetOwner()->GetName())
		{

			if (Input::GetKeyDown(eKeyCode::F))
			{

				// ui에 칼 이미지가 나오게 한다.

				// f키를 누르면 칼 idle 애니메이션으로 바뀌게 한다.
				mChangeFirst = true;
				mWeaponType = eWeaponType::SWORD;
				SceneManager::SetWeaponInfo(mWeaponType);
				mbChange = true;
				

			}

		}	



	}

	void PlayerScript::OnCollisionExit(Collider2D* other)
	{
		if (L"col_Floor" == other->GetOwner()->GetName())
		{
			mRigidbody->SetGround(false);
		

			if (mCurState != ePlayerState::JUMP)
			{
				if (mCurDir.x > 0)
				{
					mRigidbody->SetVelocity(Vector2(50.f, 0.f));
				}

				else if (mCurDir.x < 0)
				{
					mRigidbody->SetVelocity(Vector2(-50.f, 0.f));
				}

				ChangeState(ePlayerState::FALL);
			}

		}



		else if (L"col_SpecialFloor" == other->GetOwner()->GetName())
		{


			//// 플레이어가 발판 위에 있을 때만 물리 작동되게 함 (발판 아래에서 점프했을 때 물리작동되는거 방지) 

				mRigidbody->SetGround(false);
			
				if (mCurState != ePlayerState::JUMP)
				{
					if (mCurDir.x > 0)
					{
						mRigidbody->SetVelocity(Vector2(50.f, 0.f));
					}

					else if (mCurDir.x < 0)
					{
						mRigidbody->SetVelocity(Vector2(-50.f, 0.f));
					}
			
					ChangeState(ePlayerState::FALL);
				}
			
		}


		// 플레이어의 위치를 갖고 온다.
		// 해당 충돌체의 위치를 갖고 온다. (충돌체이기 전에 게임오브젝트) 
		else if (L"col_startBoss1StageObj" == other->GetOwner()->GetName())
		{
			mBossStartColObj->SetState(GameObject::eState::Dead);

		}

	}

	void PlayerScript::BindConstantBuffer()
	{

		// 상수 버퍼에 월드, 뷰, 투영 행렬을 담아 보내준다. 
		renderer::HitCB hitCB = {};
		hitCB.h_IsHit = mHit;

		// 상수 버퍼 중에 Transform 상수 버퍼를 갖고 온다. 
		ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Hit];
		cb->SetData(&hitCB); // 월드 행렬 정보를 상수 버퍼에 넣어준다.
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS); // 상수 버퍼는 어느 쉐이더 단계이든 바인딩할 수 있다는게 장점이다. 

	}



	void PlayerScript::Idle()
	{


		if (mAttackCount == 3)
		{
			mAttackCount = 0;
		}

		// 제자리에서 점프  (맨 앞에 둬야됨) 
		if (Input::GetKeyDown(eKeyCode::C))
		{
			mRigidbody->SetGravity(Vector2(0.f, 1500.f));

			mVelocity = mRigidbody->GetVelocity();
			mVelocity.y = -500.f;
			mRigidbody->SetVelocity(mVelocity);
			mRigidbody->SetGround(false);
			++mJumpCount;
			mbIdleJump = true;
			mbJump = true;

			ChangeState(ePlayerState::JUMP); // 이거 지우면 점프 모션으로 이미지 안바뀜

		}


		// 이동
		else if (Input::GetKey(eKeyCode::RIGHT))
		{
			// Idle 기준점 정해놓기 꼭 
			mCurDir = mTransform->Right();
			ChangeState(ePlayerState::RUN);
		}


		// === 왼쪽
		else if (Input::GetKey(eKeyCode::LEFT))
		{
			mCurDir = -mTransform->Right();
			ChangeState(ePlayerState::RUN);
		}


		else if (Input::GetKeyDown(eKeyCode::Z))
		{

			++mAttackCount;

		

			ChangeState(ePlayerState::ATTACK);
		}

		else if (Input::GetKeyDown(eKeyCode::Z) && mAttackCount == 1)
		{
			++mAttackCount;
			ChangeState(ePlayerState::ATTACK);

		}

		else if (Input::GetKeyDown(eKeyCode::Z) && mAttackCount == 2)
		{
			++mAttackCount;
			ChangeState(ePlayerState::ATTACK);
		}


		// 대쉬
		else if (Input::GetKeyDown(eKeyCode::X))
		{
			// 대쉬 시작
			mbDash = true;
			DashDuration = 0.5f;  // 대쉬 지속 시간 설정

			// Run일 때는 속력을 주고 있지만, Idle때는 아무런 키도 누를 일이 없어서 속도가 0이라 임의로 부여해줌 

			if(mPrevDir.x > 0)
			mRigidbody->SetVelocity(Vector2(300.f, 0.f));

			else if (mPrevDir.x < 0)
				mRigidbody->SetVelocity(Vector2(-300.f, 0.f));

			mVelocity.x *= 100 * Time::DeltaTime();  // 속도 증가

			ChangeState(ePlayerState::DASH);
		}

		// Guard
		else if (Input::GetKeyDown(eKeyCode::LCTRL))
		{
			Vector3 pos = mTransform->GetPosition();

			// Guard용 충돌체
			mGuardColObj = object::Instantiate<AttackCollider>(pos,eLayerType::Guard, L"PlayerGuardObj");
			mGuardColObj->Initialize();
			PlayerGuardColScript* guardScript = mGuardColObj->AddComponent<PlayerGuardColScript>();
			mGuardCol = mGuardColObj->AddComponent<Collider2D>();
			mGuardCol->SetName(L"PlayerGuardCol");

			if (mCurDir.x > 0)
			{
				mGuardCol->SetSize(Vector2(2.f, 15.f));
				mGuardCol->SetCenter(Vector2(13.f, 3.f));
			}

			else
			{
				mGuardCol->SetSize(Vector2(2.f, 15.f));
				mGuardCol->SetCenter(Vector2(-13.5f, 3.f));
			}


			guardScript->StorePlayerScript(this);
	

			ChangeState(ePlayerState::GUARD);
		}


		// 과부하 
		else if (Input::GetKeyDown(eKeyCode::G))
		{
			if (mWeaponType == eWeaponType::SWORD)
			{
				ChangeState(ePlayerState::OVERLOAD_START);
			}
			
		}




	}


	// 움직이는 부분에 물리 적용하지말기 (내 게임 실제 동작해보니 물리 적용하면 안됨)
	void PlayerScript::Run()
	{
		//====================== X는 대쉬 
		if (Input::GetKeyUp(eKeyCode(eKeyCode::RIGHT)) || Input::GetKeyUp(eKeyCode(eKeyCode::LEFT))
			|| Input::GetKeyUp(eKeyCode(eKeyCode::Z)) || Input::GetKeyUp(eKeyCode(eKeyCode::X)) || Input::GetKeyUp(eKeyCode(eKeyCode::C)))
		{
			ChangeState(ePlayerState::IDLE);
		}

		// === 오른쪽
		if (Input::GetKey(eKeyCode::RIGHT))
		{
			mRigidbody->SetVelocity(Vector2(300.f, 0.f));


		}

		else if (Input::GetKeyUp(eKeyCode::RIGHT))
		{
			mRigidbody->SetVelocity(Vector2(0.f, 0.f));
		}


		//=============================
				// 왼쪽
		if (Input::GetKey(eKeyCode::LEFT))
		{
			mRigidbody->SetVelocity(Vector2(-300.f, 0.f));


		}

		else if (Input::GetKeyUp(eKeyCode::LEFT))
		{
			mRigidbody->SetVelocity(Vector2(0.f, 0.f));		
		}


		// ========대쉬 
		if (Input::GetKeyDown(eKeyCode::X))
		{
			// 대쉬 시작
			mbDash = true;
			DashDuration = 0.5f;  // 대쉬 지속 시간 설정
			mVelocity.x *= 100 * Time::DeltaTime();  // 속도는 나중에 맞추기 

			ChangeState(ePlayerState::DASH);
		}

		// 점프
		if (Input::GetKeyDown(eKeyCode::C))
		{
			mRigidbody->SetGravity(Vector2(0.f, 1500.f));

				mVelocity = mRigidbody->GetVelocity();

				// 애니메이션 땜에 아래처럼 조건문을 달음 
				if (mPrevDir == mTransform->Right())
					mVelocity.x = 500.f;
				else
					mVelocity.x = -500.f;

				mJumpPower = sqrt(2 * abs(mRigidbody->GetGravity().y) * 80.f); // * 점프 높이

			
					mVelocity.y = -mJumpPower;

				mRigidbody->SetVelocity(mVelocity);

				mRigidbody->SetGround(false);
				mFallTime += Time::DeltaTime();
				++mJumpCount;

			
				ChangeState(ePlayerState::JUMP); // 이거 지우면 점프 모션으로 이미지 안바뀜
			


		}


		// Guard
		else if (Input::GetKeyDown(eKeyCode::LCTRL))
		{
			// move상태여도 guard 즉시 제자리에 있도록 고정시킴 
			mRigidbody->SetVelocity(Vector2(0.f, 0.f));


			Vector3 pos = mTransform->GetPosition();

			// Guard용 충돌체
			mGuardColObj = object::Instantiate<AttackCollider>(pos, eLayerType::Guard, L"PlayerGuardObj");
			mGuardColObj->Initialize();
			PlayerGuardColScript* guardScript = mGuardColObj->AddComponent<PlayerGuardColScript>();
			mGuardCol = mGuardColObj->AddComponent<Collider2D>();
			mGuardCol->SetName(L"PlayerGuardCol");

			if (mCurDir.x > 0)
			{
				mGuardCol->SetSize(Vector2(2.f, 15.f));
				mGuardCol->SetCenter(Vector2(8.f, 3.f));
			}

			else
			{
				mGuardCol->SetSize(Vector2(2.f, 15.f));
				mGuardCol->SetCenter(Vector2(-8.f, 3.f));
			}


			guardScript->StorePlayerScript(this);


			ChangeState(ePlayerState::GUARD);
		}

	}
	void PlayerScript::Jump()
	{
		Vector3 pos = mTransform->GetPosition();

		// Idle 상태로 뛰었을 떄 2번 이상 못 뛰게 막음 
		if (Input::GetKeyDown(eKeyCode::Z) && mJumpCount > 2)
			return;



		if (Input::GetKeyDown(eKeyCode::C) && mJumpCount == 1)
		{
		
			mVelocity = mRigidbody->GetVelocity();

			// Idle에서의 점프는 앞으로 가는게 필요없으므로 0으로 만들어줌
			if (mbIdleJump)
			{
				mVelocity.x = 0.0f;
				mbIdleJump = false;
				mJumpPower = sqrt(2 * abs(mRigidbody->GetGravity().y) * 70);

			}


			else if (mCurDir == mTransform->Right())
				mVelocity.x = 250.0f;

			else if (mCurDir == -mTransform->Right())
				mVelocity.x = -250.0f;

			mJumpPower = sqrt(2 * abs(mRigidbody->GetGravity().y) * mJumpHeight);


			mVelocity.y = -mJumpPower;

			mRigidbody->SetVelocity(mVelocity);

			mRigidbody->SetGround(false);
			++mJumpCount;
				
		}


		// 점프 중에 방향키 누르면 해당 방향으로 변경되면서 떨어지도록 함 
		if (Input::GetKey(eKeyCode::RIGHT))
		{
			mCurDir = mTransform->Right();
			mVelocity.x = 180.0f;



		}

		else if (Input::GetKey(eKeyCode::LEFT))
		{
			mCurDir = -mTransform->Right();
			mVelocity.x = -180.0f;


		}
	
		// 처음에 점프를 시도했을 때의 y값은 변경되면 안되므로 아래처럼 해줌
		mVelocity.y = mRigidbody->GetVelocity().y;
		mRigidbody->SetVelocity(mVelocity);



	}

	void PlayerScript::Fall()
	{

		// 나무 발판에서 떨어질 떄 떨어지는 모션 (falling) 모션 넣기 
		// 어색하게 떨어지는 것도 수정해야함 (나무 발판의 양 옆 충돌체의 어느 크기를 벗어나면 떨어지게 한다거나 그런 거 하면 될듯 )

	}


	void PlayerScript::Dash()
	{


		// 대쉬 시간이 0.2초가 넘어가면 대쉬가 끝난 것으로 간주

		if (mbDash)
		{
			DashDuration -= Time::DeltaTime();

			if (DashDuration <= 0.f)
			{
				mbDash = false;
				mVelocity.x /= 2;
				mRigidbody->SetVelocity(mVelocity);
				

				ChangeState(ePlayerState::IDLE);
			}

		}


	}

	void PlayerScript::Attack()
	{
		CameraScript* camera = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();

		camera->StartShake(0.01f, 0.05f);

		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(ePlayerState::IDLE);
		}

	}

	void PlayerScript::SPAttack()
	{

	}

	void PlayerScript::Overload_Start()
	{

	}

	void PlayerScript::Overloading()
	{

	}

	void PlayerScript::Overload_End()
	{

	}

	void PlayerScript::Hit()
	{
		
		// hit 상태 즉시 제자리에 멈춰있도록 속도 0으로 만듦
		mRigidbody->SetVelocity(Vector2(0.f, 0.f));

		if (!mbHitting)
		{
			mHitEffect = object::Instantiate<Effect>(eLayerType::Effect, L"PlayerHitUI");

			EffectScript* effectscript = mHitEffect->AddComponent<EffectScript>();
			effectscript->SetOriginOwner((Player*)mTransform->GetOwner());
			mbHitting = true;
		}

		// 애니메이션 재생이 끝나면 
		if (mbHitting && mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(ePlayerState::IDLE);
			mbHitting = false;
		}

	}

	void PlayerScript::Guard()
	{

		// 애니메이션 재생이 끝나면 
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(ePlayerState::IDLE);
		}

	}




	void PlayerScript::Animation()
	{
		// 애니메이션 업데이트가 계속 돌지 않도록 방어처리
		// 이전 상태와 현재 상태가 다를 때만 애니메이션 재생이 되도록 한다
		// (Update에 적었기 때문에 이전 상태로 비교해주지 않으면 1초마다 계속 play되므로 멈춰있는것처럼 보임)



		if (mCurState != mPrevState || mCurDir != mPrevDir ||
			mChangeFirst && mCurState == ePlayerState::IDLE)

		{
		}

			switch (mCurState)
			{
			case ss::ePlayerState::IDLE:

				if (mWeaponType == eWeaponType::NONE)
				{

					if (mCurDir.x > 0)
					{
						mAnimator->PlayAnimation(L"Player_D_IdleR", true);
						mCollider->SetCenter(Vector2(-6.f, 2.f));
					}

					else
					{
						mAnimator->PlayAnimation(L"Player_D_IdleL", true);
						mCollider->SetCenter(Vector2(6.f, 2.f));
					}

				}

				else if (mWeaponType == eWeaponType::SWORD)
				{
					if (mChangeFirst)
					{
						mChangeFirst = false;

						if (mPrevDir.x > 0)
						{
							mAnimator->PlayAnimation(L"Player_S_IdleR", true);
							mCollider->SetCenter(Vector2(-6.f, 2.f));
						}

						else
						{
							mAnimator->PlayAnimation(L"Player_S_IdleL", true);
							mCollider->SetCenter(Vector2(6.5f, 2.f));
						}

					}


					if (mCurDir.x > 0)
					{
						mAnimator->PlayAnimation(L"Player_S_IdleR", true);
						mCollider->SetCenter(Vector2(-6.f, 2.f));
					}

					else
					{
						mAnimator->PlayAnimation(L"Player_S_IdleL", true);
						mCollider->SetCenter(Vector2(6.5f, 2.f));
					}
				}

				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					if (mChangeFirst)
					{
						mChangeFirst = false;

						if (mPrevDir.x > 0)
							mAnimator->PlayAnimation(L"Player_B_IdleR", true);

						else
							mAnimator->PlayAnimation(L"Player_B_IdleL", true);

					}


					if (mCurDir.x > 0)
						mAnimator->PlayAnimation(L"Player_B_RunR", true);

					else
						mAnimator->PlayAnimation(L"Player_B_IdleL", true);
				}

				else if (mWeaponType == eWeaponType::PISTOL)
				{
				}



				break;

			case ss::ePlayerState::RUN:

				if (mWeaponType == eWeaponType::NONE)
				{
					if (mCurDir.x > 0)
					{
						mAnimator->PlayAnimation(L"Player_D_RunR", true);
						mCollider->SetCenter(Vector2(-6.f, 2.f));
					}

					else
					{
						mAnimator->PlayAnimation(L"Player_D_RunL", true);
						mCollider->SetCenter(Vector2(6.f, 2.f));
					}
				}

				else if (mWeaponType == eWeaponType::SWORD)
				{
					if (mCurDir.x > 0)
						mAnimator->PlayAnimation(L"Player_S_RunR", true);
					else
						mAnimator->PlayAnimation(L"Player_S_RunL", true);
				}


				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					if (mCurDir.x > 0)
						mAnimator->PlayAnimation(L"Player_B_RunR", true);
					else
						mAnimator->PlayAnimation(L"Player_B_RunL", true);
				}

				else if (mWeaponType == eWeaponType::PISTOL)
				{
					if (mCurDir.x > 0)
						mAnimator->PlayAnimation(L"Player_B_RunR", true);
					else
						mAnimator->PlayAnimation(L"Player_B_RunL", true);
				}
				break;


			case ss::ePlayerState::HANG:
				if (mWeaponType == eWeaponType::SWORD)
				{
					if (mCurDir.x > 0)
						mAnimator->PlayAnimation(L"Player_S_HangR", true);
					else
						mAnimator->PlayAnimation(L"Player_S_HangL", true);
				}

				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					if (mCurDir.x > 0)
						mAnimator->PlayAnimation(L"Player_B_HangL", true);
					else
						mAnimator->PlayAnimation(L"Player_B_HangL", true);
				}

				else if (mWeaponType == eWeaponType::PISTOL)
				{
					if (mCurDir.x > 0)
						mAnimator->PlayAnimation(L"Player_B_HangL", true);
					else
						mAnimator->PlayAnimation(L"Player_B_HangL", true);
				}

				break;


			case ss::ePlayerState::JUMP:
				if (mWeaponType == eWeaponType::NONE)
				{
					if (mCurDir.x > 0)
					{
						mAnimator->PlayAnimation(L"Player_D_JumpR", false);
						mCollider->SetCenter(Vector2(-7.f, 2.f));
					}


					else
					{
						mAnimator->PlayAnimation(L"Player_D_JumpL", false);
						mCollider->SetCenter(Vector2(7.f, 2.f));
					}
				}

				else if (mWeaponType == eWeaponType::SWORD)
				{


					if (mCurDir.x > 0)
					{
						mAnimator->PlayAnimation(L"Player_S_JumpR", false);
						mCollider->SetCenter(Vector2(-7.f, 2.f));
					}

					else
					{
						mAnimator->PlayAnimation(L"Player_S_JumpL", false);
						mCollider->SetCenter(Vector2(7.f, 2.f));
					}
				}

				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					if (mCurDir.x > 0)
					{
						mAnimator->PlayAnimation(L"Player_B_JumpR", false);
					}

					else
						mAnimator->PlayAnimation(L"Player_B_JumpL", false);
				}

				else if (mWeaponType == eWeaponType::PISTOL)
				{
				}




				break;

			case ss::ePlayerState::FALL:
				if (mWeaponType == eWeaponType::NONE)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_D_FallR", true);
					else
						mAnimator->PlayAnimation(L"Player_D_FallL", true);
				}

				else if (mWeaponType == eWeaponType::SWORD)
				{
					if (mPrevDir.x > 0)
					{
						mAnimator->PlayAnimation(L"Player_S_FallR", true);
						mCollider->SetCenter(Vector2(-6.f, 2.f));
					}
					else
					{
						mAnimator->PlayAnimation(L"Player_S_FallL", true);
						mCollider->SetCenter(Vector2(6.f, 2.f));
					}
				}


				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_B_FallR", true);
					else
						mAnimator->PlayAnimation(L"Player_B_FallL", true);
				}


				else if (mWeaponType == eWeaponType::PISTOL)
				{
				}


				break;


			case ss::ePlayerState::LADING:
				if (mWeaponType == eWeaponType::SWORD)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_S_LandingR", true);
					else
						mAnimator->PlayAnimation(L"Player_S_Landingl", true);
				}

				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_B_LandingR", true);
					else
						mAnimator->PlayAnimation(L"Player_B_Landingl", true);
				}

				else if (mWeaponType == eWeaponType::PISTOL)
				{
				}


				break;

			case ss::ePlayerState::DASH:
				if (mWeaponType == eWeaponType::SWORD)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_S_DashR", true);
					else
						mAnimator->PlayAnimation(L"Player_S_DashL", true);
				}

				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_B_DashR", true);
					else
						mAnimator->PlayAnimation(L"Player_B_DashL", true);
				}


				else if (mWeaponType == eWeaponType::PISTOL)
				{
				}


				break;

			case ss::ePlayerState::STUN:
				if (mWeaponType == eWeaponType::SWORD)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_S_StunR", false);
					else
						mAnimator->PlayAnimation(L"Player_S_StunL", false);
				}

				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_B_StunR", false);
					else
						mAnimator->PlayAnimation(L"Player_B_StunL", false);
				}


				else if (mWeaponType == eWeaponType::PISTOL)
				{
				}

				break;

			case ss::ePlayerState::HIT:
			{
				if (mWeaponType == eWeaponType::SWORD)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_S_HitR", false);

					else
						mAnimator->PlayAnimation(L"Player_S_HitL", false);

		
				}

				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_B_HitR", false);
					else
						mAnimator->PlayAnimation(L"Player_B_HitL", false);
				}

				else if (mWeaponType == eWeaponType::PISTOL)
				{
				}

			}
			break;

			case ss::ePlayerState::GUARD:
			{
				if (mWeaponType == eWeaponType::SWORD)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_S_GuardR", false);
					else
						mAnimator->PlayAnimation(L"Player_S_GuardL", false);
				}

				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_B_GuardR", false);
					else
						mAnimator->PlayAnimation(L"Player_B_GuardL", false);
				}

				else if (mWeaponType == eWeaponType::PISTOL)
				{
				}



			}
			break;


			case ss::ePlayerState::ATTACK:
				if (mWeaponType == eWeaponType::SWORD)
				{
					if (mAnimator->GetCurActiveAnimation()->GetIndex() == 2)
					{
						if (mPrevDir.x > 0)
						{

							mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
							mAttackCol->SetSize(Vector2(30.f, 40.f));
							mAttackCol->SetCenter(Vector2(14.f, 2.f));
						}

						else if (mPrevDir.x < 0)
						{
							mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
							mAttackCol->SetSize(Vector2(30.f, 40.f));
							mAttackCol->SetCenter(Vector2(-14.f, 2.f));
						}
					

					}

			

					if (mAttackCount == 1)
					{

						if (mPrevDir.x > 0)
						{


					
							mAnimator->PlayAnimation(L"Player_S_Attack1R", false);
						}

						else
							mAnimator->PlayAnimation(L"Player_S_Attack1L", false);
					}

					else if (mAttackCount == 2)
					{

						if (mPrevDir.x > 0)
							mAnimator->PlayAnimation(L"Player_S_Attack2R", false);
						else
							mAnimator->PlayAnimation(L"Player_S_Attack2L", false);
					}

					else if (mAttackCount == 3)
					{

						if (mPrevDir.x > 0)
							mAnimator->PlayAnimation(L"Player_S_Attack3R", false);
						else
							mAnimator->PlayAnimation(L"Player_S_Attack3L", false);
					}
				}


				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					if (mAttackCount == 1)
					{

						if (mPrevDir.x > 0)
							mAnimator->PlayAnimation(L"Player_B_Attack1R", false);
						else
							mAnimator->PlayAnimation(L"Player_B_Attack1L", false);
					}

					else if (mAttackCount == 2)
					{

						if (mPrevDir.x > 0)
							mAnimator->PlayAnimation(L"Player_B_Attack2R", false);
						else
							mAnimator->PlayAnimation(L"Player_B_Attack2L", false);
					}

					else if (mAttackCount == 3)
					{

						if (mPrevDir.x > 0)
							mAnimator->PlayAnimation(L"Player_B_Attack3R", false);
						else
							mAnimator->PlayAnimation(L"Player_B_Attack3L", false);
					}
				}

				else if (mWeaponType == eWeaponType::PISTOL)
				{
				}

				break;

			case ss::ePlayerState::SPATTACK:
				if (mWeaponType == eWeaponType::SWORD)
				{

				}

				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
				}


				else if (mWeaponType == eWeaponType::PISTOL)
				{
				}
				break;



			case ss::ePlayerState::OVERLOAD_START:
				if (mWeaponType == eWeaponType::SWORD)
				{

				}

				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
				}


				else if (mWeaponType == eWeaponType::PISTOL)
				{
				}
				break;

			case ss::ePlayerState::OVERLOADING:
				if (mWeaponType == eWeaponType::SWORD)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_S_DieR", true);
					else
						mAnimator->PlayAnimation(L"Player_S_DieL", true);
				}

				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
				}


				else if (mWeaponType == eWeaponType::PISTOL)
				{
				}
				break;


			case ss::ePlayerState::OVERLOAD_END:
				if (mWeaponType == eWeaponType::SWORD)
				{

				}

				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
				}


				else if (mWeaponType == eWeaponType::PISTOL)
				{
				}
				break;



			case ss::ePlayerState::DEAD:
				if (mWeaponType == eWeaponType::SWORD)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_S_DieR", true);
					else
						mAnimator->PlayAnimation(L"Player_S_DieL", true);
				}


				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_B_DieR", true);
					else
						mAnimator->PlayAnimation(L"Player_B_DieL", true);
				}

				else if (mWeaponType == eWeaponType::PISTOL)
				{
				}

				break;
			}

		
	}



	void PlayerScript::Enter_Floor()
	{


	


	}

	void PlayerScript::Enter_TreeFloor()
	{

		
		mRigidbody->SetGround(true);

		mRigidbody->SetVelocity(Vector2(0.0f, 0.0f));

		mJumpCount = 0;
		mFallTime = 0.0f;


		ChangeState(ePlayerState::IDLE);

	}

	void PlayerScript::GuardEnd()
	{
		mGuardColObj->SetState(ss::GameObject::eState::Dead);
	}

	void PlayerScript::AttackEnd()
	{

		//mCollider->SetSize(Vector2(0.2f, 0.8f));
		//mCollider->SetCenter(Vector2(-3.5f, 2.f));

		mAttackColliderObj->RemoveComponent<Collider2D>();

		if (nullptr != mMonster)
		{
			mMonster->GetComponent<MonsterScript>()->SetHit(false);
		}




		//mCollider->SetCenter(Vector2(-3.5f, 2.f));
		//ChangeState(ePlayerState::IDLE);
	}





}