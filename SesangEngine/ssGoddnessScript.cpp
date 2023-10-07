#include "ssGoddnessScript.h"
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
#include "ssRenderer.h"
#include "ssGameState.h"
#include "ssEffectScript.h"
#include "ssEnergyball.h"
#include "ssSmallEnergyballScript.h"
#include "ssSceneManager.h"
#include <random>
#include "ssRenderer.h"
#include "ssCameraScript.h"
#include "ssBackground.h"
#include "ssPlatform.h"
#include "ssSwordBullet.h"
#include "ssBigEnergyballScript.h"
#include "ssBigEnergyball.h"
#include "ssSmallEnergyball.h"


namespace ss
{
	GoddnessScript::GoddnessScript()
		: miStompCount(0)
		, miCompleteStompCount(0)
		, mbStomp(false)
		, mTest(false)
		, mbEnergySpawn(false)
		, mStatueState(eStatueState::MOVING_DOWN)
		, mbFirstStomp(false)
		, mLetterBox(false)
		, mLetterBoxBottom(nullptr)
		, mLetterBoxUP(nullptr)
		, mBossName(nullptr)
		, mPlatformLB(nullptr)
		, mPlatformLT(nullptr)
		, mPlatformRT(nullptr)
		, mPlatformRB(nullptr)
		, mGroundLT(nullptr)
		, mGroundLB(nullptr)
		, mGroundRT(nullptr)
		, mGroundRB(nullptr)
		, mPlatformMidle(nullptr)
		, mGroundMidle(nullptr)
		, mbMovingDown(false)
		, mbMovingDiagonally(true)
		, mSwordBullet_Mid(nullptr)
		, mSwordBullet_Right(nullptr)
		, mSwordBullet_Left(nullptr)
		, mbSwordSpawn(false)
		, mbDirHitSpawn(false)
		, mHitDir_Mid(nullptr)
		, mHitDir_Left(nullptr)
		, mHitDir_Right(nullptr)
		, mPlayerPos(Vector3::Zero)
		, mbFreezingPos(false)
		, miRandom(0)
		, mSpawnDirCount(0)
		, mfChangeTime(0)
		, mSummonState(eSummonState::SPAWN_DIRHIT)
		, mbFireSwordBullet(false)
		, mbFirstSpawnDone(false)
		, mbSecondSpawnDone(false)
		, mbSummonFinish(false)





	{
		m_tMonsterInfo.m_fSpeed = 200.f;
		m_tMonsterInfo.m_fAttack = 10.f;
		m_tMonsterInfo.m_fDetectRange = 300.f;

		activeStates.insert(eSummonState::SPAWN_DIRHIT);
	}



	GoddnessScript::~GoddnessScript()
	{
	}
	void GoddnessScript::Initialize()
	{
		MonsterScript::Initialize();



		GetOwner()->AddComponent<Rigidbody2D>();

		mCharacterState->SetMaxHP(110.f);
		mCharacterState->SetCurrentHP(10.f);

		mMeshRenderer->SetMaterial(Resources::Find<Material>(L"BossAnimMtrl"));


		Transform* tr = GetOwner()->GetComponent<Transform>();
		tr->SetScale(269.f, 276.f, 1.f);



		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Boss2-1_Idle");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Boss2-1_Stomp");
		std::shared_ptr<ss::graphics::Texture> Image3 = Resources::Find<ss::graphics::Texture>(L"Boss2-1_EnergyballStart");
		std::shared_ptr<ss::graphics::Texture> Image4 = Resources::Find<ss::graphics::Texture>(L"Boss2-1_EnergyballEnd");
		std::shared_ptr<ss::graphics::Texture> Image5 = Resources::Find<ss::graphics::Texture>(L"Boss2-1_Die");

		// ==================
		std::shared_ptr<ss::graphics::Texture> Image6 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_Idle");
		std::shared_ptr<ss::graphics::Texture> Image7 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_Intro");
		std::shared_ptr<ss::graphics::Texture> Image8 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_IntroEnd");
		std::shared_ptr<ss::graphics::Texture> Image9 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_MoveFront");
		std::shared_ptr<ss::graphics::Texture> Image10 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_MoveBack");
		std::shared_ptr<ss::graphics::Texture> Image11 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_Hit");


		std::shared_ptr<ss::graphics::Texture> Image12 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_Counter_Start");
		std::shared_ptr<ss::graphics::Texture> Image13 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_Countering");
		std::shared_ptr<ss::graphics::Texture> Image14 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_Counter_End");
		std::shared_ptr<ss::graphics::Texture> Image15 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_Counter_Hit");

		std::shared_ptr<ss::graphics::Texture> Image16 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_DiagonalAttack_Ready");
		std::shared_ptr<ss::graphics::Texture> Image17 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_DiagonalAttack_Vertical");
		std::shared_ptr<ss::graphics::Texture> Image18 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_DiagonalAttack_Diagonal");

		std::shared_ptr<ss::graphics::Texture> Image19 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_Energyball_Start");
		std::shared_ptr<ss::graphics::Texture> Image20 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_Energyballing");
		std::shared_ptr<ss::graphics::Texture> Image21 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_Energyball_End");

		std::shared_ptr<ss::graphics::Texture> Image22 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_GroundSpearPeople");

		std::shared_ptr<ss::graphics::Texture> Image23 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_ShiledBeam_Ready");
		std::shared_ptr<ss::graphics::Texture> Image24 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_ShiledBeam_ing");
		std::shared_ptr<ss::graphics::Texture> Image25 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_ShiledBeam_End");

		std::shared_ptr<ss::graphics::Texture> Image26 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_SummonSpear");
		std::shared_ptr<ss::graphics::Texture> Image27 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_Die");


		// ==== 1������ ���� 
		mAnimator->Create(L"Boss_Goddness_Idle", Image1, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 2, Vector2(269.f, 308.f));
		mAnimator->Create(L"Boss_Goddness_Stomp", Image2, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 4, Vector2(269.f, 308.f));
		mAnimator->Create(L"Boss_Goddness_EnergyballStart", Image3, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 11, Vector2(269.f, 308.f));
		mAnimator->Create(L"Boss_Goddness_EnergyballEnd", Image4, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 5, Vector2(269.f, 308.f));
		mAnimator->Create(L"Boss_Goddness_Die", Image5, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 16, Vector2(269.f, 308.f));


		// ==== 2������ ��
		mAnimator->Create(L"Boss2_Goddness_Idle", Image6, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 8, Vector2(269.f, 276.f), Vector2::Zero, 0.2f);
		mAnimator->Create(L"Boss2_Goddness_IdleR", Image6, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 8, Vector2(269.f, 276.f), Vector2::Zero, 0.2f, true);
		mAnimator->Create(L"Boss2_Goddness_Intro", Image7, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 17, Vector2(269.f, 276.f), Vector2::Zero, 0.11f);
		mAnimator->Create(L"Boss2_Goddness_IntroEnd", Image8, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 8, Vector2(269.f, 276.f));

		mAnimator->Create(L"Boss2_Goddness_MoveFrontR", Image9, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 6, Vector2(269.f, 276.f), Vector2::Zero, 0.15f);
		mAnimator->Create(L"Boss2_Goddness_MoveFrontL", Image9, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 6, Vector2(269.f, 276.f), Vector2::Zero, 0.15f, true);

		mAnimator->Create(L"Boss2_Goddness_MoveBackR", Image10, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 6, Vector2(269.f, 276.f), Vector2::Zero, 0.15f);
		mAnimator->Create(L"Boss2_Goddness_MoveBackL", Image10, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 6, Vector2(269.f, 276.f), Vector2::Zero, 0.15f, true);



		mAnimator->Create(L"Boss2_Goddness_Hit", Image11, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 5, Vector2(269.f, 276.f));

		mAnimator->Create(L"Boss2_Goddness_CounterStart", Image12, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 13, Vector2(269.f, 276.f));
		mAnimator->Create(L"Boss2_Goddness_Countering", Image13, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 5, Vector2(269.f, 276.f));
		mAnimator->Create(L"Boss2_Goddness_CounterEnd", Image14, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 3, Vector2(269.f, 276.f));
		mAnimator->Create(L"Boss2_Goddness_Die", Image15, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 16, Vector2(269.f, 276.f));
		mAnimator->Create(L"Boss2_Goddness_Die", Image16, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 16, Vector2(269.f, 276.f));
		mAnimator->Create(L"Boss2_Goddness_Die", Image17, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 16, Vector2(269.f, 276.f));
		mAnimator->Create(L"Boss2_Goddness_Die", Image18, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 16, Vector2(269.f, 276.f));

