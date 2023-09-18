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
		, mSPEffect(nullptr)
		, mbspAttack(false)
		, mbOverloading(false)
		, mbUseOverload(false)
		, mPlayerOverloadEffect(nullptr)
		, mbPlayerOverloadingEffet(false)
		, mTurnOverload(false)
	{
	}
	PlayerScript::~PlayerScript()
	{
	}

	void PlayerScript::Initialize()
	{
	
		mPlayer = (Player*)GetOwner();

	

		mState = GameState::GetInst().GetState(L"Player");  //GetOwner()->GetComponent<CharacterState>();
		mAnimator = GetOwner()->GetComponent<Animator>();
		mTransform = GetOwner()->GetComponent<Transform>();
		mRigidbody = GetOwner()->GetComponent<Rigidbody2D>();
		mCollider = GetOwner()->GetComponent<Collider2D>();
	

		mAnimator->PlayAnimation(L"Player_S_IdleR", true);
		//mAnimator->PlayAnimation(L"Player_D_IdleR", true);

			


		

		// �ִϸ��̼� ���⿡ ���� ����	�� ����ش�.
		mCurDir = mTransform->Right();


		mAnimator->EndEvent(L"Player_S_Attack1R") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_S_Attack1L") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_S_Attack2R") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_S_Attack2L") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_S_Attack3R") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_S_Attack3L") = std::bind(&PlayerScript::AttackEnd, this);

		// ������ �� 
		mAnimator->EndEvent(L"Player_S_GuardR") = std::bind(&PlayerScript::GuardEnd, this);
		mAnimator->EndEvent(L"Player_S_GuardL") = std::bind(&PlayerScript::GuardEnd, this);

		// ==== �÷��̾� Ŭ������ ȭ����� guard�� �� �������. �߰��س��� �̰� �ּ� Ǯ�� 
		//mAnimator->EndEvent(L"Player_B_GuardR") = std::bind(&PlayerScript::GuardEnd, this);
		//mAnimator->EndEvent(L"Player_B_GuardL") = std::bind(&PlayerScript::GuardEnd, this);

		// �÷��̾� ��ũ��Ʈ�� complete�� ȣ���ϸ� Idle�� ��ȯ�Ѵ�.
		//mAnimator->CompleteEvent(L"Player_D_Idle") = std::bind(&PlayerScript::Complete, this);


		// ���ݿ� �浹ü 
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

		case ss::ePlayerState::OVERLOAD_READY:
			Overload_Ready();
			break;

		case ss::ePlayerState::OVERLOAD_START:
			Overload_Start();
			break;

		case ss::ePlayerState::OVERLOADING:
			Overloading();
			break;

		case ss::ePlayerState::OVERLOAD_END:
			Overload_End();
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


		if (mTurnOverload)
		{
			float speed = 15.f;

			mState->SetDecreaseOverload(Time::DeltaTime() * speed);

			if (mState->GetCurrentOverload() <= 0.f)
			{
				mTurnOverload = false;

				mPlayerOverloadEffect->SetState(GameObject::eState::Dead);
				mOverloadUIEffect->SetState(GameObject::eState::Dead);

				mBottomHorizion->SetState(GameObject::eState::Dead);
				mLT->SetState(GameObject::eState::Dead);
				mLB->SetState(GameObject::eState::Dead);
				mRT->SetState(GameObject::eState::Dead);
				mRB->SetState(GameObject::eState::Dead);
				mTopHorizion->SetState(GameObject::eState::Dead);

			}
		}

		// �̵�->���º�ȯ->�ִϸ��̼�


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
		// ���� y�� üũ�� Ÿ�� 
		else if (L"col_SpecialFloor" == other->GetOwner()->GetName())
		{


			// �÷��̾ ���� ���� ���� ���� ���� �۵��ǰ� �� (���� �Ʒ����� �������� �� �����۵��Ǵ°� ����) 
			if (GetOwner()->GetComponent<Collider2D>()->GetPosition().y > other->GetPosition().y)
				Enter_TreeFloor();
		

			

			// �÷��̾��� ��ġ�� ���� �´�.
			// �ش� �浹ü�� ��ġ�� ���� �´�. (�浹ü�̱� ���� ���ӿ�����Ʈ) 


		}

	
		else if (L"L_col_Door" == other->GetOwner()->GetName())
		{
			SceneManager::LoadScene(L"Stage1Scene");
		}

		else if (L"col_startBoss1StageObj" == other->GetOwner()->GetName())
		{
			// storm �������� ���� �ٲٱ� 
			mMonster->GetComponent<BigWolfScript>()->ChangeState(eWolfBossState::STOM_START);
	
		}


	}

	void PlayerScript::OnCollisionStay(Collider2D* other)
	{


		// ==== �ϴ� �̹����� ȭ��������, �� �ִϸ��̼� ������ �غ��� 
		if (L"col_TreeBorrow" == other->GetOwner()->GetName())
		{

			if (Input::GetKeyDown(eKeyCode::F))
			{

				// ui�� Į �̹����� ������ �Ѵ�.

				// fŰ�� ������ Į idle �ִϸ��̼����� �ٲ�� �Ѵ�.
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


			//// �÷��̾ ���� ���� ���� ���� ���� �۵��ǰ� �� (���� �Ʒ����� �������� �� �����۵��Ǵ°� ����) 

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


		// �÷��̾��� ��ġ�� ���� �´�.
		// �ش� �浹ü�� ��ġ�� ���� �´�. (�浹ü�̱� ���� ���ӿ�����Ʈ) 
		else if (L"col_startBoss1StageObj" == other->GetOwner()->GetName())
		{
			mBossStartColObj->SetState(GameObject::eState::Dead);

		}

	}

	void PlayerScript::BindConstantBuffer()
	{

		// ��� ���ۿ� ����, ��, ���� ����� ��� �����ش�. 
		renderer::HitCB hitCB = {};
		hitCB.h_IsHit = mHit;

		// ��� ���� �߿� Transform ��� ���۸� ���� �´�. 
		ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Hit];
		cb->SetData(&hitCB); // ���� ��� ������ ��� ���ۿ� �־��ش�.
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS); // ��� ���۴� ��� ���̴� �ܰ��̵� ���ε��� �� �ִٴ°� �����̴�. 

	}



	void PlayerScript::Idle()
	{


		if (mAttackCount == 3)
		{
			mAttackCount = 0;
		}

		// ���ڸ����� ����  (�� �տ� �־ߵ�) 
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

			ChangeState(ePlayerState::JUMP); // �̰� ����� ���� ������� �̹��� �ȹٲ�

		}


		// �̵�
		else if (Input::GetKey(eKeyCode::RIGHT))
		{
			// Idle ������ ���س��� �� 
			mCurDir = mTransform->Right();
			ChangeState(ePlayerState::RUN);
		}


		// === ����
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


		// �뽬
		else if (Input::GetKeyDown(eKeyCode::X))
		{
			// �뽬 ����
			mbDash = true;
			DashDuration = 0.5f;  // �뽬 ���� �ð� ����

			// Run�� ���� �ӷ��� �ְ� ������, Idle���� �ƹ��� Ű�� ���� ���� ��� �ӵ��� 0�̶� ���Ƿ� �ο����� 

			if(mPrevDir.x > 0)
			mRigidbody->SetVelocity(Vector2(300.f, 0.f));

			else if (mPrevDir.x < 0)
				mRigidbody->SetVelocity(Vector2(-300.f, 0.f));

			mVelocity.x *= 100 * Time::DeltaTime();  // �ӵ� ����

			ChangeState(ePlayerState::DASH);
		}

		// Guard
		else if (Input::GetKeyDown(eKeyCode::LCTRL))
		{
			Vector3 pos = mTransform->GetPosition();

			// Guard�� �浹ü
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

		// SP ����
		else if (Input::GetKeyDown(eKeyCode::LSHIFT))
		{
			ChangeState(ePlayerState::SPATTACK);

			// SP ������ ���ҽ�Ű�� (-30) && mTurnOverload ���°� �ƴҋ��� 

		}


		// ������ (������ �������� 100% �� ���� �۵��ϵ��� ���߿� �ٲٱ�)
		else if (Input::GetKeyDown(eKeyCode::G))
		{
			float CurOverload = mState->GetCurrentOverload();

			if (CurOverload == 100.f && !mTurnOverload)
			{
				// ������ ������ �˾Ƽ� �����ǰ� �ϱ� & ������ �������� 100�̶�� ���¸� �����Ѵ�.
				ChangeState(ePlayerState::OVERLOAD_READY);

				// false�� ������ �������� 0�� �Ǹ� �������� �صα�
				// ���� �Ʒ� bool �� �̿��ؼ� �ִϸ��̼ǿ��� �ش� bool ���� �����ִٸ� �ٸ� �� �����ϰ� �صα� 
				mTurnOverload = true;

			}
			
		}



	}


	// �����̴� �κп� ���� ������������ (�� ���� ���� �����غ��� ���� �����ϸ� �ȵ�)
	void PlayerScript::Run()
	{
		//====================== X�� �뽬 
		if (Input::GetKeyUp(eKeyCode(eKeyCode::RIGHT)) || Input::GetKeyUp(eKeyCode(eKeyCode::LEFT))
			|| Input::GetKeyUp(eKeyCode(eKeyCode::Z)) || Input::GetKeyUp(eKeyCode(eKeyCode::X)) || Input::GetKeyUp(eKeyCode(eKeyCode::C)))
		{
			ChangeState(ePlayerState::IDLE);
		}

		// === ������
		if (Input::GetKey(eKeyCode::RIGHT))
		{
			mRigidbody->SetVelocity(Vector2(300.f, 0.f));


		}

		else if (Input::GetKeyUp(eKeyCode::RIGHT))
		{
			mRigidbody->SetVelocity(Vector2(0.f, 0.f));
		}


		//=============================
				// ����
		if (Input::GetKey(eKeyCode::LEFT))
		{
			mRigidbody->SetVelocity(Vector2(-300.f, 0.f));


		}

		else if (Input::GetKeyUp(eKeyCode::LEFT))
		{
			mRigidbody->SetVelocity(Vector2(0.f, 0.f));		
		}


		// ========�뽬 
		if (Input::GetKeyDown(eKeyCode::X))
		{
			// �뽬 ����
			mbDash = true;
			DashDuration = 0.5f;  // �뽬 ���� �ð� ����
			mVelocity.x *= 100 * Time::DeltaTime();  // �ӵ��� ���߿� ���߱� 

			ChangeState(ePlayerState::DASH);
		}

		// ����
		if (Input::GetKeyDown(eKeyCode::C))
		{
			mRigidbody->SetGravity(Vector2(0.f, 1500.f));

				mVelocity = mRigidbody->GetVelocity();

				// �ִϸ��̼� ���� �Ʒ�ó�� ���ǹ��� ���� 
				if (mPrevDir == mTransform->Right())
					mVelocity.x = 500.f;
				else
					mVelocity.x = -500.f;

				mJumpPower = sqrt(2 * abs(mRigidbody->GetGravity().y) * 80.f); // * ���� ����

			
					mVelocity.y = -mJumpPower;

				mRigidbody->SetVelocity(mVelocity);

				mRigidbody->SetGround(false);
				mFallTime += Time::DeltaTime();
				++mJumpCount;

			
				ChangeState(ePlayerState::JUMP); // �̰� ����� ���� ������� �̹��� �ȹٲ�
			


		}


		// Guard
		else if (Input::GetKeyDown(eKeyCode::LCTRL))
		{
			// move���¿��� guard ��� ���ڸ��� �ֵ��� ������Ŵ 
			mRigidbody->SetVelocity(Vector2(0.f, 0.f));


			Vector3 pos = mTransform->GetPosition();

			// Guard�� �浹ü
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

		// Idle ���·� �پ��� �� 2�� �̻� �� �ٰ� ���� 
		if (Input::GetKeyDown(eKeyCode::Z) && mJumpCount > 2)
			return;



		if (Input::GetKeyDown(eKeyCode::C) && mJumpCount == 1)
		{
		
			mVelocity = mRigidbody->GetVelocity();

			// Idle������ ������ ������ ���°� �ʿ�����Ƿ� 0���� �������
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


		// ���� �߿� ����Ű ������ �ش� �������� ����Ǹ鼭 ���������� �� 
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
	
		// ó���� ������ �õ����� ���� y���� ����Ǹ� �ȵǹǷ� �Ʒ�ó�� ����
		mVelocity.y = mRigidbody->GetVelocity().y;
		mRigidbody->SetVelocity(mVelocity);



	}

	void PlayerScript::Fall()
	{

		// ���� ���ǿ��� ������ �� �������� ��� (falling) ��� �ֱ� 
		// ����ϰ� �������� �͵� �����ؾ��� (���� ������ �� �� �浹ü�� ��� ũ�⸦ ����� �������� �Ѵٰų� �׷� �� �ϸ� �ɵ� )

	}


	void PlayerScript::Dash()
	{


		// �뽬 �ð��� 0.2�ʰ� �Ѿ�� �뽬�� ���� ������ ����

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

		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(ePlayerState::IDLE);
		}

	}

	void PlayerScript::SPAttack()
	{
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(ePlayerState::IDLE);
			mbOverloading = false;
		
		}
	}

	void PlayerScript::Overload_Ready()
	{
	}

	void PlayerScript::Overload_Start()
	{

	}

	void PlayerScript::Overloading()
	{
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(ePlayerState::IDLE);
		}
	}

	void PlayerScript::Overload_End()
	{

	}

	void PlayerScript::Hit()
	{
		
		// hit ���� ��� ���ڸ��� �����ֵ��� �ӵ� 0���� ����
		mRigidbody->SetVelocity(Vector2(0.f, 0.f));

		if (!mbHitting)
		{
			mHitEffect = object::Instantiate<Effect>(eLayerType::Effect, L"PlayerHitUI");

			EffectScript* effectscript = mHitEffect->AddComponent<EffectScript>();
			effectscript->SetOriginOwner((Player*)mTransform->GetOwner());
			mbHitting = true;
		}

		// �ִϸ��̼� ����� ������ 
		if (mbHitting && mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(ePlayerState::IDLE);
			mbHitting = false;
		}

	}

	void PlayerScript::Guard()
	{

		// �ִϸ��̼� ����� ������ 
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			ChangeState(ePlayerState::IDLE);
		}

	}




	void PlayerScript::Animation()
	{
		// �ִϸ��̼� ������Ʈ�� ��� ���� �ʵ��� ���ó��
		// ���� ���¿� ���� ���°� �ٸ� ���� �ִϸ��̼� ����� �ǵ��� �Ѵ�
		// (Update�� ������ ������ ���� ���·� �������� ������ 1�ʸ��� ��� play�ǹǷ� �����ִ°�ó�� ����)



		if (mCurState != mPrevState || mCurDir != mPrevDir ||
			mChangeFirst && mCurState == ePlayerState::IDLE)

		{
		}

			switch (mCurState)
			{
			case ss::ePlayerState::IDLE:

				mCollider->SetCenter(Vector2(6.f, -2.f));

				if (mWeaponType == eWeaponType::NONE)
				{
				

					if (mCurDir.x > 0)
					{
						mAnimator->PlayAnimation(L"Player_D_IdleR", true);
					
					}

					else
					{
						mAnimator->PlayAnimation(L"Player_D_IdleL", true);
				
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
						
						}

						else
						{
							mAnimator->PlayAnimation(L"Player_S_IdleL", true);

		
						}

					}


					if (mCurDir.x > 0)
					{
						mAnimator->PlayAnimation(L"Player_S_IdleR", true);

						
					}

					else
					{
						mAnimator->PlayAnimation(L"Player_S_IdleL", true);
					
						
					}
				}

				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
				


					if (mChangeFirst)
					{
						mChangeFirst = false;

						if (mPrevDir.x > 0)
						{
							mAnimator->PlayAnimation(L"Player_G_IdleR", true);
	
						}

						else
						{
							mAnimator->PlayAnimation(L"Player_G_IdleL", true);
	
						}

					}


					if (mCurDir.x > 0)
					{
						mAnimator->PlayAnimation(L"Player_G_IdleR", true);
			
					}

					else
					{
						mAnimator->PlayAnimation(L"Player_G_IdleL", true);
					}
				}

				else if (mWeaponType == eWeaponType::PISTOL)
				{
				

					if (mChangeFirst)
					{
						mChangeFirst = false;


						if (mPrevDir.x > 0)
						{
							mAnimator->PlayAnimation(L"Player_P_IdleR", true);
							
						}

						else
						{
							mAnimator->PlayAnimation(L"Player_P_IdleL", true);

						}


					}


					if (mCurDir.x > 0)
					{
						mAnimator->PlayAnimation(L"Player_P_IdleR", true);
						
					}

					else
					{
						mAnimator->PlayAnimation(L"Player_P_IdleL", true);
					
					}

				}

				break;

			case ss::ePlayerState::RUN:

				if (mWeaponType == eWeaponType::NONE)
				{
					if (mCurDir.x > 0)
					{
						mAnimator->PlayAnimation(L"Player_D_RunR", true);
						
					}

					else
					{
						mAnimator->PlayAnimation(L"Player_D_RunL", true);
				
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
						mAnimator->PlayAnimation(L"Player_G_RunR", true);
					else
						mAnimator->PlayAnimation(L"Player_G_RunL", true);
				}

				else if (mWeaponType == eWeaponType::PISTOL)
				{
					if (mCurDir.x > 0)
						mAnimator->PlayAnimation(L"Player_P_RunR", true);
					else
						mAnimator->PlayAnimation(L"Player_P_RunL", true);
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
						mAnimator->PlayAnimation(L"Player_G_HangL", true);
					else
						mAnimator->PlayAnimation(L"Player_G_HangL", true);
				}

				else if (mWeaponType == eWeaponType::PISTOL)
				{
					if (mCurDir.x > 0)
						mAnimator->PlayAnimation(L"Player_P_HangL", true);
					else
						mAnimator->PlayAnimation(L"Player_P_HangL", true);
				}

				break;


			case ss::ePlayerState::JUMP:

				mCollider->SetCenter(Vector2(6.f, -2.f));

				if (mWeaponType == eWeaponType::NONE)
				{
					if (mCurDir.x > 0)
					{
						mAnimator->PlayAnimation(L"Player_D_JumpR", false);
					
					}


					else
					{
						mAnimator->PlayAnimation(L"Player_D_JumpL", false);
				
					}
				}

				else if (mWeaponType == eWeaponType::SWORD)
				{


					if (mCurDir.x > 0)
					{
						mAnimator->PlayAnimation(L"Player_S_JumpR", false);
						
					}

					else
					{
						mAnimator->PlayAnimation(L"Player_S_JumpL", false);
				
					}
				}

				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					if (mCurDir.x > 0)
					{
						mAnimator->PlayAnimation(L"Player_G_JumpR", false);
					}

					else
					{
						mAnimator->PlayAnimation(L"Player_G_JumpL", false);
					}
				}


				else if (mWeaponType == eWeaponType::PISTOL)
				{

					if (mCurDir.x > 0)
					{
						mAnimator->PlayAnimation(L"Player_P_JumpR", false);
					}

					else
					{
						mAnimator->PlayAnimation(L"Player_P_JumpL", false);
					}

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
					}
					else
					{
						mAnimator->PlayAnimation(L"Player_S_FallL", true);
					}
				}


				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					if (mPrevDir.x > 0)
					{

						mAnimator->PlayAnimation(L"Player_G_FallR", true);
					}
					else
					{
						mAnimator->PlayAnimation(L"Player_G_FallL", true);
					}
				}


				else if (mWeaponType == eWeaponType::PISTOL)
				{
					if (mPrevDir.x > 0)
					{
						mAnimator->PlayAnimation(L"Player_P_FallR", true);
					}


					else
					{
						mAnimator->PlayAnimation(L"Player_P_FallL", true);
					}
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
						mAnimator->PlayAnimation(L"Player_G_DashR", true);
					else
						mAnimator->PlayAnimation(L"Player_G_DashL", true);
				}


				else if (mWeaponType == eWeaponType::PISTOL)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_P_DashR", true);
					else
						mAnimator->PlayAnimation(L"Player_P_DashL", true);
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
						mAnimator->PlayAnimation(L"Player_G_HitR", false);
					else
						mAnimator->PlayAnimation(L"Player_G_HitL", false);
				}

				else if (mWeaponType == eWeaponType::PISTOL)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_P_HitR", false);
					else
						mAnimator->PlayAnimation(L"Player_P_HitL", false);
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
						mAnimator->PlayAnimation(L"Player_G_GuardR", false);
					else
						mAnimator->PlayAnimation(L"Player_G_GuardL", false);
				}

				else if (mWeaponType == eWeaponType::PISTOL)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_P_GuardR", false);
					else
						mAnimator->PlayAnimation(L"Player_P_GuardL", false);
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
						CameraScript* camera = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();

						camera->StartShake(0.01f, 0.05f);

						if (mPrevDir.x > 0)
						{


					
							mAnimator->PlayAnimation(L"Player_S_Attack1R", false);
						}

						else
							mAnimator->PlayAnimation(L"Player_S_Attack1L", false);
					}

					else if (mAttackCount == 2)
					{
						CameraScript* camera = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();

						camera->StartShake(0.01f, 0.05f);

						if (mPrevDir.x > 0)
							mAnimator->PlayAnimation(L"Player_S_Attack2R", false);
						else
							mAnimator->PlayAnimation(L"Player_S_Attack2L", false);
					}

					else if (mAttackCount == 3)
					{
						CameraScript* camera = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();

						camera->StartShake(0.1f, 0.3f); // 1, 2�ε������� ���! 

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
							mAnimator->PlayAnimation(L"Player_G_Attack1R", false);
						else
							mAnimator->PlayAnimation(L"Player_G_Attack1L", false);
					}

					else if (mAttackCount == 2)
					{

						if (mPrevDir.x > 0)
							mAnimator->PlayAnimation(L"Player_G_Attack2R", false);
						else
							mAnimator->PlayAnimation(L"Player_G_Attack2L", false);
					}

					else if (mAttackCount == 3)
					{

						if (mPrevDir.x > 0)
							mAnimator->PlayAnimation(L"Player_G_Attack3R", false);
						else
							mAnimator->PlayAnimation(L"Player_G_Attack3L", false);
					}
				}

				else if (mWeaponType == eWeaponType::PISTOL)
				{

					if (mAttackCount == 1)
					{

						if (mPrevDir.x > 0)
							mAnimator->PlayAnimation(L"Player_P_Attack1,2R", false);
						else
							mAnimator->PlayAnimation(L"Player_P_Attack1,2L", false);
					}

					else if (mAttackCount == 2)
					{

						if (mPrevDir.x > 0)
							mAnimator->PlayAnimation(L"Player_P_Attack1,2R", false);
						else
							mAnimator->PlayAnimation(L"Player_P_Attack1,2L", false);
					}


					else if (mAttackCount == 3)
					{

						if (mPrevDir.x > 0)
							mAnimator->PlayAnimation(L"Player_P_Attack3R", false);
						else
							mAnimator->PlayAnimation(L"Player_P_Attack3L", false);
					}

				}

				break;

			case ss::ePlayerState::SPATTACK:
				if (mWeaponType == eWeaponType::SWORD)
				{

					if (mAnimator->GetCurActiveAnimation()->GetIndex() == 2)
					{
						// hit ���� ��� ���ڸ��� �����ֵ��� �ӵ� 0���� ����
						mRigidbody->SetVelocity(Vector2(0.f, 0.f));

						if (!mbOverloading)
						{
							if (mPrevDir.x > 0)
							{
								Vector3 PlayerPos = Vector3(mTransform->GetPosition().x + 107.f, mTransform->GetPosition().y, mTransform->GetPosition().z);
								

								mSPEffect = object::Instantiate<Effect>(PlayerPos, eLayerType::Effect, L"Player_Sword_SPEffectR");

								EffectScript* effectscript = mSPEffect->AddComponent<EffectScript>();
								mbOverloading = true;
								//Transform* effectTR = mSPEffect->GetComponent<Transform>();
								
								effectscript->SetOriginOwner((Player*)mTransform->GetOwner());
							}

							else if (mPrevDir.x < 0)
							{
								Vector3 PlayerPos = Vector3(mTransform->GetPosition().x - 107.f, mTransform->GetPosition().y, mTransform->GetPosition().z);


								mbOverloading = true;
								mSPEffect = object::Instantiate<Effect>(PlayerPos, eLayerType::Effect, L"Player_Sword_SPEffectL");

								EffectScript* effectscript = mSPEffect->AddComponent<EffectScript>();
								effectscript->SetOriginOwner((Player*)mTransform->GetOwner());
							}
						}

					}


					else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 3)
					{
						if (mPrevDir.x > 0)
						{
							Vector3 CurPos = mTransform->GetPosition();

						    mTransform->SetPosition(CurPos.x + 0.7f, CurPos.y, CurPos.z);
							//mPrevDir.x * mRigidbody->GetVelocity().x;


						}


						else if (mPrevDir.x < 0)
						{
							Vector3 CurPos = mTransform->GetPosition();

							mTransform->SetPosition(CurPos.x - 0.7f, CurPos.y, CurPos.z);
						}
						
						
						
						/*
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
						}*/


					}

					else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 4)
					{
						CameraScript* camera = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();

						camera->StartShake(0.1f, 0.3f);
					}




					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_S_spAttackR", false);
					else
						mAnimator->PlayAnimation(L"Player_S_spAttackL", false);
				}

				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_G_spAttackR", false);
					else
						mAnimator->PlayAnimation(L"Player_G_spAttackL", false);
				}


				else if (mWeaponType == eWeaponType::PISTOL)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_P_spAttackR", false);
					else
						mAnimator->PlayAnimation(L"Player_P_spAttackL", false);
				}
				break;

			case ss::ePlayerState::OVERLOAD_READY:
				
				if (!mbUseOverload)
				{
					mbUseOverload = true;

					if (mPrevDir.x > 0)
					{
						
						// player�� start ����Ʈ ��� 
						mPlayerOverloadEffect = object::Instantiate<Effect>(mTransform->GetPosition(), eLayerType::Effect, L"OverloadStratEffectR");						
					}

					else if (mPrevDir.x < 0)
					{
						// player�� start ����Ʈ ��� 
						mPlayerOverloadEffect = object::Instantiate<Effect>(mTransform->GetPosition(), eLayerType::Effect, L"OverloadStratEffectL");
						EffectScript* effectscript = mPlayerOverloadEffect->AddComponent<EffectScript>();
						effectscript->SetOriginOwner((Player*)mTransform->GetOwner());
					}

					// ***bool�� ������ �����°� �����ϴ°� ���ֱ��ؾߵ� 
					EffectScript* effectscript = mPlayerOverloadEffect->AddComponent<EffectScript>();
					effectscript->SetOriginOwner((Player*)mTransform->GetOwner());

					// overload�� UI
					mOverloadUIEffect = object::Instantiate<Effect>(eLayerType::Effect, L"OverloadUI");

					EffectScript* effectscript2 = mOverloadUIEffect->AddComponent<EffectScript>();
					effectscript2->SetOriginOwner((Player*)mTransform->GetOwner());



					// OVERLOAD UI�� ����� �༮�� 

					mLT = object::Instantiate<Effect>(eLayerType::Effect, L"OverloadLTUI");

					EffectScript* effectscript3 = mLT->AddComponent<EffectScript>();
					effectscript3->SetOriginOwner((Player*)mTransform->GetOwner());


					mLB = object::Instantiate<Effect>(eLayerType::Effect, L"OverloadLBUI");

					EffectScript* effectscript4 = mLB->AddComponent<EffectScript>();
					effectscript4->SetOriginOwner((Player*)mTransform->GetOwner());


					mRT = object::Instantiate<Effect>(eLayerType::Effect, L"OverloadRTUI");

					EffectScript* effectscript5 = mRT->AddComponent<EffectScript>();
					effectscript5->SetOriginOwner((Player*)mTransform->GetOwner());


					mRB = object::Instantiate<Effect>(eLayerType::Effect, L"OverloadRBUI");

					EffectScript* effectscript6 = mRB->AddComponent<EffectScript>();
					effectscript6->SetOriginOwner((Player*)mTransform->GetOwner());


					mTopHorizion = object::Instantiate<Effect>(eLayerType::Effect, L"OverloadTopHorizionUI");

					EffectScript* effectscript7 = mTopHorizion->AddComponent<EffectScript>();
					effectscript7->SetOriginOwner((Player*)mTransform->GetOwner());


					mBottomHorizion = object::Instantiate<Effect>(eLayerType::Effect, L"OverloadBottomHorizionUI");

					EffectScript* effectscript8 = mBottomHorizion->AddComponent<EffectScript>();
					effectscript8->SetOriginOwner((Player*)mTransform->GetOwner());
				}
	
				// �ִϸ��̼� ����� ������ 
				if (mbUseOverload && mPlayerOverloadEffect->GetComponent<Animator>()->GetCurActiveAnimation()->IsComplete())
				{
					mbUseOverload = false; 

					if (!mbPlayerOverloadingEffet)
					{
						mbPlayerOverloadingEffet = true;

						// ����Ʈ ������ update���� ������ 0�� �� �����ǰ� �ص� 
						if (mPrevDir.x > 0)
						{
							mPlayerOverloadEffect = object::Instantiate<Effect>(mTransform->GetPosition(), eLayerType::Effect, L"PlayerOverloadingEffectR");

							EffectScript* effectscript = mPlayerOverloadEffect->AddComponent<EffectScript>();
							effectscript->SetOriginOwner((Player*)mTransform->GetOwner());
						}

						else if (mPrevDir.x < 0)
						{
							mPlayerOverloadEffect = object::Instantiate<Effect>(mTransform->GetPosition(), eLayerType::Effect, L"PlayerOverloadingEffectL");

							EffectScript* effectscript = mPlayerOverloadEffect->AddComponent<EffectScript>();
							effectscript->SetOriginOwner((Player*)mTransform->GetOwner());
						}
			

					}

						ChangeState(ePlayerState::IDLE);
				}

				break;
			

			case ss::ePlayerState::OVERLOAD_START:
				if (mWeaponType == eWeaponType::SWORD)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_S_OverLoadReadyR", false);
					else
						mAnimator->PlayAnimation(L"Player_S_OverLoadReadyL", false);
				}

				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_G_OverLoadReadyR", false);
					else
						mAnimator->PlayAnimation(L"Player_G_OverLoadReadyL", false);
				}


				else if (mWeaponType == eWeaponType::PISTOL)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_P_OverLoadReadyR", false);
					else
						mAnimator->PlayAnimation(L"Player_P_OverLoadReadyL", false);
				}
				break;

			case ss::ePlayerState::OVERLOADING:
				if (mWeaponType == eWeaponType::SWORD)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_S_OverLoadingR", true);
					else
						mAnimator->PlayAnimation(L"Player_S_OverLoadingL", true);
				}

				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_G_OverLoadingR", true);
					else
						mAnimator->PlayAnimation(L"Player_G_OverLoadingL", true);
				}


				else if (mWeaponType == eWeaponType::PISTOL)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_P_OverLoadingR", true);
					else
						mAnimator->PlayAnimation(L"Player_P_OverLoadingL", true);
				}
				break;


			case ss::ePlayerState::OVERLOAD_END:
				if (mWeaponType == eWeaponType::SWORD)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_S_OverLoadEndR", true);
					else
						mAnimator->PlayAnimation(L"Player_S_OverLoadEndL", true);
				}

				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_G_OverLoadEndR", true);
					else
						mAnimator->PlayAnimation(L"Player_G_OverLoadEndL", true);
				}


				else if (mWeaponType == eWeaponType::PISTOL)
				{
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_P_OverLoadEndR", true);
					else
						mAnimator->PlayAnimation(L"Player_P_OverLoadEndL", true);
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