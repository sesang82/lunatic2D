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
#include "ssItem.h"
#include "ssItemScript.h"
#include "ssPistolBulletScript.h"
#include "ssPlayerPistolBullet.h"
#include "ssAudioSource.h"
#include "ssResources.h"
#include "ssSoundMgrScript.h"
#include "ssGoddnessScript.h"


namespace ss
{
	PlayerScript::PlayerScript()
		: mSpeed(50.0f)
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
		, mSPEffect2(nullptr)
		, mbRepeat(false)
		, mAttackEffect(nullptr)
		, mbAttack(false)
		, mPistolBullet(nullptr)
		, mbGetSword(false)
		, mbGetGauntlet(false)
		, mbGetPistol(false)
		, mAudioSource(nullptr)
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
		mAudioSource = GetOwner()->GetComponent<AudioSource>();

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
		mAnimator->EndEvent(L"Player_S_spAttackR") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_S_spAttackL") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_S_OverloadingR") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_S_OverloadingL") = std::bind(&PlayerScript::AttackEnd, this);

		mAnimator->EndEvent(L"Player_P_Attack1,2R") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_P_Attack1,2L") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_P_Attack3R") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_P_Attack3L") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_P_spAttackR") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_P_spAttackL") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_P_OverLoadingR") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_P_OverLoadingL") = std::bind(&PlayerScript::AttackEnd, this);

		mAnimator->EndEvent(L"Player_G_Attack1R") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_G_Attack1L") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_G_Attack2R") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_G_Attack2L") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_G_Attack3R") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_G_Attack3L") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_G_spAttackR") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_G_spAttackL") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_G_overload_Attack1R") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_G_overload_Attack1L") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_G_overload_Attack2R") = std::bind(&PlayerScript::AttackEnd, this);
		mAnimator->EndEvent(L"Player_G_overload_Attack2L") = std::bind(&PlayerScript::AttackEnd, this);

		// ������ �� 
		mAnimator->EndEvent(L"Player_S_GuardR") = std::bind(&PlayerScript::GuardEnd, this);
		mAnimator->EndEvent(L"Player_S_GuardL") = std::bind(&PlayerScript::GuardEnd, this);

		mAnimator->EndEvent(L"Player_P_GuardR") = std::bind(&PlayerScript::GuardEnd, this);
		mAnimator->EndEvent(L"Player_P_GuardL") = std::bind(&PlayerScript::GuardEnd, this);

		mAnimator->EndEvent(L"Player_G_GuardR") = std::bind(&PlayerScript::GuardEnd, this);
		mAnimator->EndEvent(L"Player_G_GuardL") = std::bind(&PlayerScript::GuardEnd, this);

	

		// ==== �÷��̾� Ŭ������ ȭ����� guard�� �� �������. �߰��س��� �̰� �ּ� Ǯ�� 
		//mAnimator->EndEvent(L"Player_B_GuardR") = std::bind(&PlayerScript::GuardEnd, this);
		//mAnimator->EndEvent(L"Player_B_GuardL") = std::bind(&PlayerScript::GuardEnd, this);

		// �÷��̾� ��ũ��Ʈ�� complete�� ȣ���ϸ� Idle�� ��ȯ�Ѵ�.
		//mAnimator->CompleteEvent(L"Player_D_Idle") = std::bind(&PlayerScript::Complete, this);


		// �� ���ݿ� �浹ü 
		mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"PlayerAttackCollider");
		mAttackColliderObj->Initialize();
		PlayerAttackColScript* colScript = mAttackColliderObj->AddComponent<PlayerAttackColScript>();
		colScript->SetOriginOwner(mTransform->GetOwner());
			
		mAttackColTr = mAttackColliderObj->GetComponent<Transform>();


		// ====== ���� ���� 
		mAnimator->StartEvent(L"Player_S_Attack1R") = std::bind(&PlayerScript::SwordAttack_1, this);
		mAnimator->StartEvent(L"Player_S_Attack1L") = std::bind(&PlayerScript::SwordAttack_1, this);

		mAnimator->StartEvent(L"Player_S_Attack2R") = std::bind(&PlayerScript::SwordAttack_2, this);
		mAnimator->StartEvent(L"Player_S_Attack2L") = std::bind(&PlayerScript::SwordAttack_2, this);

		mAnimator->StartEvent(L"Player_S_Attack3R") = std::bind(&PlayerScript::SwordAttack_3, this);
		mAnimator->StartEvent(L"Player_S_Attack3L") = std::bind(&PlayerScript::SwordAttack_3, this);

		
		mAnimator->StartEvent(L"Player_S_spAttackR") = std::bind(&PlayerScript::SwordSPAttack, this);
		mAnimator->StartEvent(L"Player_S_spAttackL") = std::bind(&PlayerScript::SwordSPAttack, this);

		mAnimator->StartEvent(L"Player_S_OverloadingR") = std::bind(&PlayerScript::SwordOverload, this);
		mAnimator->StartEvent(L"Player_S_OverloadingL") = std::bind(&PlayerScript::SwordOverload, this);
		
		mAnimator->StartEvent(L"Player_P_Attack1,2R") = std::bind(&PlayerScript::PistolAttack_1_2, this);
		mAnimator->StartEvent(L"Player_P_Attack1,2L") = std::bind(&PlayerScript::PistolAttack_1_2, this);
		mAnimator->StartEvent(L"Player_P_Attack3R") = std::bind(&PlayerScript::PistolAttack_3, this);
		mAnimator->StartEvent(L"Player_P_Attack3L") = std::bind(&PlayerScript::PistolAttack_3, this);
		mAnimator->StartEvent(L"Player_P_spAttackR") = std::bind(&PlayerScript::PistolSPAttack, this);
		mAnimator->StartEvent(L"Player_P_spAttackL") = std::bind(&PlayerScript::PistolSPAttack, this);
		mAnimator->StartEvent(L"Player_P_OverLoadingR") = std::bind(&PlayerScript::PistolOverload, this);
		mAnimator->StartEvent(L"Player_P_OverLoadingL") = std::bind(&PlayerScript::PistolOverload, this);



		mAnimator->StartEvent(L"Player_G_Attack1R") = std::bind(&PlayerScript::GuntletAttack_1, this);
		mAnimator->StartEvent(L"Player_G_Attack1L") = std::bind(&PlayerScript::GuntletAttack_1, this);
		mAnimator->StartEvent(L"Player_G_Attack2R") = std::bind(&PlayerScript::GuntletAttack_2, this);
		mAnimator->StartEvent(L"Player_G_Attack2L") = std::bind(&PlayerScript::GuntletAttack_2, this);
		mAnimator->StartEvent(L"Player_G_Attack3R") = std::bind(&PlayerScript::GuntletAttack_3, this);
		mAnimator->StartEvent(L"Player_G_Attack3L") = std::bind(&PlayerScript::GuntletAttack_3, this);
		mAnimator->StartEvent(L"Player_G_spAttackR") = std::bind(&PlayerScript::GuntletSPAttackReady, this);
		mAnimator->StartEvent(L"Player_G_spAttackL") = std::bind(&PlayerScript::GuntletSPAttackReady, this);
		mAnimator->StartEvent(L"Player_G_overload_Attack1R") = std::bind(&PlayerScript::GuntletOverload_1, this);
		mAnimator->StartEvent(L"Player_G_overload_Attack1L") = std::bind(&PlayerScript::GuntletOverload_1, this);
		mAnimator->StartEvent(L"Player_G_overload_Attack2R") = std::bind(&PlayerScript::GuntletOverload_2, this);
		mAnimator->StartEvent(L"Player_G_overload_Attack2L") = std::bind(&PlayerScript::GuntletOverload_2, this);



	}
	void PlayerScript::Update()
	{


		Vector3 pos = mTransform->GetPosition();
	
			mWeaponType = SceneManager::GetWeaponInfo();
			
		
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


	/*	if (mTurnOverload)
		{
			float DecreseSpeed = 15.f;

			mState->SetDecreaseOverload(Time::DeltaTime() * DecreseSpeed);

			if (mState->GetCurrentOverload() <= 0.f)
			{
				mTurnOverload = false;

				mPlayerOverloadEffect->SetState(GameObject::eState::Dead);
				mbPlayerOverloadingEffet = false;

				mOverloadUIEffect->SetState(GameObject::eState::Dead);

				mBottomHorizion->SetState(GameObject::eState::Dead);
				mLT->SetState(GameObject::eState::Dead);
				mLB->SetState(GameObject::eState::Dead);
				mRT->SetState(GameObject::eState::Dead);
				mRB->SetState(GameObject::eState::Dead);
				mTopHorizion->SetState(GameObject::eState::Dead);

			}
		}*/

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

	
		

		else if (L"L_Stage1_col_Door" == other->GetOwner()->GetName())
		{
			SceneManager::LoadScene(L"Boss1Scene");
		}

		else if (L"col_startBoss1StageObj" == other->GetOwner()->GetName())
		{
			// storm �������� ���� �ٲٱ� 
			mMonster->GetComponent<BigWolfScript>()->ChangeState(eWolfBossState::STOM_START);
	
		}

		// ���� ���� ���� 
		else if (L"WoodAttackColObj" == other->GetOwner()->GetName()
			|| L"WolfAttackColObj" == other->GetOwner()->GetName())
		{			
			// �뽬 �߿� ���� ����ȭ 
			if (!mbDash)
			{
				mState->SetCurrentHP(mState->GetCurrentHP() - 10);

				// ���� ������ �� HIT ���·� ���� 
				ChangeState(ePlayerState::HIT);

			}

		}

		else if (L"Sword_RightToLeft" == other->GetOwner()->GetName())
		{
			int i = 0; // ȸ�� �ణ �浹 �ȸ´µ� ���Ǵ� ����
		}


		// ==== ���� �� �Ѿ�� �浹ü 
		else if (L"L_col_Door" == other->GetOwner()->GetName())
		{
			SceneManager::LoadScene(L"EntranceScene");
		}
		
		else if (L"L_col_Door2" == other->GetOwner()->GetName())
		{
			SceneManager::LoadScene(L"EntryScene");
		}

		else if (L"L_col_Door3" == other->GetOwner()->GetName())
		{
			SceneManager::LoadScene(L"Stage1Scene");
		}
	
		else if (L"Boss2Start_Platform" == other->GetOwner()->GetName())
		{
	
			mMonster->GetComponent<GoddnessScript>()->ChangeState(eBoss2_Phase1::IDLE);
			other->GetOwner()->SetState(GameObject::eState::Dead);


		}

	}

	void PlayerScript::OnCollisionStay(Collider2D* other)
	{

		if (L"col_TreeSword" == other->GetOwner()->GetName())
		{

			if (Input::GetKeyDown(eKeyCode::F))
			{

				// ui�� Į �̹����� ������ �Ѵ�.
				mbGetSword = true;
				SceneManager::SetOnSword(mbGetSword);


				// fŰ�� ������ Į idle �ִϸ��̼����� �ٲ�� �Ѵ�.
				mChangeFirst = true;
				mWeaponType = eWeaponType::SWORD;
				SceneManager::SetWeaponInfo(mWeaponType);
				mbChange = true;
				

			}

		}	

		else if (L"col_TreePistol" == other->GetOwner()->GetName())
		{

			if (Input::GetKeyDown(eKeyCode::F))
			{

				// ui�� Į �̹����� ������ �Ѵ�.
				SceneManager::SetOnPistol(true);


				// fŰ�� ������ Į idle �ִϸ��̼����� �ٲ�� �Ѵ�.
				mChangeFirst = true;
				mWeaponType = eWeaponType::PISTOL;
				SceneManager::SetWeaponInfo(mWeaponType);
				mbChange = true; // ����Ʈ�� ���������� ���� 


			}

		}

		else if (L"col_TreeGauntlet" == other->GetOwner()->GetName())
		{

			if (Input::GetKeyDown(eKeyCode::F))
			{

				// ui�� Į �̹����� ������ �Ѵ�.
				SceneManager::SetOnGauntlet(true);

				// fŰ�� ������ Į idle �ִϸ��̼����� �ٲ�� �Ѵ�.
				mChangeFirst = true;
				mWeaponType = eWeaponType::GAUNTLET;
				SceneManager::SetWeaponInfo(mWeaponType);
				mbChange = true;


			}

		}

		// ������  ** ������
		else if (L"overload_Item" == other->GetOwner()->GetName())
		{
			if (Input::GetKeyDown(eKeyCode::F))
			{
				mState->SetCurOverload(100);
				
				Item* obj = (Item*)other->GetOwner();
				obj->SetState(GameObject::eState::Dead);
			}

		}


		else if (L"WolfPotal" == other->GetOwner()->GetName())
		{
			if (Input::GetKeyDown(eKeyCode::ENTER))
			{
				SceneManager::LoadScene(L"Boss2Scene");
				other->GetOwner()->SetState(GameObject::eState::Dead);
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

		else if (mAttackCount == 2 && mWeaponType == eWeaponType::GAUNTLET && mTurnOverload)
		{
			mAttackCount = 0; 
		}

		// ���ڸ����� ����  (�� �տ� �־ߵ�) 
		if (Input::GetKeyDown(eKeyCode::C))
		{
			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
			pSFX->SetClip(Resources::Find<AudioClip>(L"Player_Jump_Bgm"));
			pSFX->Play();
			pSFX->SetVolume(0.3f);
		

			mRigidbody->SetGravity(Vector2(0.f, 1500.f));

			mVelocity = mRigidbody->GetVelocity();
			mVelocity.y = -500.f;
			mRigidbody->SetVelocity(mVelocity);
			mRigidbody->SetGround(false);
			++mJumpCount;
			mbIdleJump = true;
			mbJump = false;

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
			if (!mTurnOverload &&  mWeaponType == eWeaponType::SWORD
				|| !mTurnOverload && mWeaponType == eWeaponType::GAUNTLET
				|| mTurnOverload && mWeaponType == eWeaponType::GAUNTLET
				|| !mTurnOverload && mWeaponType == eWeaponType::PISTOL)
			{
				++mAttackCount;
			}

	
			ChangeState(ePlayerState::ATTACK);
		}

		else if (Input::GetKeyDown(eKeyCode::Z) && mAttackCount == 1)
		{
			++mAttackCount;
		
			ChangeState(ePlayerState::ATTACK);

		}

		else if (Input::GetKeyDown(eKeyCode::Z) && mAttackCount == 2)
		{
			if (mTurnOverload && mWeaponType == eWeaponType::GAUNTLET)
				return;

			++mAttackCount;
		

			ChangeState(ePlayerState::ATTACK);
		}


		// �뽬
		else if (Input::GetKeyDown(eKeyCode::X))
		{
			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
			pSFX->SetClip(Resources::Find<AudioClip>(L"Player_Dash_Bgm"));
			pSFX->Play();
			pSFX->SetVolume(0.3f);

		
			// �뽬 ����
			mbDash = true;
			DashDuration = 0.5f;  // �뽬 ���� �ð� ����

			// Run�� ���� �ӷ��� �ְ� ������, Idle���� �ƹ��� Ű�� ���� ���� ��� �ӵ��� 0�̶� ���Ƿ� �ο����� 

			if (mPrevDir.x > 0 && !mTurnOverload)
			{
				mRigidbody->SetVelocity(Vector2(300.f, 0.f));
			}

			else if (mPrevDir.x > 0 && mTurnOverload)
			{
				mRigidbody->SetVelocity(Vector2(400.f, 0.f));
			}


			if (mPrevDir.x < 0 && !mTurnOverload)
			{
				mRigidbody->SetVelocity(Vector2(-300.f, 0.f));
			}

			else if (mPrevDir.x < 0 && mTurnOverload)
			{
				mRigidbody->SetVelocity(Vector2(-400.f, 0.f));
			}


			mVelocity.x *= 100 * Time::DeltaTime();  // �ӵ� ����

			ChangeState(ePlayerState::DASH);
		}

		// Guard
		else if (Input::GetKeyDown(eKeyCode::LCTRL))
		{
			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
			pSFX->SetClip(Resources::Find<AudioClip>(L"Player_GaurdHit_Bgm"));
			pSFX->Play();
			pSFX->SetVolume(0.3f);


			Vector3 pos = mTransform->GetPosition();

			// Guard�� �浹ü
			mGuardColObj = object::Instantiate<AttackCollider>(pos,eLayerType::Guard, L"PlayerGuardObj");
			mGuardColObj->Initialize();
			PlayerGuardColScript* guardScript = mGuardColObj->AddComponent<PlayerGuardColScript>();
			mGuardCol = mGuardColObj->AddComponent<Collider2D>();
			mGuardCol->SetName(L"PlayerGuardCol");
			mGuardCol->SetSize(Vector2(30.f, 50.f));

			if (mCurDir.x > 0)
			{
				
				mGuardCol->SetCenter(Vector2(15.f, 2.f));

				// Guard�� ����Ʈ
				mGuardEffectObj = object::Instantiate<Effect>(pos, eLayerType::Effect, L"PlayerGuardEffectObjR");
				EffectScript* effectscript = mGuardEffectObj->AddComponent<EffectScript>();
			}

			else
			{
				
				mGuardCol->SetCenter(Vector2(-5.5f, 2.f));

				// Guard�� ����Ʈ
				mGuardEffectObj = object::Instantiate<Effect>(pos, eLayerType::Effect, L"PlayerGuardEffectObjL");
				EffectScript* effectscript = mGuardEffectObj->AddComponent<EffectScript>();
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
				ChangeState(ePlayerState::OVERLOAD_READY);
		
				AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
				pSFX->SetClip(Resources::Find<AudioClip>(L"Player_Overload_Bgm"));
				pSFX->Play();
				pSFX->SetVolume(0.1f);


				mTurnOverload = true;

			}
			
		}


		// ���� ��ü
		else if (Input::GetKeyDown(eKeyCode::A) && SceneManager::IsOnSword())
		{
			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
			pSFX->SetClip(Resources::Find<AudioClip>(L"Player_WeaponChange_Bgm"));
			pSFX->Play();
			pSFX->SetVolume(0.1f);

			mWeaponType = eWeaponType::SWORD;
			SceneManager::SetWeaponInfo(mWeaponType);
		}

		else if (Input::GetKeyDown(eKeyCode::S) && SceneManager::IsOnPistol())
		{
			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
			pSFX->SetClip(Resources::Find<AudioClip>(L"Player_WeaponChange_Bgm"));
			pSFX->Play();
			pSFX->SetVolume(0.1f);

			mWeaponType = eWeaponType::PISTOL;
			SceneManager::SetWeaponInfo(mWeaponType);
		}

		else if (Input::GetKeyDown(eKeyCode::D) && SceneManager::IsOnGauntlet())
		{
			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
			pSFX->SetClip(Resources::Find<AudioClip>(L"Player_WeaponChange_Bgm"));
			pSFX->Play();
			pSFX->SetVolume(0.1f);


			mWeaponType = eWeaponType::GAUNTLET;
			SceneManager::SetWeaponInfo(mWeaponType);
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
			if(!mTurnOverload)
			mRigidbody->SetVelocity(Vector2(300.f, 0.f));

			else
			mRigidbody->SetVelocity(Vector2(400.f, 0.f));


		}

		else if (Input::GetKeyUp(eKeyCode::RIGHT))
		{
			mRigidbody->SetVelocity(Vector2(0.f, 0.f));
		}


		//=============================
				// ����
		if (Input::GetKey(eKeyCode::LEFT))
		{
			if (!mTurnOverload)
			mRigidbody->SetVelocity(Vector2(-300.f, 0.f));

			else
				mRigidbody->SetVelocity(Vector2(-400.f, 0.f));


		}

		else if (Input::GetKeyUp(eKeyCode::LEFT))
		{
			mRigidbody->SetVelocity(Vector2(0.f, 0.f));		
		}


		// ========�뽬 
		if (Input::GetKeyDown(eKeyCode::X))
		{
			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
			pSFX->SetClip(Resources::Find<AudioClip>(L"Player_Dash_Bgm"));
			pSFX->Play();
			pSFX->SetVolume(0.3f);


			// �뽬 ����
			mbDash = true;
			DashDuration = 0.5f;  // �뽬 ���� �ð� ����
			mVelocity.x *= 100 * Time::DeltaTime();  // �ӵ��� ���߿� ���߱� 

			ChangeState(ePlayerState::DASH);
		}

		// ����
		if (Input::GetKeyDown(eKeyCode::C))
		{
			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
			pSFX->SetClip(Resources::Find<AudioClip>(L"Player_Jump_Bgm"));
			pSFX->Play();
			pSFX->SetVolume(0.3f);



			mbJump = false;

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
			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
			pSFX->SetClip(Resources::Find<AudioClip>(L"Player_GaurdHit_Bgm"));
			pSFX->Play();
			pSFX->SetVolume(0.3f);


			// move���¿��� guard ��� ���ڸ��� �ֵ��� ������Ŵ 
			mRigidbody->SetVelocity(Vector2(0.f, 0.f));


			Vector3 pos = mTransform->GetPosition();

			// Guard�� �浹ü
			mGuardColObj = object::Instantiate<AttackCollider>(pos, eLayerType::Guard, L"PlayerGuardObj");
			mGuardColObj->Initialize();
			PlayerGuardColScript* guardScript = mGuardColObj->AddComponent<PlayerGuardColScript>();
			mGuardCol = mGuardColObj->AddComponent<Collider2D>();
			

			if (mCurDir.x > 0)
			{
				mGuardCol->SetSize(Vector2(4.f, 20.f));
				mGuardCol->SetCenter(Vector2(25.f, 3.f));

				// Guard�� ����Ʈ
				mGuardEffectObj = object::Instantiate<Effect>(pos, eLayerType::Effect, L"PlayerGuardEffectObjR");
				EffectScript* effectscript = mGuardEffectObj->AddComponent<EffectScript>();
			}

			else
			{
				mGuardCol->SetSize(Vector2(4.f, 20.f));
				mGuardCol->SetCenter(Vector2(-17.5f, 3.f));

				// Guard�� ����Ʈ
				mGuardEffectObj = object::Instantiate<Effect>(pos, eLayerType::Effect, L"PlayerGuardEffectObjL");
				EffectScript* effectscript = mGuardEffectObj->AddComponent<EffectScript>();
			}


			guardScript->StorePlayerScript(this);

			ChangeState(ePlayerState::GUARD);
		}


		else if (Input::GetKeyDown(eKeyCode::Z))
		{

			mRigidbody->SetVelocity(Vector2::Zero);

			if (!mTurnOverload && mWeaponType == eWeaponType::SWORD
				|| !mTurnOverload && mWeaponType == eWeaponType::GAUNTLET
				|| mTurnOverload && mWeaponType == eWeaponType::GAUNTLET
				|| !mTurnOverload && mWeaponType == eWeaponType::PISTOL)
			{
				++mAttackCount;
			}


			ChangeState(ePlayerState::ATTACK);
		}

		else if (Input::GetKeyDown(eKeyCode::Z) && mAttackCount == 1)
		{
			mRigidbody->SetVelocity(Vector2::Zero);

			++mAttackCount;

			ChangeState(ePlayerState::ATTACK);

		}

		else if (Input::GetKeyDown(eKeyCode::Z) && mAttackCount == 2)
		{
			mRigidbody->SetVelocity(Vector2::Zero);

			if (mTurnOverload && mWeaponType == eWeaponType::GAUNTLET)
				return;


			++mAttackCount;


			ChangeState(ePlayerState::ATTACK);
		}


		// SP ����
		else if (Input::GetKeyDown(eKeyCode::LSHIFT))
		{
			mRigidbody->SetVelocity(Vector2::Zero);
			ChangeState(ePlayerState::SPATTACK);

			// SP ������ ���ҽ�Ű�� (-30) && mTurnOverload ���°� �ƴҋ��� 

		}

		// ������ (������ �������� 100% �� ���� �۵��ϵ��� ���߿� �ٲٱ�)
		else if (Input::GetKeyDown(eKeyCode::G))
		{	
			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
			pSFX->SetClip(Resources::Find<AudioClip>(L"Player_Overload_Bgm"));
			pSFX->Play();
			pSFX->SetVolume(0.3f);

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

		// ���� ��ü
		else if (Input::GetKeyDown(eKeyCode::A) && SceneManager::IsOnSword())
		{
			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
			pSFX->SetClip(Resources::Find<AudioClip>(L"Player_WeaponChange_Bgm"));
			pSFX->Play();
			pSFX->SetVolume(0.3f);

	
			mWeaponType = eWeaponType::SWORD;
			SceneManager::SetWeaponInfo(mWeaponType);
		}

		else if (Input::GetKeyDown(eKeyCode::S) && SceneManager::IsOnPistol())
		{
			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
			pSFX->SetClip(Resources::Find<AudioClip>(L"Player_WeaponChange_Bgm"));
			pSFX->Play();
			pSFX->SetVolume(0.3f);

			mWeaponType = eWeaponType::PISTOL;
			SceneManager::SetWeaponInfo(mWeaponType);
		}

		else if (Input::GetKeyDown(eKeyCode::D) && SceneManager::IsOnGauntlet())
		{
			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
			pSFX->SetClip(Resources::Find<AudioClip>(L"Player_WeaponChange_Bgm"));
			pSFX->Play();
			pSFX->SetVolume(0.3f);

			mWeaponType = eWeaponType::GAUNTLET;
			SceneManager::SetWeaponInfo(mWeaponType);
		}


	}
	void PlayerScript::Jump()
	{
		mbJump = true;

		Vector3 pos = mTransform->GetPosition();

		// Idle ���·� �پ��� �� 2�� �̻� �� �ٰ� ���� 
		if (Input::GetKeyDown(eKeyCode::Z) && mJumpCount > 2)
			return;



		if (Input::GetKeyDown(eKeyCode::C) && mJumpCount == 1)
		{
			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
			pSFX->SetClip(Resources::Find<AudioClip>(L"Player_Jump_Bgm"));
			pSFX->Play();
			pSFX->SetVolume(0.3f);

		
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

		mRigidbody->SetGravity(Vector2(0.f, 1500.f));
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

			if (mTurnOverload && mWeaponType == eWeaponType::PISTOL)
			{
				ChangeState(ePlayerState::OVERLOAD_START);
			}

			else
			{
				ChangeState(ePlayerState::IDLE);
			}
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


		 if (mAnimator->GetCurActiveAnimation()->IsComplete() && mWeaponType == eWeaponType::PISTOL)
		{

			if (mbRepeat)
			{
				ChangeState(ePlayerState::OVERLOAD_START);
				
				
				
			}

			else
			{
				ChangeState(ePlayerState::OVERLOAD_END);
			}
		}

		
	}




	void PlayerScript::Overload_End()
	{
		if (mAnimator->GetCurActiveAnimation()->IsComplete() && mWeaponType == eWeaponType::PISTOL)
		{
			ChangeState(ePlayerState::IDLE);

		}
	}

	void PlayerScript::Hit()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
		pSFX->SetClip(Resources::Find<AudioClip>(L"Player_Hit_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.1f);


		// hit ���� ��� ���ڸ��� �����ֵ��� �ӵ� 0���� ����
		mRigidbody->SetVelocity(Vector2(0.f, 0.f));
		mAttackColliderObj->RemoveComponent<Collider2D>();

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
			mGuardColObj->SetState(GameObject::eState::Dead);
		}

	}




	void PlayerScript::Animation()
	{
		// �ִϸ��̼� ������Ʈ�� ��� ���� �ʵ��� ���ó��
		// ���� ���¿� ���� ���°� �ٸ� ���� �ִϸ��̼� ����� �ǵ��� �Ѵ�
		// (Update�� ������ ������ ���� ���·� �������� ������ 1�ʸ��� ��� play�ǹǷ� �����ִ°�ó�� ����)



		if (mCurState != mPrevState || mCurDir != mPrevDir
	       || mChangeFirst&& mCurState == ePlayerState::IDLE
		   || mCurState == ePlayerState::OVERLOAD_START && mPrevState == ePlayerState::OVERLOAD_START)

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
					if (!mTurnOverload)
					{
						if (mAnimator->GetCurActiveAnimation()->GetIndex() == 2)
						{

							if (mAttackCount == 1)
							{
							}

							else if (mAttackCount == 2)
							{
								
							}

							else if (mAttackCount == 3)
							{
							
							}

							if (mPrevDir.x > 0)
							{

								mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
								mAttackCol->SetSize(Vector2(50.f, 40.f));
								mAttackCol->SetCenter(Vector2(40.f, 2.f));
							}

							else if (mPrevDir.x < 0)
							{
								mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
								mAttackCol->SetSize(Vector2(50.f, 40.f));
								mAttackCol->SetCenter(Vector2(-25.f, 2.f));
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


					else if (mTurnOverload)
					{
						mAttackColliderObj->RemoveComponent<Collider2D>();

						if (mAnimator->GetCurActiveAnimation()->GetIndex() == 1
							|| mAnimator->GetCurActiveAnimation()->GetIndex() == 3
							|| mAnimator->GetCurActiveAnimation()->GetIndex() == 5
							|| mAnimator->GetCurActiveAnimation()->GetIndex() == 7)
						{
							CameraScript* camera = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();

							camera->StartShake(0.01f, 0.06f); // ������ �ʹ� ũ�� �ָ� ����� �����ε� ������ �����Ű��Ƽ� �������θ��ϱ� 

							if (mPrevDir.x > 0)
							{

								mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
								mAttackCol->SetSize(Vector2(100.f, 40.f));
								mAttackCol->SetCenter(Vector2(65.f, 2.f));
							}

							else if (mPrevDir.x < 0)
							{
								mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
								mAttackCol->SetSize(Vector2(100.f, 40.f));
								mAttackCol->SetCenter(Vector2(-55.f, 2.f));
							}

							

						}

					


						if (mPrevDir.x > 0)
							mAnimator->PlayAnimation(L"Player_S_OverloadingR", false);
						else
							mAnimator->PlayAnimation(L"Player_S_OverloadingL", false);



					}
				}


				else if (mWeaponType == eWeaponType::GAUNTLET)
				{
					mRigidbody->SetVelocity(Vector2(0.f, 0.f));

					if (!mTurnOverload)
					{
					
						


						 if (mAttackCount == 1)
						{
							


							CameraScript* camera = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
							camera->StartShake(0.01f, 0.05f);


							if (mAnimator->GetCurActiveAnimation()->GetIndex() == 0)
							{
								mAttackColliderObj->RemoveComponent<Collider2D>();
							}

							else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 1)
								
							{
								
								if (mPrevDir.x > 0)
								{

									mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
									mAttackCol->SetSize(Vector2(34.f, 40.f));
									mAttackCol->SetCenter(Vector2(16.f, 2.f));
								}

								else if (mPrevDir.x < 0)
								{
									mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
									mAttackCol->SetSize(Vector2(34.f, 40.f));
									mAttackCol->SetCenter(Vector2(-6.f, 2.f));
								}


							}

							else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 4)
							{
						

								if (mPrevDir.x > 0)
								{

									mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
									mAttackCol->SetSize(Vector2(34.f, 40.f));
									mAttackCol->SetCenter(Vector2(16.f, 2.f));
								}

								else if (mPrevDir.x < 0)
								{
									mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
									mAttackCol->SetSize(Vector2(34.f, 40.f));
									mAttackCol->SetCenter(Vector2(-6.f, 2.f));
								}

							}

							else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 2
								|| mAnimator->GetCurActiveAnimation()->GetIndex() == 5)
							{
								mAttackColliderObj->RemoveComponent<Collider2D>();
							}



							if (mPrevDir.x > 0)
								mAnimator->PlayAnimation(L"Player_G_Attack1R", false);
							else
								mAnimator->PlayAnimation(L"Player_G_Attack1L", false);
						}

						else if (mAttackCount == 2)
						{

	
							CameraScript* camera = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
							camera->StartShake(0.01f, 0.05f);

							if (mAnimator->GetCurActiveAnimation()->GetIndex() == 1)
							{
							

								if (mPrevDir.x > 0)
								{

									mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
									mAttackCol->SetSize(Vector2(34.f, 40.f));
									mAttackCol->SetCenter(Vector2(16.f, 2.f));
								}

								else if (mPrevDir.x < 0)
								{
									mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
									mAttackCol->SetSize(Vector2(34.f, 40.f));
									mAttackCol->SetCenter(Vector2(-6.f, 2.f));
								}
							}

							else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 2)
							{
								mAttackColliderObj->RemoveComponent<Collider2D>();
							}


							if (mPrevDir.x > 0)
								mAnimator->PlayAnimation(L"Player_G_Attack2R", false);
							else
								mAnimator->PlayAnimation(L"Player_G_Attack2L", false);
						}



						else if (mAttackCount == 3)
						{

							CameraScript* camera = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
							camera->StartShake(0.03f, 0.15f);

							if (mAnimator->GetCurActiveAnimation()->GetIndex() == 2)
							{
							
								if (mPrevDir.x > 0)
								{

									mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
									mAttackCol->SetSize(Vector2(34.f, 40.f));
									mAttackCol->SetCenter(Vector2(16.f, 2.f));
								}

								else if (mPrevDir.x < 0)
								{
									mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
									mAttackCol->SetSize(Vector2(34.f, 40.f));
									mAttackCol->SetCenter(Vector2(-6.f, 2.f));
								}
							}

							else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 3)
							{
								mAttackColliderObj->RemoveComponent<Collider2D>();
							}

							if (mPrevDir.x > 0)
								mAnimator->PlayAnimation(L"Player_G_Attack3R", false);
							else
								mAnimator->PlayAnimation(L"Player_G_Attack3L", false);
						}

					}

					// �����ε� ���¶�� 
					else
					{
						if (mAttackCount == 1)
						{
							mAttackColliderObj->RemoveComponent<Collider2D>();

							CameraScript* camera = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
							camera->StartShake(0.03f, 0.15f);

							if (mAnimator->GetCurActiveAnimation()->GetIndex() == 1
								|| mAnimator->GetCurActiveAnimation()->GetIndex() == 3
								|| mAnimator->GetCurActiveAnimation()->GetIndex() == 5)
							{

								if (mPrevDir.x > 0)
								{

									mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
									mAttackCol->SetSize(Vector2(45.f, 40.f));
									mAttackCol->SetCenter(Vector2(25.f, 2.f));
								}

								else if (mPrevDir.x < 0)
								{
									mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
									mAttackCol->SetSize(Vector2(45.f, 40.f));
									mAttackCol->SetCenter(Vector2(-25.f, 2.f));
								}
							}


							if (mPrevDir.x > 0)
								mAnimator->PlayAnimation(L"Player_G_overload_Attack1R", false);
							else if (mPrevDir.x < 0)
								mAnimator->PlayAnimation(L"Player_G_overload_Attack1L", false);
						}

						else if (mAttackCount == 2)
						{
							mAttackColliderObj->RemoveComponent<Collider2D>();

							CameraScript* camera = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
							camera->StartShake(0.01f, 0.15f);

							if (mAnimator->GetCurActiveAnimation()->GetIndex() == 1)
								
							{

								if (mPrevDir.x > 0)
								{

									mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
									mAttackCol->SetSize(Vector2(45.f, 40.f));
									mAttackCol->SetCenter(Vector2(25.f, 2.f));
								}

								else if (mPrevDir.x < 0)
								{
									mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
									mAttackCol->SetSize(Vector2(45.f, 40.f));
									mAttackCol->SetCenter(Vector2(-25.f, 2.f));
								}
								
							}

							else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 4)

							{
								CameraScript* camera = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
								camera->StartShake(0.05f, 0.3f);

								if (mPrevDir.x > 0)
								{

									mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
									mAttackCol->SetSize(Vector2(45.f, 40.f));
									mAttackCol->SetCenter(Vector2(15.f, 2.f));
								}

								else if (mPrevDir.x < 0)
								{
									mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
									mAttackCol->SetSize(Vector2(45.f, 40.f));
									mAttackCol->SetCenter(Vector2(-15.f, 2.f));
								}

							}

							if (mPrevDir.x > 0)
								mAnimator->PlayAnimation(L"Player_G_overload_Attack2R", false);
							else if (mPrevDir.x < 0)
								mAnimator->PlayAnimation(L"Player_G_overload_Attack2L", false);

						}


					}

				}

				else if (mWeaponType == eWeaponType::PISTOL)
				{
					mRigidbody->SetVelocity(Vector2(0.f, 0.f));


					if (!mTurnOverload)
					{
						if (mAttackCount == 1 
							|| mAttackCount == 2)
						{
						
							// �Ѿ� �߻� , ����Ʈ ���� 
						    if (mAnimator->GetCurActiveAnimation()->GetIndex() == 1)
							{
								CameraScript* camera = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();

								camera->StartShake(0.01f, 0.06f);

								if (mPrevDir.x > 0)
								{

									Vector3 PlayerPos = Vector3(mTransform->GetPosition().x + 55.f, mTransform->GetPosition().y, mTransform->GetPosition().z);


									mAttackEffect = object::Instantiate<Effect>(PlayerPos, eLayerType::Effect, L"Player_Pistol_AttackEffectR");

									EffectScript* effectscript = mAttackEffect->AddComponent<EffectScript>();
									
							

									effectscript->SetOriginOwner((Player*)mTransform->GetOwner());

									if (!mbAttack)
									{
										mbAttack = true;

										Vector3 BulletPos = Vector3(mTransform->GetPosition().x + 55.f, mTransform->GetPosition().y + 8.f, mTransform->GetPosition().z);

										// �Ѿ� �߻�
										mPistolBullet = object::Instantiate<PlayerPistolBullet>(BulletPos, eLayerType::Collision, L"Pistolbullet_Small_ObjR");
										mPistolBullet->AddComponent<PistolBulletScript>();
										

									

									}
								



								}


								else
								{
									Vector3 PlayerPos = Vector3(mTransform->GetPosition().x - 55.f, mTransform->GetPosition().y, mTransform->GetPosition().z);


									mAttackEffect = object::Instantiate<Effect>(PlayerPos, eLayerType::Effect, L"Player_Pistol_AttackEffectL");

									EffectScript* effectscript = mAttackEffect->AddComponent<EffectScript>();
							
						
									if (!mbAttack)
									{
										mbAttack = true;

										Vector3 BulletPos = Vector3(mTransform->GetPosition().x - 55.f, mTransform->GetPosition().y + 8.f, mTransform->GetPosition().z);

										// �Ѿ� �߻�
										mPistolBullet = object::Instantiate<PlayerPistolBullet>(BulletPos, eLayerType::Collision, L"Pistolbullet_Small_ObjL");
										mPistolBullet->AddComponent<PistolBulletScript>();




									}
								}

							}

							else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 2)
							{
								mbAttack = false;

							}

							if (mPrevDir.x > 0)
								mAnimator->PlayAnimation(L"Player_P_Attack1,2R", false);
							else
								mAnimator->PlayAnimation(L"Player_P_Attack1,2L", false);


						}


						else if (mAttackCount == 3)
						{

							// �Ѿ� �߻� , ����Ʈ ���� 

							if (mAnimator->GetCurActiveAnimation()->GetIndex() == 2)
							{
								CameraScript* camera = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();

								camera->StartShake(0.1f, 0.3f);
							}


							else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 3 && !mbAttack)
							{
								

								CameraScript* camera = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();

								camera->StartShake(0.1f, 0.3f);
								mbAttack = true;


								if (mPrevDir.x > 0 && mbAttack)
								{

									Vector3 PlayerPos = Vector3(mTransform->GetPosition().x + 50.f, mTransform->GetPosition().y, mTransform->GetPosition().z);


									mAttackEffect = object::Instantiate<Effect>(PlayerPos, eLayerType::Effect, L"Player_Pistol_Attack_Last_EffectR");

									EffectScript* effectscript = mAttackEffect->AddComponent<EffectScript>();

									effectscript->SetOriginOwner((Player*)mTransform->GetOwner());


									// �Ѿ� �߻� 
									Vector3 BulletPos = Vector3(mTransform->GetPosition().x + 55.f, mTransform->GetPosition().y + 8.f, mTransform->GetPosition().z);

									mPistolBullet = object::Instantiate<PlayerPistolBullet>(BulletPos, eLayerType::Collision, L"Pistolbullet_Big_ObjR");
									mPistolBullet->AddComponent<PistolBulletScript>();

								}

								// ����

								else if(mPrevDir.x < 0 && mbAttack)
								{
									Vector3 PlayerPos = Vector3(mTransform->GetPosition().x - 50.f, mTransform->GetPosition().y, mTransform->GetPosition().z);


									mAttackEffect = object::Instantiate<Effect>(PlayerPos, eLayerType::Effect, L"Player_Pistol_Attack_Last_EffectL");

									EffectScript* effectscript = mAttackEffect->AddComponent<EffectScript>();
				
									effectscript->SetOriginOwner((Player*)mTransform->GetOwner());



									// �Ѿ� �߻� 
									Vector3 BulletPos = Vector3(mTransform->GetPosition().x - 55.f, mTransform->GetPosition().y + 8.f, mTransform->GetPosition().z);

									mPistolBullet = object::Instantiate<PlayerPistolBullet>(BulletPos, eLayerType::Collision, L"Pistolbullet_Big_ObjL");
									mPistolBullet->AddComponent<PistolBulletScript>();
								}


							}


							if (mAnimator->GetCurActiveAnimation()->GetIndex() == 4)
							{
								mbAttack = false;
							}



							if (mPrevDir.x > 0)
								mAnimator->PlayAnimation(L"Player_P_Attack3R", false);
							else
								mAnimator->PlayAnimation(L"Player_P_Attack3L", false);

						}
					}


					// �����ε� ���̶�� 
					else
					{
						if (mPrevDir.x > 0)
							mAnimator->PlayAnimation(L"Player_P_OverLoadReadyR", false);
						else
							mAnimator->PlayAnimation(L"Player_P_OverLoadReadyL", false);

					}

				}

				break;






			case ss::ePlayerState::SPATTACK:
				if (mWeaponType == eWeaponType::SWORD)
				{
					mRigidbody->SetVelocity(Vector2(0.f, 0.f));
				

					if (mAnimator->GetCurActiveAnimation()->GetIndex() == 2)
					{
					
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


					// �� ���� ��ũ��Ʈ���ٰ� bool�� �̿��ذ����� true�� ��� pause���·� �д� �̷��� �����α� 
					else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 3)
					{
						if (mPrevDir.x > 0)
						{
							Vector3 CurPos = Vector3(mTransform->GetPosition().x + 1.5f, mTransform->GetPosition().y, mTransform->GetPosition().z);

						    mTransform->SetPosition(CurPos);
							//mPrevDir.x * mRigidbody->GetVelocity().x;

							mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
							mAttackCol->SetSize(Vector2(34.f, 40.f));
							mAttackCol->SetCenter(Vector2(8.f, 2.f));
							mAttackColliderObj->GetComponent<Transform>()->SetPosition(Vector3(CurPos));


						}


						else if (mPrevDir.x < 0)
						{
							Vector3 CurPos = Vector3(mTransform->GetPosition().x - 1.5f, mTransform->GetPosition().y, mTransform->GetPosition().z);

							mTransform->SetPosition(CurPos);

							mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
							mAttackCol->SetSize(Vector2(34.f, 40.f));
							mAttackCol->SetCenter(Vector2(8.f, 2.f));
							mAttackColliderObj->GetComponent<Transform>()->SetPosition(Vector3(CurPos));

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
						mAttackColliderObj->RemoveComponent<Collider2D>();

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


						if (!mbOverloading)
						{
							if (mAnimator->GetCurActiveAnimation()->GetIndex() == 3)
							{
								
								mbspAttack = false;

								if (mPrevDir.x > 0)
								{
									// ������ ����Ʈ ��ũ��Ʈ���� �������� 
									Vector3 PlayerPos = Vector3(mTransform->GetPosition().x, mTransform->GetPosition().y, mTransform->GetPosition().z);


									mSPEffect = object::Instantiate<Effect>(PlayerPos, eLayerType::Effect, L"Player_Gauntlet_SPEffect_DustR");

									EffectScript* effectscript = mSPEffect->AddComponent<EffectScript>();

									effectscript->SetOriginOwner((Player*)mTransform->GetOwner());
								}

								else if(mPrevDir.x < 0)
								{
									Vector3 PlayerPos = Vector3(mTransform->GetPosition().x, mTransform->GetPosition().y, mTransform->GetPosition().z);


									mSPEffect = object::Instantiate<Effect>(PlayerPos, eLayerType::Effect, L"Player_Gauntlet_SPEffect_DustL");

									EffectScript* effectscript = mSPEffect->AddComponent<EffectScript>();

									effectscript->SetOriginOwner((Player*)mTransform->GetOwner());
								}

							}




							else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 4 && !mbspAttack)
							{
								

								mbspAttack = true;
		

								// ** Ư�� �ε����� ��� �ð� ���� �ٲٴ� �Լ� (�׽�Ʈ �Ϸ�) 
								//mAnimator->GetCurActiveAnimation()->SetCurSpriteDuration(3.f);

								if (mPrevDir.x > 0)
								{
									

									Vector3 PlayerPos = Vector3(mTransform->GetPosition().x, mTransform->GetPosition().y, mTransform->GetPosition().z);


									mSPEffect2 = object::Instantiate<Effect>(PlayerPos, eLayerType::Effect, L"Player_Gauntlet_SPEffect_PushR");

									EffectScript* effectscript = mSPEffect2->AddComponent<EffectScript>();

									effectscript->SetOriginOwner((Player*)mTransform->GetOwner());


									// 
									Vector3 CurPos = mTransform->GetPosition();

									mTransform->SetPosition(CurPos.x + 130.f, CurPos.y, CurPos.z);



									mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
									mAttackCol->SetSize(Vector2(34.f, 40.f));
									mAttackCol->SetCenter(Vector2(8.f, 2.f));

								}

								else
								{
									Vector3 PlayerPos = Vector3(mTransform->GetPosition().x, mTransform->GetPosition().y, mTransform->GetPosition().z);


									mSPEffect2 = object::Instantiate<Effect>(PlayerPos, eLayerType::Effect, L"Player_Gauntlet_SPEffect_PushL");

									EffectScript* effectscript = mSPEffect2->AddComponent<EffectScript>();

									effectscript->SetOriginOwner((Player*)mTransform->GetOwner());

									
									Vector3 CurPos = mTransform->GetPosition();

									mTransform->SetPosition(CurPos.x - 130.f, CurPos.y, CurPos.z);



									mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
									mAttackCol->SetSize(Vector2(34.f, 40.f));
									mAttackCol->SetCenter(Vector2(0.f, 2.f));
								}
							}






							else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 7)
							{
								mAttackColliderObj->RemoveComponent<Collider2D>();

							
							}
						


							if (mPrevDir.x > 0)
								mAnimator->PlayAnimation(L"Player_G_spAttackR", false);
							else
								mAnimator->PlayAnimation(L"Player_G_spAttackL", false);

					

						}
					
				
				}


				// �����ε��� ���� ���Ҷ��� �Ȱ��� 
				else if (mWeaponType == eWeaponType::PISTOL)
				{

			


					// 2,3 �ε��� �Ѿ� ���� �߻� // 10, 11�ε��� �Ѿ� �߻� 
					if (mAnimator->GetCurActiveAnimation()->GetIndex() == 2
						|| mAnimator->GetCurActiveAnimation()->GetIndex() == 10)
					{


						if (mPrevDir.x > 0 && !mbspAttack)
						{
							mbspAttack = true;

							Vector3 PlayerPos = Vector3(mTransform->GetPosition().x + 65.f, mTransform->GetPosition().y, mTransform->GetPosition().z);


							mAttackEffect = object::Instantiate<Effect>(PlayerPos, eLayerType::Effect, L"Player_Pistol_SPAttack_EffectR");

							EffectScript* effectscript = mAttackEffect->AddComponent<EffectScript>();

							effectscript->SetOriginOwner((Player*)mTransform->GetOwner());


							
							Vector3 BulletPos = Vector3(mTransform->GetPosition().x + 55.f, mTransform->GetPosition().y + 8.f, mTransform->GetPosition().z);

							// �Ѿ� �߻�
							mPistolBullet = object::Instantiate<PlayerPistolBullet>(BulletPos, eLayerType::Collision, L"Pistolbullet_Big_ObjR");
							mPistolBullet->AddComponent<PistolBulletScript>();


						}

						else if (mPrevDir.x < 0 && !mbspAttack)
						{
							mbspAttack = true;

							Vector3 PlayerPos = Vector3(mTransform->GetPosition().x - 50.f, mTransform->GetPosition().y, mTransform->GetPosition().z);


							mAttackEffect = object::Instantiate<Effect>(PlayerPos, eLayerType::Effect, L"Player_Pistol_SPAttack_EffectL");

							EffectScript* effectscript = mAttackEffect->AddComponent<EffectScript>();

							effectscript->SetOriginOwner((Player*)mTransform->GetOwner());



							Vector3 BulletPos = Vector3(mTransform->GetPosition().x - 55.f, mTransform->GetPosition().y + 8.f, mTransform->GetPosition().z);

							// �Ѿ� �߻�
							mPistolBullet = object::Instantiate<PlayerPistolBullet>(BulletPos, eLayerType::Collision, L"Pistolbullet_Big_ObjL");
							mPistolBullet->AddComponent<PistolBulletScript>();
						}

						CameraScript* camera = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();

						camera->StartShake(0.1f, 0.3f); // 1, 2�ε������� ���! 
					}

					else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 3
						|| mAnimator->GetCurActiveAnimation()->GetIndex() == 11)
					{

						if (mPrevDir.x > 0 && !mbAttack)
						{
							mbAttack = true;

							Vector3 BulletPos = Vector3(mTransform->GetPosition().x + 55.f, mTransform->GetPosition().y + 8.f, mTransform->GetPosition().z);

							// �Ѿ� �߻�
							mPistolBullet = object::Instantiate<PlayerPistolBullet>(BulletPos, eLayerType::Collision, L"Pistolbullet_Small_ObjR");
							mPistolBullet->AddComponent<PistolBulletScript>();
						}

						else if (mPrevDir.x < 0 && !mbAttack)
						{
							mbAttack = true;

							Vector3 BulletPos = Vector3(mTransform->GetPosition().x - 55.f, mTransform->GetPosition().y + 8.f, mTransform->GetPosition().z);

							// �Ѿ� �߻�
							mPistolBullet = object::Instantiate<PlayerPistolBullet>(BulletPos, eLayerType::Collision, L"Pistolbullet_Small_ObjL");
							mPistolBullet->AddComponent<PistolBulletScript>();
						}

						
					}

					else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 4
						|| mAnimator->GetCurActiveAnimation()->GetIndex() == 12)
					{
						mbspAttack = false;
						mbAttack = false;
					}
					
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
				if (mWeaponType == eWeaponType::PISTOL)
				{
					Vector3 PlayerPos = Vector3(mTransform->GetPosition().x, mTransform->GetPosition().y, mTransform->GetPosition().z);

		
					if (mPrevDir.x > 0)
					{
						
						if (!mbRepeat)
						{
							mAnimator->SetAgainAttack(false);

							mAnimator->PlayAnimation(L"Player_P_OverLoadingR", false);

							// ���ݿ� �浹ü 
							Vector3 CurPos = Vector3(mTransform->GetPosition().x, mTransform->GetPosition().y, mTransform->GetPosition().z);

							mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
							mAttackCol->SetSize(Vector2(100.f, 100.f));
							mAttackCol->SetCenter(Vector2(8.f, 2.f));
							mAttackColliderObj->GetComponent<Transform>()->SetPosition(Vector3(CurPos));



							if (Input::GetKeyDown(eKeyCode::Z))
							{

								mbRepeat = true;


							}
						}

						else if (!mbspAttack)
						{
							mbspAttack = true;

							mSPEffect = object::Instantiate<Effect>(PlayerPos, eLayerType::Effect, L"PlayerPistol_CircleEffectObjR");

							EffectScript* effectscript = mSPEffect->AddComponent<EffectScript>();

							effectscript->SetOriginOwner((Player*)mTransform->GetOwner());
						}


						else if (mbRepeat && mAnimator->GetCurActiveAnimation()->IsComplete())
						{
							mbRepeat = false;

							mAnimator->SetAgainAttack(true);

							mAnimator->PlayAnimation(L"Player_P_OverLoadingR", false);


							// ���ݿ� �浹ü 
							Vector3 CurPos = Vector3(mTransform->GetPosition().x, mTransform->GetPosition().y, mTransform->GetPosition().z);

							mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
							mAttackCol->SetSize(Vector2(100.f, 100.f));
							mAttackCol->SetCenter(Vector2(8.f, 2.f));
							mAttackColliderObj->GetComponent<Transform>()->SetPosition(Vector3(CurPos));

						}



			
						
					}


					else if (mPrevDir.x < 0)
					{
						if (!mbRepeat)
						{
							mAnimator->SetAgainAttack(false);

							mAnimator->PlayAnimation(L"Player_P_OverLoadingL", false);

							// ���ݿ� �浹ü 
							Vector3 CurPos = Vector3(mTransform->GetPosition().x, mTransform->GetPosition().y, mTransform->GetPosition().z);

							mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
							mAttackCol->SetSize(Vector2(100.f, 100.f));
							mAttackCol->SetCenter(Vector2(8.f, 2.f));
							mAttackColliderObj->GetComponent<Transform>()->SetPosition(Vector3(CurPos));


							if (Input::GetKeyDown(eKeyCode::Z))
							{

								mbRepeat = true;


							}
						}

						else if (!mbspAttack)
						{
							mbspAttack = true;


							mSPEffect = object::Instantiate<Effect>(PlayerPos, eLayerType::Effect, L"PlayerPistol_CircleEffectObjL");

							EffectScript* effectscript = mSPEffect->AddComponent<EffectScript>();

							effectscript->SetOriginOwner((Player*)mTransform->GetOwner());

						}

						// zŰ ������ ������ ���� ����ƴ� �ִϸ��̼��� ���� ������ ����ǵ��� 
						else if (mbRepeat && mAnimator->GetCurActiveAnimation()->IsComplete())
						{
							mbRepeat = false;

							mAnimator->SetAgainAttack(true);

							mAnimator->PlayAnimation(L"Player_P_OverLoadingL", false);


							// ���ݿ� �浹ü 
							Vector3 CurPos = Vector3(mTransform->GetPosition().x, mTransform->GetPosition().y, mTransform->GetPosition().z);

							mAttackCol = mAttackColliderObj->AddComponent<Collider2D>();
							mAttackCol->SetSize(Vector2(100.f, 100.f));
							mAttackCol->SetCenter(Vector2(8.f, 2.f));
							mAttackColliderObj->GetComponent<Transform>()->SetPosition(Vector3(CurPos));


						}

					}
				}

				break;

			case ss::ePlayerState::OVERLOAD_END:
				 if (mWeaponType == eWeaponType::GAUNTLET)
				{
					 mbspAttack = false;
					 
					if (mPrevDir.x > 0)
						mAnimator->PlayAnimation(L"Player_G_OverLoadEndR", true);
					else
						mAnimator->PlayAnimation(L"Player_G_OverLoadEndL", true);

				}

				 else if (mWeaponType == eWeaponType::PISTOL)
				 {
					 mAttackColliderObj->RemoveComponent<Collider2D>();

					 if (mPrevDir.x > 0)
						 mAnimator->PlayAnimation(L"Player_P_OverLoadEndR", false);
					 else
						 mAnimator->PlayAnimation(L"Player_P_OverLoadEndL", false);

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

		if (nullptr != mMonster && mMonster->GetState() != GameObject::eState::Dead)
		{
			mMonster->GetComponent<MonsterScript>()->SetHit(false);
		}




		//mCollider->SetCenter(Vector2(-3.5f, 2.f));
		//ChangeState(ePlayerState::IDLE);
	}

	void PlayerScript::SwordAttack_1()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
		pSFX->SetClip(Resources::Find<AudioClip>(L"Sword_Attack1_Bgm"));
		pSFX->Play();
		pSFX->PlaybackSpeed(1.f);

		pSFX->SetVolume(0.3f);

	}

	void PlayerScript::SwordAttack_2()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
		pSFX->SetClip(Resources::Find<AudioClip>(L"Sword_Attack2_Bgm"));
		pSFX->Play();
		pSFX->PlaybackSpeed(1.f);

		pSFX->SetVolume(0.3f);
	}

	void PlayerScript::SwordAttack_3()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
		pSFX->SetClip(Resources::Find<AudioClip>(L"Sword_Attack3_Bgm"));
		pSFX->Play();
		pSFX->PlaybackSpeed(1.f);

		pSFX->SetVolume(0.3f);
	}

	void PlayerScript::SwordSPAttack()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
		pSFX->SetClip(Resources::Find<AudioClip>(L"Sword_SPAttack_Bgm"));
		pSFX->Play();
		pSFX->PlaybackSpeed(1.f);

		pSFX->SetVolume(0.3f);


	}

	// Ư�� �����ӿ� ���ε��ϴ°� �ʿ� 
	void PlayerScript::SwordOverload()
	{

		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
		pSFX->SetClip(Resources::Find<AudioClip>(L"Sword_OverloadAttack1_Bgm"));
		pSFX->Play();
		pSFX->PlaybackSpeed(1.f);

		pSFX->SetVolume(0.3f);


	}

	void PlayerScript::GuntletAttack_1()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
	

		pSFX->SetClip(Resources::Find<AudioClip>(L"Gauntlet_Attack1_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.3f);
	}

	void PlayerScript::GuntletAttack_2()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();


		pSFX->SetClip(Resources::Find<AudioClip>(L"Gauntlet_Attack2_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.3f);
	}

	void PlayerScript::GuntletAttack_3()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();


		pSFX->SetClip(Resources::Find<AudioClip>(L"Gauntlet_Attack3_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.3f);
	}

	void PlayerScript::GuntletSPAttackReady()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();


		pSFX->SetClip(Resources::Find<AudioClip>(L"Gauntlet_SPAttackReady_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.3f);
	}

	// Ư�� �����ӿ� ���ε��ؾߵɵ� 
	void PlayerScript::GuntletSPAttack()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();


		pSFX->SetClip(Resources::Find<AudioClip>(L"Gauntlet_SPAttack_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.3f);
	}

	void PlayerScript::GuntletOverload_1()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();


		pSFX->SetClip(Resources::Find<AudioClip>(L"Gauntlet_OverloadBuff_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.3f);
	}

	void PlayerScript::GuntletOverload_2()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();


		pSFX->SetClip(Resources::Find<AudioClip>(L"Gauntlet_OverloadBuff_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.3f);
	
	}


	void PlayerScript::PistolAttack_1_2()
	{

		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();

		pSFX->SetClip(Resources::Find<AudioClip>(L"Pistol_Attack1_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.3f);

	}


	void PlayerScript::PistolAttack_3()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();


		pSFX->SetClip(Resources::Find<AudioClip>(L"Pistol_Attack2_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.3f);
	}

	// Ư�� �����ӿ� ���� �ߵ� ���Ѿ��ҵ�... 
	void PlayerScript::PistolSPAttack()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
		pSFX->GetClip()->Stop();

		pSFX->SetClip(Resources::Find<AudioClip>(L"Pistol_Attack1_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.3f);
	}

	void PlayerScript::PistolOverload()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
		pSFX->GetClip()->Stop();

		pSFX->SetClip(Resources::Find<AudioClip>(L"Pistol_OverloadAttack_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.3f);
	}






}