		mAnimator->Create(L"Boss2_Goddness_EnergyballStart", Image19, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 10, Vector2(269.f, 276.f));
		mAnimator->Create(L"Boss2_Goddness_Energyballing", Image20, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 6, Vector2(269.f, 276.f));
		mAnimator->Create(L"Boss2_Goddness_EnergyballEnd", Image21, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 3, Vector2(269.f, 276.f));

		mAnimator->Create(L"Boss2_Goddness_Die", Image22, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 16, Vector2(269.f, 276.f));
		mAnimator->Create(L"Boss2_Goddness_Die", Image23, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 16, Vector2(269.f, 276.f));
		mAnimator->Create(L"Boss2_Goddness_Die", Image24, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 16, Vector2(269.f, 276.f));
		mAnimator->Create(L"Boss2_Goddness_Die", Image25, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 16, Vector2(269.f, 276.f));
		mAnimator->Create(L"Boss2_Goddness_SummonSpear", Image26, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 17, Vector2(269.f, 276.f), Vector2::Zero, 0.08f);
		mAnimator->Create(L"Boss2_Goddness_Die", Image27, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 16, Vector2(269.f, 276.f));












		mAnimator->PlayAnimation(L"Boss_Goddness_Idle", true); // trigger �ϼ��ϸ� ����� 
		// ======
	// �ִϸ��̼� ���⿡ ���� ����	�� ����ش�.


		mDir = -mTransform->Right();



		// ������ �ʱ� ���´� �ο������� �ʴ´�. (������) 


		// ===== �⺻ �浹ü Hit �޴� �뵵 
		mCollider->SetName(L"Goddness_HitCol");
		mCollider->SetType(eColliderType::Rect);
		mCollider->SetSize(Vector2(0.6f, 0.36f));
		mCollider->SetCenter(Vector2(-4.f, -84.f));

		// ===== �ʱ� ���°� (�ӽ÷� �ο��� tirrger �����ϸ� �̰� ���ֱ�) 
		//mBossType = eBossType::GODDNESS;
		//mCurBoss2_Phase2_State = eBoss2_Phase2::IDLE;


		mCurBoss2_Phase1_State = eBoss2_Phase1::IDLE;
		mBossType = eBossType::STATUE;



		////==== ���� ���� Ư�� �ε��� �浹ü 
		////�浹ü�� ���⼭ �ٷ� ���� ���� �ش� �ε��� �� �־��ٰ� ���� ������ �ϱ� 
		//mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"BigWolf_AttackColObj");
		//mAttackColliderObj->Initialize();

		//mAttackColTr = mAttackColliderObj->GetComponent<Transform>();


		// trigger �ϼ��ϸ� �̰� ����� 
		//mMeshRenderer->SetMaterial(Resources::Find<Material>(L"tempMtrl"));




	}
	void GoddnessScript::Update()
	{

		MonsterScript::Update();


		// �̵�->���º�ȯ->�ִϸ��̼�


		Vector3 cameraOffset = Vector3(0, 0, 0);  // �⺻ ������ ��

		Transform* Target_tr = mPlayer->GetComponent<Transform>();
		Vector3 TargetPos = Target_tr->GetPosition();

		//// �÷��̾��� ��ġ�� ���� ������ ����
		//if (TargetPos.y > 25)
		//{
		//	cameraOffset.y = -50;  // ��: y�� �������� -50��ŭ ������ ����

		//	Vector3 MainCameraPos = mMainCamera->GetComponent<Transform>()->GetPosition();

		//	Vector3 newCameraPos = Vector3(TargetPos.x + cameraOffset.x, TargetPos.y + cameraOffset.y, MainCameraPos.z);

		//	mMainCamera->GetComponent<Transform>()->SetPosition(newCameraPos);
		//}




		if (mBossType == eBossType::STATUE)
		{
			switch (mCurBoss2_Phase1_State)
			{
			case ss::eBoss2_Phase1::INTRO:
				Intro();
				break;

			case ss::eBoss2_Phase1::IDLE:
				Idle();
				break;

			case ss::eBoss2_Phase1::STOMP_READY:
				Stomp_Ready();
				break;

			case ss::eBoss2_Phase1::STOMP_ING:
				Stomp_Ing();
				break;

			case ss::eBoss2_Phase1::STOMP_END:
				Stomp_End();
				break;

			case ss::eBoss2_Phase1::ENERGYBALL_READY:
				Energyball_Start();
				break;

			case ss::eBoss2_Phase1::ENERGYBALL_ING:
				Energyball_ing();
				break;

			case ss::eBoss2_Phase1::ENERGYBALL_END:
				Energyball_End();
				break;

			case ss::eBoss2_Phase1::WIND:
				break;

			case ss::eBoss2_Phase1::DIE:
				Dead();
				break;

			}

		}

		else
		{
			switch (mCurBoss2_Phase2_State)
			{
			case ss::eBoss2_Phase2::INTRO:
				Intro();
				break;

			case ss::eBoss2_Phase2::INTRO_END:
				IntroEnd();
				break;

			case ss::eBoss2_Phase2::IDLE:
				Idle();
				break;

			case ss::eBoss2_Phase2::MOVE_FRONT:
				MoveFront();
				break;

			case ss::eBoss2_Phase2::MOVE_BACK:
				MoveBack();
				break;

			case ss::eBoss2_Phase2::DIAGONAL_ATTACK_DIAGONAL:
				break;

			case ss::eBoss2_Phase2::DIAGONAL_ATTACK_VERTICAL:
				break;

			case ss::eBoss2_Phase2::ENERGYBALL_START:
				Energyball_Start();
				break;

			case ss::eBoss2_Phase2::ENERGYBALL_ING:
				Energyball_ing();
				break;

			case ss::eBoss2_Phase2::ENERGYBALL_END:
				Energyball_End();
				break;

			case ss::eBoss2_Phase2::COUNTER_START:
				break;

			case ss::eBoss2_Phase2::COUNTER_ING:
				break;

			case ss::eBoss2_Phase2::COUNTER_END:
				break;

			case ss::eBoss2_Phase2::COUNTER_HIT:
				break;

			case ss::eBoss2_Phase2::GROUNDSPEAR:
				break;

			case ss::eBoss2_Phase2::SHIELDBEAM_START:
				break;

			case ss::eBoss2_Phase2::SHIELDBEAM_ING:
				break;

			case ss::eBoss2_Phase2::SHIELDBEAM_END:
				break;

			case ss::eBoss2_Phase2::SUMMONSPEAR_START:
				SummonSpear_Start();
				break;

			case ss::eBoss2_Phase2::SUMMONSPEAR_ING:
				SummonSpear_Ing();
				break;

			case ss::eBoss2_Phase2::SUMMONSPEAR_END:
				SummonSpear_End();
				break;

			case ss::eBoss2_Phase2::HIT:
				break;

			case ss::eBoss2_Phase2::DIE:
				break;

			}


		}

		mPrevState = mCurState;
		mPrevDir = mCurDir;

	}

	void GoddnessScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void GoddnessScript::OnCollisionStay(Collider2D* other)
	{
	}
	void GoddnessScript::OnCollisionExit(Collider2D* other)
	{
	}



	void GoddnessScript::Intro()
	{

		if (mBossType == eBossType::GODDNESS)
		{

			//// == �ʱⰪ ��ġ���� y���� ��� ���� ���� �ø���. 
			Vector3 StatuePos = mTransform->GetPosition();

			float moveSpeed = 100.0f;  // ���ϴ� �ӵ� ���� �����ϼ���.

			float moveAmountY = moveSpeed * Time::DeltaTime();  // �����Ӵ� ������ ���� ����մϴ�.


			// �������� ��ġ ���� 
			if (StatuePos.y > -45.f)
			{

				StatuePos.y -= moveAmountY;
				mTransform->SetPosition(StatuePos);


			}




			m_fTime += Time::DeltaTime();

			if (m_fTime > 3.f)
			{

				ChangeState(eBoss2_Phase2::INTRO_END);

				m_fTime = 0.0f;
			}
		}


	}

	void GoddnessScript::IntroEnd()
	{
		mMainCamera->GetComponent<Camera>()->SetTargetSize(3.8f); // 4.3
		mMainCamera->GetComponent<CameraScript>()->SetTarget(this->GetOwner());

		Vector3 CameraPos = mMainCamera->GetComponent<Transform>()->GetPosition();
		mMainCamera->GetComponent<Transform>()->SetPosition(CameraPos.x, CameraPos.y + 0.45f, CameraPos.z);


		// ===============
		// ���͹ڽ��� �����.

		if (!mLetterBox)
		{
			mLetterBox = true;

			mLetterBoxUP = object::Instantiate<Background>(eLayerType::Grid, L"letterbox_UP");
			mLetterBoxUP->Initialize();

			Transform* tr = mLetterBoxUP->GetComponent<Transform>();
			tr->SetPosition(Vector3(0.f, -160.f, 100.f));
			tr->SetScale(Vector3(1600.f, 896.f, 1.f));

			MeshRenderer* mr = mLetterBoxUP->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"LetterBoxUpMtrl"));



			mLetterBoxBottom = object::Instantiate<Background>(eLayerType::Grid, L"letterbox_Bottom");
			mLetterBoxBottom->Initialize();

			Transform* TR = mLetterBoxBottom->GetComponent<Transform>();
			TR->SetPosition(Vector3(0.f, -560.f, 100.f));
			TR->SetScale(Vector3(1600.f, 896.f, 1.f));

			MeshRenderer* MR = mLetterBoxBottom->GetComponent<MeshRenderer>();
			MR->SetMaterial(Resources::Find<Material>(L"LetterBoxUpMtrl"));


			// ���͹ڽ��� ��� �۱͸� �����.

			mBossName = object::Instantiate<Background>(eLayerType::Grid, L"Boss2Name");
			mBossName->Initialize();

			Transform* nameTR = mBossName->GetComponent<Transform>();
			nameTR->SetPosition(Vector3(0.f, -128.f, 100.f));
			nameTR->SetScale(Vector3(40.f, 26.f, 1.f));

			MeshRenderer* nameMR = mBossName->GetComponent<MeshRenderer>();
			nameMR->SetMaterial(Resources::Find<Material>(L"Boss2_2NameMtrl"));
		}







		// ========

		mAnimator->PlayAnimation(L"Boss2_Goddness_Intro", false);




		// �ִϸ��̼� ����� ������ 
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{

			mMainCamera->GetComponent<Camera>()->SetTargetSize(2.3f);
			mMainCamera->GetComponent<CameraScript>()->SetTarget(mPlayer);
			mMainCamera->GetComponent<Transform>()->SetPosition(CameraPos.x, CameraPos.y, CameraPos.z);
			mMainCamera->GetComponent<Camera>()->TurnLayerMask(eLayerType::Grid, false);

			mUICamera->GetComponent<Camera>()->TurnLayerMask(eLayerType::UI, true);

			mLetterBoxBottom->SetState(GameObject::eState::Dead);
			mLetterBoxUP->SetState(GameObject::eState::Dead);
			mBossName->SetState(GameObject::eState::Dead);

			ChangeState(eBoss2_Phase2::IDLE);
			mLetterBox = false;





		}





	}

	void GoddnessScript::Idle()
	{

		if (mBossType == eBossType::STATUE)
		{
			// �������� ���¸� �����Ѵ�. 
			int randomValue = rand() % 2; // 0 �Ǵ� 1�� ���� (��������) 


			// ���� ���ڰ� stomp Ready�� �ش��ϴ°Ŷ�� ���¸� �ٲٱ� ���� �Ʒ��� ���� ������ �߰����ش�.
			// ī�޶� �÷��̾� ������ ���� �� Ȯ��ȴ�. (stomp ���� �Ͻ�) 
			m_fTime += Time::DeltaTime();


			// ���� ���� 


			if (m_fTime > 3.0f)
			{
				if (randomValue == 0)
				{

					mAnimator->PlayAnimation(L"Boss_Goddness_Stomp", false);



					mStompState = GetRandomStompState();
					ChangeState(eBoss2_Phase1::STOMP_READY);


					m_fTime = 0.0f;




				}

				// �������� ���� 
				else if (randomValue == 1)
				{
					ChangeState(eBoss2_Phase1::ENERGYBALL_READY);

					m_fTime = 0.0f;



				}
			}





			// ���࿡ �������� ���·� �Ѿ�ٸ� 
			// ���� ���°� stomp ���¿��ٸ� �ϴ� ����� ��ġ�� �ű��.
			// �װ� �ƴ϶�� ���� ��ġ�ߴ� ù ��ġ���� ���� ���ݸ� ��ġ�� �ø���.
			// �� ������ ���¸� ��ȯ�Ѵ�. 


		}

		else if (mBossType == eBossType::GODDNESS)
		{

			mAnimator->PlayAnimation(L"Boss2_Goddness_IntroEnd", true);

			Vector3 BossPos = mTransform->GetPosition(); // 2, -45, 500 (���� �߰� ��ġ) 

			Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();

			// �÷��̾� ��ġ 
			// (DEAD - IDLE�� ���� ��ġ) 
			// LT => -146. // 35.f // 400 
			// LB => -146.f // -86.f // 400


			if (!mLetterBox)
			{
				mLetterBox = true;

				//mPlatformLB = object::Instantiate<Background>(Vector3(BossPos.x - 148.f, BossPos.y -40.f, BossPos.z), eLayerType::Collision, L"PlatformLBObj");
				//mPlatformLT = object::Instantiate<Background>(Vector3(BossPos.x - 148.f, BossPos.y + 90.f, BossPos.z), eLayerType::Collision, L"PlatformLTObj");
				//mPlatformRT = object::Instantiate<Background>(Vector3(BossPos.x + 145.f, BossPos.y + 90.f, BossPos.z), eLayerType::Collision, L"PlatformRTObj");
				//mPlatformRB = object::Instantiate<Background>(Vector3(BossPos.x + 145.f, BossPos.y - 40.f, BossPos.z), eLayerType::Collision, L"PlatformRBObj");

				//mPlatformLT->Initialize();
				//mPlatformLB->Initialize();
				//mPlatformRT->Initialize();
				//mPlatformRB->Initialize();


				//// ==== �ٴ��� �� �浹ü 
				//mGroundLT = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
				//mGroundLT->Initialize();

				//Transform* tr = mGroundLT->GetComponent<Transform>();
				//tr->SetPosition(Vector3(BossPos.x - 148.f, BossPos.y - 33.f, BossPos.z));
				//tr->SetScale(Vector3(70.f, 2.f, 1.f));


				//mGroundLB = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
				//mGroundLB->Initialize();

				//Transform* tr2 = mGroundLB->GetComponent<Transform>();
				//tr2->SetPosition(Vector3(BossPos.x - 148.f, BossPos.y + 97.f, BossPos.z));
				//tr2->SetScale(Vector3(70.f, 2.f, 1.f));


				//mGroundRT = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
				//mGroundRT->Initialize();

				//Transform* tr3 = mGroundRT->GetComponent<Transform>();
				//tr3->SetPosition(Vector3(BossPos.x + 145.f, BossPos.y + 97.f, BossPos.z));
				//tr3->SetScale(Vector3(70.f, 2.f, 1.f));


				//mGroundRB = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
				//mGroundRB->Initialize();

				//Transform* tr4 = mGroundRB->GetComponent<Transform>();
				//tr4->SetPosition(Vector3(BossPos.x + 145.f, BossPos.y - 33.f, BossPos.z));
				//tr4->SetScale(Vector3(70.f, 2.f, 1.f));



				// =================
				// ���� ��� ���� �ϼ��Ǹ� �Ʒ��ɷ� ������Ű�� (dead - idle�� �Ѿ�� ���� ��ġ�� �۾��Ϸ��� idle���� ������������ ��ġ�� �ٸ�) 

				mPlatformLB = object::Instantiate<Background>(Vector3(BossPos.x - 145.f, BossPos.y - 87.f, BossPos.z), eLayerType::Collision, L"PlatformLBObj");
				mPlatformLT = object::Instantiate<Background>(Vector3(BossPos.x - 145.f, BossPos.y + 25.f, BossPos.z), eLayerType::Collision, L"PlatformLTObj");
				mPlatformRT = object::Instantiate<Background>(Vector3(BossPos.x + 140.f, BossPos.y + 25.f, BossPos.z), eLayerType::Collision, L"PlatformRTObj");
				mPlatformRB = object::Instantiate<Background>(Vector3(BossPos.x + 140.f, BossPos.y - 87.f, BossPos.z), eLayerType::Collision, L"PlatformRBObj");
				mPlatformMidle = object::Instantiate<Background>(Vector3(BossPos.x, BossPos.y - 28.f, BossPos.z), eLayerType::Collision, L"PlatformRBObj");


				mPlatformLT->Initialize();
				mPlatformLB->Initialize();
				mPlatformRT->Initialize();
				mPlatformRB->Initialize();
				mPlatformMidle->Initialize();


				// ==== �ٴ��� �� �浹ü 
				mGroundLT = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
				mGroundLT->Initialize();
				Transform* tr = mGroundLT->GetComponent<Transform>();
				tr->SetPosition(Vector3(BossPos.x - 145.f, BossPos.y + 32.f, BossPos.z));
				tr->SetScale(Vector3(77.f, 2.f, 1.f));

				mGroundLB = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
				mGroundLB->Initialize();
				Transform* tr2 = mGroundLB->GetComponent<Transform>();
				tr2->SetPosition(Vector3(BossPos.x - 145.f, BossPos.y - 80.f, BossPos.z));
				tr2->SetScale(Vector3(77.f, 2.f, 1.f));

				mGroundRT = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
				mGroundRT->Initialize();
				Transform* tr3 = mGroundRT->GetComponent<Transform>();
				tr3->SetPosition(Vector3(BossPos.x + 140.f, BossPos.y + 32.f, BossPos.z));
				tr3->SetScale(Vector3(77.f, 2.f, 1.f));

				mGroundRB = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
				mGroundRB->Initialize();
				Transform* tr4 = mGroundRB->GetComponent<Transform>();
				tr4->SetPosition(Vector3(BossPos.x + 140.f, BossPos.y - 80.f, BossPos.z));
				tr4->SetScale(Vector3(77.f, 2.f, 1.f));

				mGroundMidle = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
				mGroundMidle->Initialize();
				Transform* tr5 = mGroundMidle->GetComponent<Transform>();
				tr5->SetPosition(Vector3(BossPos.x, BossPos.y - 21.f, BossPos.z));
				tr5->SetScale(Vector3(77.f, 2.f, 1.f));







			}



			m_fTime += Time::DeltaTime();

			if (m_fTime > 2.f)
			{
				ChangeState(eBoss2_Phase2::MOVE_BACK);

				m_fTime = 0.0f;
			}

		}



	}




	void GoddnessScript::Hit()
	{
	}
	void GoddnessScript::Stomp_Ready()
	{
		// Ȯ��ƴ� ī�޶� stomp �ִϸ��̼� ù �ε����� �ٽ� ���� �������� 2.3������ ���ƿ´�. 

		// ��� �ִϸ��̼� ����� �����. (0�ε����� �ӹ�����), �� ���� ���� ���ð� ���´�.


		// ���� ���ð� �� ������ �÷��̾��� ��ġ ���� ���ִ´� (ü���� 2�� ����?)
		bool isGround = mPlayer->GetComponent<Rigidbody2D>()->IsGround();

		if (!mbStomp && isGround)
		{
			// hit ground�� ����. 
			// hit ground ������ �̵��Ѵ�.

			Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();


			// hit ground�� ����.
			mHitGround = object::Instantiate<Effect>(Vector3(PlayerPos.x, PlayerPos.y - 20.f, PlayerPos.z), eLayerType::Collision, L"StompHitGroundObj");

			HitGroundScript* script = mHitGround->AddComponent<HitGroundScript>();
			script->SetMonsterOwner((Monster*)mTransform->GetOwner());

			mbStomp = true;


		}


		if (nullptr != mHitGround)
		{


			// **** �����ϸ� tagetPos�� �� �������� ���� ����


			// ������ �밢������ �̵��Ѵ�. 
			Vector3 StatuePos = mTransform->GetPosition();
			Vector3 HitGroundPos = mHitGround->GetComponent<Transform>()->GetPosition();


			Vector3 TargetPos = Vector3(HitGroundPos.x, HitGroundPos.y + 250.f, HitGroundPos.z);


			// ���󿡼� ��ǥ ��ġ���� ���� ���͸� ����մϴ�.
			Vector3 dir = TargetPos - StatuePos;
			float distance = dir.Length();  // ����� ��ǥ ��ġ ������ �Ÿ��� ����մϴ�.
			dir.Normalize();  // ���� ���͸� ����ȭ�մϴ�.


			float moveSpeed = 700.0f;  // ���ϴ� �ӵ� ���� �����ϼ���.

			Vector3 moveAmount = dir * moveSpeed * Time::DeltaTime();  // �����Ӵ� ������ ���� ����մϴ�.

			StatuePos += moveAmount;  // ���� ��ġ�� ������Ʈ�մϴ�.
			mTransform->SetPosition(StatuePos);  // ������Ʈ�� ��ġ�� �����մϴ�.

			m_fTime += Time::DeltaTime();

			if (m_fTime > 0.5f) // 1.5���� ���̸� ��Ȯ�� ��ġ�� ���� ���� �������� 
			{
				ChangeState(eBoss2_Phase1::STOMP_ING);
				m_fTime = 0.0f;
				mbStomp = false;

			}

		}








		// 2�ʰ� ������ ���޾� 3�� �� ��ġ�� ��������� ��������. 

		// 3�� Ƚ�� ���� �ٴڿ� �پ��ִ� ���·� �����Ѵ�. �� �� �Ŀ�, �÷��̾��� ��ġ�� �ٽ� üũ�Ѵ�.

		//�� �ʰ� ������ ���޾� 3�� �� ��´�.

		// �� ������ 4�� ���ӵǸ� Idle ���·� ���ư���.









	}
	void GoddnessScript::Stomp_Ing()
	{


		float StatuePosY = mTransform->GetPosition().y;
		float HitGroundPosY = mHitGround->GetComponent<Transform>()->GetPosition().y;
		float TargetPosY = 0.0f;

		// ���¿� ���� ��ǥ ��ġ ����
		switch (mStatueState)
		{
		case eStatueState::MOVING_DOWN:
			TargetPosY = HitGroundPosY + 136.f;
			mRigidbody->AddForce(Vector2(0.f, 150.f));
			break;

		case eStatueState::MOVING_UP:
			TargetPosY = HitGroundPosY + 250.f;
			mRigidbody->AddForce(Vector2(0.f, -150.f));
			break;
		}

		float dirY = TargetPosY - StatuePosY;

		float moveDistance = abs(dirY);
		dirY = dirY > 0 ? 1.0f : -1.0f;

		float moveSpeed = 700.0f;
		float moveAmountY = dirY * moveSpeed * Time::DeltaTime();

		StatuePosY += moveAmountY;


		mTransform->SetPosition(Vector3(mTransform->GetPosition().x, StatuePosY, mTransform->GetPosition().z));


		// ���� ��ȯ ����
		if ((dirY > 0 && StatuePosY >= TargetPosY) || (dirY < 0 && StatuePosY <= TargetPosY))
		{
			if (mStompState == eStompState::QUADRUPLE_STOMP)
			{

				if (mStatueState == eStatueState::MOVING_DOWN)
				{
					mStatueState = eStatueState::MOVING_UP;



					if (miStompCount == 3)
						return;

					miStompCount++;
				}


				else
				{
					mStatueState = eStatueState::MOVING_DOWN;

				}


				if (miStompCount == 3)
				{
					ChangeState(eBoss2_Phase1::STOMP_END);

				}

				else
				{
					ChangeState(eBoss2_Phase1::STOMP_ING);
				}
			}


			else if (mStompState == eStompState::SINGLE_STOMP)
			{

				ChangeState(eBoss2_Phase1::STOMP_END);

				if (!mbFirstStomp)
				{

					mbFirstStomp = true;
				}

				else
				{
					mStatueState = eStatueState::MOVING_DOWN; // 1���� ��� ������ �� ���°� ��� �Ʒ� ���¿����ϹǷ�.
				}


			}



		}








	}



	void GoddnessScript::Stomp_End()
	{
		// ī�޶� �� �� Ȯ����Ѽ� �� Ÿ������ �˷��ش�. 
		mAnimator->PlayAnimation(L"Boss_Goddness_Idle", true);


		miStompCount = 0; // 0�� ���� �ʱ�ȭ���� �� �����غ��� 


		m_fTime += Time::DeltaTime();


		if (mStompState == eStompState::QUADRUPLE_STOMP)
		{

			// ���°� �ٲ� �� ī�޶� �ٽ� ������� �������´�. 

			if (miCompleteStompCount < 3)
			{
				mAnimator->PlayAnimation(L"Boss_Goddness_Stomp", false);

				if (m_fTime > 2.0f)
				{
					ChangeState(eBoss2_Phase1::STOMP_READY);
					++miCompleteStompCount;
					m_fTime = 0.f;
				}


			}

			else if (miCompleteStompCount == 3)
			{
				if (m_fTime > 2.0f)
				{



					ChangeState(eBoss2_Phase1::IDLE);
					mPrevBoss2_Phase1_State = eBoss2_Phase1::STOMP_END;
					miCompleteStompCount = 0; // �ʱ�ȭ 
					m_fTime = 0.0f;

				}


			}







		}

		else if (mStompState == eStompState::SINGLE_STOMP)
		{
			// ���°� �ٲ� �� ī�޶� �ٽ� ������� �������´�. 

			if (m_fTime > 0.2f)
			{
				if (miCompleteStompCount < 4)
				{
					mAnimator->PlayAnimation(L"Boss_Goddness_Stomp", false);
					ChangeState(eBoss2_Phase1::STOMP_READY);
					++miCompleteStompCount;
					m_fTime = 0.0f;


				}

				else if (miCompleteStompCount == 4)
				{

					if (m_fTime > 2.0f)
					{

						ChangeState(eBoss2_Phase1::IDLE);
						mPrevBoss2_Phase1_State = eBoss2_Phase1::STOMP_END;
						miCompleteStompCount = 0; // �ʱ�ȭ 
						m_fTime = 0.0f;
						mbFirstStomp = false;


					}
				}


			}






		}

	}

	void GoddnessScript::Energyball_Start()
	{

		if (mBossType == eBossType::STATUE)
		{
			if (mPrevBoss2_Phase1_State == eBoss2_Phase1::STOMP_END)
			{
				// ������ �߾����� �̵��Ѵ�. 
				Vector3 StatuePos = mTransform->GetPosition();
				Vector3 FirstPos = Vector3(0.f, -87.f, 500.f);


				Vector3 TargetPos = Vector3(0.f, FirstPos.y + 67.f, FirstPos.z);


				// ���󿡼� ��ǥ ��ġ���� ���� ���͸� ����մϴ�.
				Vector3 dir = TargetPos - StatuePos;
				float distance = dir.Length();  // ����� ��ǥ ��ġ ������ �Ÿ��� ����մϴ�.
				dir.Normalize();  // ���� ���͸� ����ȭ�մϴ�.


				float moveSpeed = 200.0f;  // ���ϴ� �ӵ� ���� �����ϼ���.

				Vector3 moveAmount = dir * moveSpeed * Time::DeltaTime();  // �����Ӵ� ������ ���� ����մϴ�.

				StatuePos += moveAmount;  // ���� ��ġ�� ������Ʈ�մϴ�.
				mTransform->SetPosition(StatuePos);  // ������Ʈ�� ��ġ�� �����մϴ�.
			}


			else
			{
				// == �ʱⰪ ��ġ���� y���� ��� ���� ���� �ø���. 
				Vector3 StatuePos = mTransform->GetPosition();

				float moveSpeed = 200.0f;  // ���ϴ� �ӵ� ���� �����ϼ���.

				float moveAmountY = moveSpeed * Time::DeltaTime();  // �����Ӵ� ������ ���� ����մϴ�.


				// �ö󰡴� ��ġ ���� 
				if (StatuePos.y < -21.f)
				{

					StatuePos.y += moveAmountY;
					mTransform->SetPosition(StatuePos);


				}


			}


			m_fTime += Time::DeltaTime();

			if (m_fTime > 2.f)
			{
				ChangeState(eBoss2_Phase1::ENERGYBALL_ING);
				m_fTime = 0.0f;
			}

		}

		else if (mBossType == eBossType::GODDNESS)
		{
			mAnimator->PlayAnimation(L"Boss2_Goddness_EnergyballStart", false);

			// ���������� ��ȯ�Ѵ�. 

			Vector3 BossPos = mTransform->GetPosition();

			if (!mbEnergySpawn)
			{
				
				mbEnergySpawn = true;

				// ==== ���� ��������
				mSmallEngeryball = object::Instantiate<SmallEnergyball>(Vector3(BossPos.x + 82.f, BossPos.y + 9.3f, 400.f),eLayerType::Collision, L"S_EnergyballObj");
				mSmallEngeryball->Initialize();

				mSmallEngeryball->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(L"tempMtrl"));
				mSmallEngeryball->RemoveComponent<Collider2D>();
			

				// ===== ū ��������
				
				mBigEngeryball = object::Instantiate<BigEnergyball>(Vector3(BossPos.x + 82.f, BossPos.y + 9.3f, 400.f), eLayerType::Collision, L"��_EnergyballObj");
				mBigEngeryball->Initialize();

				mBigEngeryball->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(L"tempMtrl"));
				mBigEngeryball->RemoveComponent<Collider2D>();



			}



		}





	}


	void GoddnessScript::Energyball_ing()
	{


		if (mBossType == eBossType::STATUE)
		{
			Vector3 StatuePos = mTransform->GetPosition();

			if (!mbEnergySpawn)
			{
				mbEnergySpawn = true;

				mEngeryball = object::Instantiate<Energyball>(Vector3(StatuePos.x - 60.f, StatuePos.y + 20.f, 400.f), eLayerType::Collision, L"Parrying_S_EnergyballObj");
				mEngeryball->SetFirstEnergyball(mEngeryball);
				mEngeryball->SetOriginOwenr(mTransform->GetOwner());

				mEngeryball->IncreaseSpawnCount();

				SceneManager::SetPlayer(mPlayer);



			}


			m_fTime += Time::DeltaTime();

			if (mEngeryball->GetSpawnCount() == 12 && m_fTime > 13.5f)
			{
				ChangeState(eBoss2_Phase1::ENERGYBALL_END);
				mbEnergySpawn = false;
				mEngeryball->InitializeSpawnCount();
				m_fTime = 0.0f;
			}
		}

		else if (mBossType == eBossType::GODDNESS)
		{

		}




	}
	void GoddnessScript::Energyball_End()
	{

		if (mBossType == eBossType::STATUE)
		{
			Vector3 targetPos = Vector3(0.f, -87.f, 500.f);
			Vector3 StatuePos = mTransform->GetPosition();

			float moveSpeed = 150.0f;  // ���ϴ� �ӵ� ���� �����ϼ���.

			float moveAmountY = moveSpeed * Time::DeltaTime();  // �����Ӵ� ������ ���� ����մϴ�.

			// �������� ��ġ ����
			if (StatuePos.y > targetPos.y)
			{
				StatuePos.y -= moveAmountY;
				// ���� moveAmountY�� ���� �� ���� ���� targetPos.y���� �۴ٸ� targetPos.y�� ����
				if (StatuePos.y < targetPos.y)
				{
					StatuePos.y = targetPos.y;
				}
				mTransform->SetPosition(StatuePos);


			}


			m_fTime += Time::DeltaTime();

			if (m_fTime > 1.5f)
			{
				ChangeState(eBoss2_Phase1::IDLE);
				m_fTime = 0.f;
			}
		}

		else if (mBossType == eBossType::GODDNESS)
		{

		}



	}
	void GoddnessScript::Wind_Ready()
	{
	}

	void GoddnessScript::Dead()
	{
		mUICamera->GetComponent<Camera>()->TurnLayerMask(eLayerType::UI, false);

		SetHit(false); // dead ���� �������� �� �ǰݴ��ص� ������ ������ �ʵ��� �Ѵ�. 
		GetOwner()->RemoveComponent<Collider2D>(); // dead ���¿��� ��� ���������ű� ������ Ȥ�ø� �Ͽ� ����� ������Ʈ�� ���д�. 

		mMainCamera->GetComponent<Camera>()->SetTargetSize(5.3f);
		mMainCamera->GetComponent<CameraScript>()->SetTarget(this->GetOwner());


		//if (mHitGround->GetState() != GameObject::eState::Dead)
		//{
		//	mHitGround->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(L"tempMtrl"));
		//}


		Vector3 CameraPos = mMainCamera->GetComponent<Transform>()->GetPosition();
		mMainCamera->GetComponent<Transform>()->SetPosition(CameraPos.x, CameraPos.y + 0.2f, CameraPos.z);

		if (mAnimator->GetCurActiveAnimation()->GetIndex() == 8)
		{

			mAnimator->GetCurActiveAnimation()->SetCurSpriteDuration(0.01f);
		}

		else if (mAnimator->GetCurActiveAnimation()->GetIndex() == 9
			|| mAnimator->GetCurActiveAnimation()->GetIndex() == 10
			|| mAnimator->GetCurActiveAnimation()->GetIndex() == 11
			|| mAnimator->GetCurActiveAnimation()->GetIndex() == 12
			|| mAnimator->GetCurActiveAnimation()->GetIndex() == 13
			|| mAnimator->GetCurActiveAnimation()->GetIndex() == 14
			|| mAnimator->GetCurActiveAnimation()->GetIndex() == 15)
		{
			mMainCamera->GetComponent<Camera>()->SetTargetSize(2.3f); // �� �ƿ��� �� �ε巴�� �ǵ��� �ص� 
		}

		mAnimator->PlayAnimation(L"Boss_Goddness_Die", false);




		// �ִϸ��̼� ����� ������ 
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			mMainCamera->GetComponent<CameraScript>()->SetTarget(mPlayer);

			//mAttackColliderObj->SetState(GameObject::eState::Dead);

			mMainCamera->GetComponent<Transform>()->SetPosition(CameraPos.x, CameraPos.y, CameraPos.z);

			GetOwner()->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(L"tempMtrl"));

			GetOwner()->GetComponent<Transform>()->SetPosition
			(Vector3(2.f, 200.f, mTransform->GetPosition().z)); // 300.f 	

			m_fTime += Time::DeltaTime();

			if (m_fTime > 3.5f)
			{
				mBossType = eBossType::GODDNESS;
				ChangeState(eBoss2_Phase1::INTRO);

				mAnimator->PlayAnimation(L"Boss2_Goddness_IntroEnd", true);

				GetOwner()->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(L"BossAnimMtrl"));

				GetOwner()->AddComponent<Collider2D>(); // dead ���¿��� ��� ���������ű� ������ Ȥ�ø� �Ͽ� ����� ������Ʈ�� ���д�. 
				mCharacterState->SetMaxHP(110.f);
				mCharacterState->SetCurrentHP(10.f);



				m_fTime = 0.0f;


			}


		}




	}

	void GoddnessScript::MoveBack()
	{
		mAnimator->PlayAnimation(L"Boss2_Goddness_MoveBackL", true);


		// �밢�� ������ ���Ѵ�.
		Vector3 DiagonalDirection = Vector3(1.0, 1.0, 500);
		DiagonalDirection.Normalize(); // �������� ������ �����ϵ� ũ�⸸ 1�� �صд�. 

		Vector3 BossPos = mTransform->GetPosition();

		if (mbMovingDiagonally)
		{

			float moveSpeed = 40000.0f;  // �������� �ӵ�


			BossPos += DiagonalDirection * moveSpeed * Time::DeltaTime();

			// �밢�� �������� ���� �Ÿ� �̻� �������ٸ�
			if (BossPos.y >= 80.f)
			{
				mbMovingDiagonally = false;
				mbMovingDown = true;
			}

			// ������ ��ġ�� ������ �����մϴ�.
			mTransform->SetPosition(BossPos);
		}


		// �밢�� �̵��� ������ �Ʒ��� ���� �̵��Ѵ�. 
		else if (mbMovingDown)
		{


			float moveSpeed = 100.0f;  // �������� �ӵ�


			BossPos.y -= moveSpeed * Time::DeltaTime();

			// �Ʒ��� �������� ���� �Ÿ� �̻� �������ٸ�
			if (mTransform->GetPosition().y <= -110.f)
			{
				mbMovingDown = false;

				mAnimator->PlayAnimation(L"Boss2_Goddness_SummonSpear", false);
				ChangeState(eBoss2_Phase2::SUMMONSPEAR_START);

			}

			mTransform->SetPosition(BossPos);

		}



	}

	void GoddnessScript::MoveFront()
	{
	}

	void GoddnessScript::SummonSpear_Start()
	{

		if (mSpawnDirCount < 5)
		{
			mAnimator->PlayAnimation(L"Boss2_Goddness_IdleR", true);
		}


		// Į 3���� �߻�ȴ�. (dead ó���� ���� �ε�ġ�� ������ �� �׋� �� ���̹Ƿ� �Ű� �Ƚᵵ ��) 




		m_fTime += Time::DeltaTime();



		if (!mbFreezingPos)
		{
			mbFreezingPos = true; // false�� ���� ��ȯ�� �� ���ֱ� 
			mPlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();

		}


		switch (mSummonState)
		{

		case eSummonState::SPAWN_DIRHIT:
			// dirhit ���� ����

			if (!mbDirHitSpawn)
			{

				float Changetime = mbFirstSpawnDone ? 0.0f : 1.4f;

				if (m_fTime > Changetime)
				{
					mbDirHitSpawn = true;


					CreateDirHit();
					mbFirstSpawnDone = true; // 2��° ���������ʹ� true������ �ð� �ݿ��� 
					m_fTime = 0.0f;

					if (mSpawnDirCount == 5)
					{
						mSummonState = eSummonState::MOVE_TO_CENTER;
					}

					else
					{
						mSummonState = eSummonState::SPAWN_SWORD;
					}

				}

			}// ���� ��ȯ
			break;
		
		case eSummonState::MOVE_TO_CENTER:
		{
			mAnimator->PlayAnimation(L"Boss2_Goddness_MoveFrontL", false);

			Vector3 BossPos = mTransform->GetPosition();
			Vector3 BossToCenterDir = Vector3(2.f, -45.f, 500.f) - BossPos;
			BossToCenterDir.Normalize();
			float speed = 250000.f;
			BossPos += BossToCenterDir * speed * Time::DeltaTime();
			mTransform->SetPosition(BossPos);

			// �߾ӿ� �����ߴ��� Ȯ��
			if (BossPos.x >= 2.4f && BossPos.x <= 2.6f)
			{
				mAnimator->PlayAnimation(L"Boss2_Goddness_CounterStart", false);

				// ���� ���·� ��ȯ
				mSummonState = mSummonState = eSummonState::SPAWN_SWORD;
			}

		}
			break;

		case eSummonState::SPAWN_SWORD:
			// Į ���� ����

			if (!mbSwordSpawn)
			{

				float Changetime = mbSecondSpawnDone ? 0.5f : 0.6f; // 1��������

				if (m_fTime > Changetime)
				{

					mbSwordSpawn = true;

					CreateSwordBullet();
					mbSecondSpawnDone = true; // 2��° ���ʹ� 0.7�� �ð� �ݿ��� 
					m_fTime = 0.0f;

					mSummonState = eSummonState::SPAWN_DIRHIT;
					mbDirHitSpawn = false;
					ChangeState(eBoss2_Phase2::SUMMONSPEAR_ING);

				}

			}
			break;


		}


	}

	void GoddnessScript::SummonSpear_Ing()
	{
		m_fTime += Time::DeltaTime();


		FireSwordBullet();
		mbSwordSpawn = false;



		if (mSpawnDirCount < 8 && m_fTime > 0.8f)
		{
	


			ChangeState(eBoss2_Phase2::SUMMONSPEAR_START);
			mbFreezingPos = false;
			m_fTime = 0.f;

			mbSummonFinish = true; // false�� ���� ���·� �Ѿ �� ���ֱ� 
		}

		else if (mSpawnDirCount == 8)
		{
			mSpawnDirCount = 0;
			ChangeState(eBoss2_Phase2::SUMMONSPEAR_END);
		}

	}

	void GoddnessScript::SummonSpear_End()
	{
		mAnimator->PlayAnimation(L"Boss2_Goddness_CounterEnd", false);
		ChangeState(eBoss2_Phase2::ENERGYBALL_START);
	}

	void GoddnessScript::CreateDirHit()
	{
		// ���ʿ��� ���������� �߻� 
		if (mPlayerPos.x >= 0.0)
		{
			miRandom = rand() % 2;

			if (miRandom == 0)
			{
				if (mPlayerPos.y >= -210 && mPlayerPos.y <= -100)
				{
					// mHitDir_Left mHitDir_Right
					mHitDir_Left = object::Instantiate<Effect>(eLayerType::Effect, L"HitDir_MidObj_LR");
					EffectScript* effectcript1 = mHitDir_Left->AddComponent<EffectScript>();
					effectcript1->SetOriginOwner((Monster*)mTransform->GetOwner());

					Vector3 BossPos = mTransform->GetPosition();
					mHitDir_Left->GetComponent<Transform>()->SetPosition(mPlayerPos.x, mPlayerPos.y, 400.f);

					//=========
					mHitDir_Mid = object::Instantiate<Effect>(eLayerType::Effect, L"HitDir_MidObj_LR");
					EffectScript* effectcript2 = mHitDir_Mid->AddComponent<EffectScript>();
					effectcript2->SetOriginOwner((Monster*)mTransform->GetOwner());
					mHitDir_Mid->GetComponent<Transform>()->SetPosition(mPlayerPos.x, mPlayerPos.y + 20.f, 400.f);

					// ====
					mHitDir_Right = object::Instantiate<Effect>(eLayerType::Effect, L"HitDir_MidObj_LR");
					EffectScript* effectcript3 = mHitDir_Right->AddComponent<EffectScript>();
					effectcript3->SetOriginOwner((Monster*)mTransform->GetOwner());
					mHitDir_Right->GetComponent<Transform>()->SetPosition(mPlayerPos.x, mPlayerPos.y + 40.f, 400.f);



				}


				else if (mPlayerPos.y >= -100)
				{
					// mHitDir_Left mHitDir_Right
					mHitDir_Mid = object::Instantiate<Effect>(eLayerType::Effect, L"HitDir_MidObj_LR");
					EffectScript* effectcript1 = mHitDir_Mid->AddComponent<EffectScript>();
					effectcript1->SetOriginOwner((Monster*)mTransform->GetOwner());

					Vector3 BossPos = mTransform->GetPosition();
					mHitDir_Mid->GetComponent<Transform>()->SetPosition(mPlayerPos.x, mPlayerPos.y, 400.f);

					//=========
					mHitDir_Left = object::Instantiate<Effect>(eLayerType::Effect, L"HitDir_MidObj_LR");
					EffectScript* effectcript2 = mHitDir_Left->AddComponent<EffectScript>();
					effectcript2->SetOriginOwner((Monster*)mTransform->GetOwner());
					mHitDir_Left->GetComponent<Transform>()->SetPosition(mPlayerPos.x, mPlayerPos.y - 20.f, 400.f);

					// ====
					mHitDir_Right = object::Instantiate<Effect>(eLayerType::Effect, L"HitDir_MidObj_LR");
					EffectScript* effectcript3 = mHitDir_Right->AddComponent<EffectScript>();
					effectcript3->SetOriginOwner((Monster*)mTransform->GetOwner());
					mHitDir_Right->GetComponent<Transform>()->SetPosition(mPlayerPos.x, mPlayerPos.y + 20.f, 400.f);
				}
			}

			else if (miRandom == 1)
			{
				// ���η� ���� (�Ʒ����� ����) ���� ���� �� ELSE IF �� ���� �ϳ��� ����, ELSE�� �ϳ��� ��� ���ư��� �ϱ� 
				if (mPlayerPos.x <= -308 || mPlayerPos.x <= 297)
				{
					mHitDir_Mid = object::Instantiate<Effect>(eLayerType::Effect, L"HitDir_MidObj_DU");
					EffectScript* effectcript1 = mHitDir_Mid->AddComponent<EffectScript>();
					effectcript1->SetOriginOwner((Monster*)mTransform->GetOwner());

					Vector3 BossPos = mTransform->GetPosition();
					mHitDir_Mid->GetComponent<Transform>()->SetPosition(mPlayerPos.x, mPlayerPos.y, 400.f);

					//	=========
					mHitDir_Left = object::Instantiate<Effect>(eLayerType::Effect, L"HitDir_MidObj_DU");
					EffectScript* effectcript2 = mHitDir_Left->AddComponent<EffectScript>();
					effectcript2->SetOriginOwner((Monster*)mTransform->GetOwner());
					mHitDir_Left->GetComponent<Transform>()->SetPosition(mPlayerPos.x - 20.f, mPlayerPos.y, 400.f);

					// ====
					mHitDir_Right = object::Instantiate<Effect>(eLayerType::Effect, L"HitDir_MidObj_DU");
					EffectScript* effectcript3 = mHitDir_Right->AddComponent<EffectScript>();
					effectcript3->SetOriginOwner((Monster*)mTransform->GetOwner());
					mHitDir_Right->GetComponent<Transform>()->SetPosition(mPlayerPos.x + 20.f, mPlayerPos.y, 400.f);

				}
			}

		}

		// �����ʿ��� �������� �߻� 
		else if (mPlayerPos.x <= 0.0)
		{
			miRandom = rand() % 2;

			if (miRandom == 0)
			{
				if (mPlayerPos.y >= -210 && mPlayerPos.y <= -100)
				{
					// mHitDir_Left mHitDir_Right
					mHitDir_Left = object::Instantiate<Effect>(eLayerType::Effect, L"HitDir_MidObj_RL");
					EffectScript* effectcript1 = mHitDir_Left->AddComponent<EffectScript>();
					effectcript1->SetOriginOwner((Monster*)mTransform->GetOwner());

					Vector3 BossPos = mTransform->GetPosition();
					mHitDir_Left->GetComponent<Transform>()->SetPosition(mPlayerPos.x, mPlayerPos.y, 400.f);

					//=========
					mHitDir_Mid = object::Instantiate<Effect>(eLayerType::Effect, L"HitDir_MidObj_RL");
					EffectScript* effectcript2 = mHitDir_Mid->AddComponent<EffectScript>();
					effectcript2->SetOriginOwner((Monster*)mTransform->GetOwner());
					mHitDir_Mid->GetComponent<Transform>()->SetPosition(mPlayerPos.x, mPlayerPos.y + 20.f, 400.f);

					// ====
					mHitDir_Right = object::Instantiate<Effect>(eLayerType::Effect, L"HitDir_MidObj_RL");
					EffectScript* effectcript3 = mHitDir_Right->AddComponent<EffectScript>();
					effectcript3->SetOriginOwner((Monster*)mTransform->GetOwner());
					mHitDir_Right->GetComponent<Transform>()->SetPosition(mPlayerPos.x, mPlayerPos.y + 40.f, 400.f);



				}


				else if (mPlayerPos.y >= -100)
				{
					// mHitDir_Left mHitDir_Right
					mHitDir_Mid = object::Instantiate<Effect>(eLayerType::Effect, L"HitDir_MidObj_RL");
					EffectScript* effectcript1 = mHitDir_Mid->AddComponent<EffectScript>();
					effectcript1->SetOriginOwner((Monster*)mTransform->GetOwner());

					Vector3 BossPos = mTransform->GetPosition();
					mHitDir_Mid->GetComponent<Transform>()->SetPosition(mPlayerPos.x, mPlayerPos.y, 400.f);

					//=========
					mHitDir_Left = object::Instantiate<Effect>(eLayerType::Effect, L"HitDir_MidObj_RL");
					EffectScript* effectcript2 = mHitDir_Left->AddComponent<EffectScript>();
					effectcript2->SetOriginOwner((Monster*)mTransform->GetOwner());
					mHitDir_Left->GetComponent<Transform>()->SetPosition(mPlayerPos.x, mPlayerPos.y - 20.f, 400.f);

					// ====
					mHitDir_Right = object::Instantiate<Effect>(eLayerType::Effect, L"HitDir_MidObj_RL");
					EffectScript* effectcript3 = mHitDir_Right->AddComponent<EffectScript>();
					effectcript3->SetOriginOwner((Monster*)mTransform->GetOwner());
					mHitDir_Right->GetComponent<Transform>()->SetPosition(mPlayerPos.x, mPlayerPos.y + 20.f, 400.f);
				}
			}


			else if (miRandom == 1)
			{
				// ���η� ���� (������ �Ʒ�) ���� ���� �� ELSE IF �� ���� �ϳ��� ����, ELSE�� �ϳ��� ��� ���ư��� �ϱ� 
				if (mPlayerPos.x <= -308 || mPlayerPos.x <= 297)
				{


					mHitDir_Mid = object::Instantiate<Effect>(eLayerType::Effect, L"HitDir_MidObj_UD");
					EffectScript* effectcript1 = mHitDir_Mid->AddComponent<EffectScript>();
					effectcript1->SetOriginOwner((Monster*)mTransform->GetOwner());

					Vector3 BossPos = mTransform->GetPosition();
					mHitDir_Mid->GetComponent<Transform>()->SetPosition(mPlayerPos.x, mPlayerPos.y, 400.f);

					//=========
					mHitDir_Left = object::Instantiate<Effect>(eLayerType::Effect, L"HitDir_MidObj_UD");
					EffectScript* effectcript2 = mHitDir_Left->AddComponent<EffectScript>();
					effectcript2->SetOriginOwner((Monster*)mTransform->GetOwner());
					mHitDir_Left->GetComponent<Transform>()->SetPosition(mPlayerPos.x - 20.f, mPlayerPos.y, 400.f);

					// ====
					mHitDir_Right = object::Instantiate<Effect>(eLayerType::Effect, L"HitDir_MidObj_UD");
					EffectScript* effectcript3 = mHitDir_Right->AddComponent<EffectScript>();
					effectcript3->SetOriginOwner((Monster*)mTransform->GetOwner());
					mHitDir_Right->GetComponent<Transform>()->SetPosition(mPlayerPos.x + 20.f, mPlayerPos.y, 400.f);


				}
			}
		}



		++mSpawnDirCount;

	}

	void GoddnessScript::CreateSwordBullet()
	{


		if (mSpawnDirCount == 8)
			return;

		// ���ʿ��� ���������� �߻� 
		if (mPlayerPos.x >= 0.0)
		{
			if (miRandom == 0)
			{
				if (mPlayerPos.y >= -210 && mPlayerPos.y <= -100)
				{

					mSwordBullet_Left = object::Instantiate<SwordBullet>
						(Vector3(mPlayerPos.x - 600.f, mPlayerPos.y, 400.f), eLayerType::Collision, L"Sword_LeftToLight");

					mSwordBullet_Mid = object::Instantiate<SwordBullet>
						(Vector3(mPlayerPos.x - 600.f, mPlayerPos.y + 20.f, 400.f), eLayerType::Collision, L"Sword_LeftToLight");


					// ====
					mSwordBullet_Right = object::Instantiate<SwordBullet>
						(Vector3(mPlayerPos.x - 600.f, mPlayerPos.y + 40.f, 400.f), eLayerType::Collision, L"Sword_LeftToLight");

				}

				else if (mPlayerPos.y >= -100)
				{

					mSwordBullet_Mid = object::Instantiate<SwordBullet> // 570
						(Vector3(mPlayerPos.x - 600.f, mPlayerPos.y, 400.f), eLayerType::Collision, L"Sword_LeftToLight");

					mSwordBullet_Left = object::Instantiate<SwordBullet>
						(Vector3(mPlayerPos.x - 600.f, mPlayerPos.y - 20.f, 400.f), eLayerType::Collision, L"Sword_LeftToLight");

					mSwordBullet_Right = object::Instantiate<SwordBullet>
						(Vector3(mPlayerPos.x - 600.f, mPlayerPos.y + 20.f, 400.f), eLayerType::Collision, L"Sword_LeftToLight");
				}
			}

			else if (miRandom == 1)
			{
				mSwordBullet_Mid = object::Instantiate<SwordBullet>
					(Vector3(mPlayerPos.x, mPlayerPos.y - 40.f, 400.f), eLayerType::Collision, L"Sword_DownToUp");

				mSwordBullet_Left = object::Instantiate<SwordBullet>
					(Vector3(mPlayerPos.x - 20.f, mPlayerPos.y - 40.f, 400.f), eLayerType::Collision, L"Sword_DownToUp");

				mSwordBullet_Right = object::Instantiate<SwordBullet>
					(Vector3(mPlayerPos.x + 20.f, mPlayerPos.y - 40.f, 400.f), eLayerType::Collision, L"Sword_DownToUp");
			}


		}

		// �����ʿ��� �������� �߻� 
		else if (mPlayerPos.x <= 0.0)
		{
			if (miRandom == 0)
			{
				if (mPlayerPos.y >= -210 && mPlayerPos.y <= -100)
				{
					mSwordBullet_Left = object::Instantiate<SwordBullet>
						(Vector3(mPlayerPos.x + 570.f, mPlayerPos.y, 400.f), eLayerType::Collision, L"Sword_RightToLeft");


					// ==== ȸ�� �׽�Ʈ 
					/*Vector3 rotation = mTransform->GetDegreeRot();
					rotation.z = 10.0f;
					SwordBullet_MidTr->SetRotation(rotation);*/

					// ======
					mSwordBullet_Mid = object::Instantiate<SwordBullet>
						(Vector3(mPlayerPos.x + 570.f, mPlayerPos.y + 20.f, 400.f), eLayerType::Collision, L"Sword_RightToLeft");


					// ====
					mSwordBullet_Right = object::Instantiate<SwordBullet>
						(Vector3(mPlayerPos.x + 570.f, mPlayerPos.y + 40.f, 400.f), eLayerType::Collision, L"Sword_RightToLeft");




					Transform* SwordBullet_MidTr = mSwordBullet_Left->GetComponent<Transform>();




				}


				else if (mPlayerPos.y >= -100)
				{

					mSwordBullet_Mid = object::Instantiate<SwordBullet>
						(Vector3(mPlayerPos.x + 570.f, mPlayerPos.y, 400.f), eLayerType::Collision, L"Sword_RightToLeft");

					mSwordBullet_Left = object::Instantiate<SwordBullet>
						(Vector3(mPlayerPos.x + 570.f, mPlayerPos.y - 20.f, 400.f), eLayerType::Collision, L"Sword_RightToLeft");

					mSwordBullet_Right = object::Instantiate<SwordBullet>
						(Vector3(mPlayerPos.x + 570.f, mPlayerPos.y + 20.f, 400.f), eLayerType::Collision, L"Sword_RightToLeft");
				}

			}


			else if (miRandom == 1)
			{


				mSwordBullet_Mid = object::Instantiate<SwordBullet> // 570
					(Vector3(mPlayerPos.x, mPlayerPos.y + 450.f, 400.f), eLayerType::Collision, L"Sword_UpToDown");

				mSwordBullet_Left = object::Instantiate<SwordBullet>
					(Vector3(mPlayerPos.x - 20.f, mPlayerPos.y + 450.f, 400.f), eLayerType::Collision, L"Sword_UpToDown");

				mSwordBullet_Right = object::Instantiate<SwordBullet>
					(Vector3(mPlayerPos.x + 20.f, mPlayerPos.y + 450.f, 400.f), eLayerType::Collision, L"Sword_UpToDown");
			}





		}



	}

	void GoddnessScript::FireSwordBullet()
	{



		float VerticalSpeed = 900.f;
		float HorizionSpeed = 1200.f;

		// ���ʿ��� ���������� �߻� , �Ʒ����� ���� 
		if (mPlayerPos.x >= 0.0)
		{



			if (miRandom == 0)
			{
				Vector3 LeftPos = mSwordBullet_Left->GetComponent<Transform>()->GetPosition();

				mSwordBullet_Left->GetComponent<Transform>()->SetPosition(LeftPos.x + HorizionSpeed * Time::DeltaTime(), LeftPos.y, LeftPos.z);



				//== �߾�
			   // ==== ����
				Vector3 MidPos = mSwordBullet_Mid->GetComponent<Transform>()->GetPosition();

				mSwordBullet_Mid->GetComponent<Transform>()->SetPosition(MidPos.x + HorizionSpeed * Time::DeltaTime(), MidPos.y, MidPos.z);


				// ==== ��
				// ==== ����
				Vector3 RightPos = mSwordBullet_Right->GetComponent<Transform>()->GetPosition();

				mSwordBullet_Right->GetComponent<Transform>()->SetPosition(RightPos.x + HorizionSpeed * Time::DeltaTime(), RightPos.y, RightPos.z);

			}

			else if (miRandom == 1)
			{
				// ==== ����
				Vector3 LeftPos = mSwordBullet_Left->GetComponent<Transform>()->GetPosition();

				mSwordBullet_Left->GetComponent<Transform>()->SetPosition(LeftPos.x, LeftPos.y + VerticalSpeed * Time::DeltaTime(), LeftPos.z);



				// == �߾�
				// ==== ����
				Vector3 MidPos = mSwordBullet_Mid->GetComponent<Transform>()->GetPosition();

				mSwordBullet_Mid->GetComponent<Transform>()->SetPosition(MidPos.x, MidPos.y + VerticalSpeed * Time::DeltaTime(), MidPos.z);


				// ==== ��
				// ==== ����
				Vector3 RightPos = mSwordBullet_Right->GetComponent<Transform>()->GetPosition();

				mSwordBullet_Right->GetComponent<Transform>()->SetPosition(RightPos.x, RightPos.y + VerticalSpeed * Time::DeltaTime(), RightPos.z);
			}



		}

		// �����ʿ��� �������� �߻� , ������ �Ʒ� 
		else if (mPlayerPos.x <= 0.0)
		{

			if (miRandom == 0)
			{
				Vector3 LeftPos = mSwordBullet_Left->GetComponent<Transform>()->GetPosition();

				mSwordBullet_Left->GetComponent<Transform>()->SetPosition(LeftPos.x - HorizionSpeed * Time::DeltaTime(), LeftPos.y, LeftPos.z);



				// == �߾�
				// ==== ����
				Vector3 MidPos = mSwordBullet_Mid->GetComponent<Transform>()->GetPosition();

				mSwordBullet_Mid->GetComponent<Transform>()->SetPosition(MidPos.x - HorizionSpeed * Time::DeltaTime(), MidPos.y, MidPos.z);


				// ==== ��
				// ==== ����
				Vector3 RightPos = mSwordBullet_Right->GetComponent<Transform>()->GetPosition();

				mSwordBullet_Right->GetComponent<Transform>()->SetPosition(RightPos.x - HorizionSpeed * Time::DeltaTime(), RightPos.y, RightPos.z);

			}

			else if (miRandom == 1)
			{


				// ==== ����
				Vector3 LeftPos = mSwordBullet_Left->GetComponent<Transform>()->GetPosition();

				mSwordBullet_Left->GetComponent<Transform>()->SetPosition(LeftPos.x, LeftPos.y - VerticalSpeed * Time::DeltaTime(), LeftPos.z);



				// == �߾�
				// ==== ����
				Vector3 MidPos = mSwordBullet_Mid->GetComponent<Transform>()->GetPosition();

				mSwordBullet_Mid->GetComponent<Transform>()->SetPosition(MidPos.x, MidPos.y - VerticalSpeed * Time::DeltaTime(), MidPos.z);


				// ==== ��
				// ==== ����
				Vector3 RightPos = mSwordBullet_Right->GetComponent<Transform>()->GetPosition();

				mSwordBullet_Right->GetComponent<Transform>()->SetPosition(RightPos.x, RightPos.y - VerticalSpeed * Time::DeltaTime(), RightPos.z);
			}



		}






	}



	eStompState GoddnessScript::GetRandomStompState()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(static_cast<int>(eStompState::SINGLE_STOMP), static_cast<int>(eStompState::QUADRUPLE_STOMP));

		return static_cast<eStompState>(dis(gen));
	}

}
