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
#include "ssEnergyballScript.h"
#include "ssSceneManager.h"
#include <random>
#include "ssRenderer.h"
#include "ssCameraScript.h"
#include "ssBackground.h"


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
	{
		m_tMonsterInfo.m_fSpeed = 200.f;
		m_tMonsterInfo.m_fAttack = 10.f;
		m_tMonsterInfo.m_fDetectRange = 300.f;
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


		std::shared_ptr<ss::graphics::Texture> Image6 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_Die");
	
		// ==== 1������ ���� 
		mAnimator->Create(L"Boss_Goddness_Idle", Image1, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 2, Vector2(269.f, 308.f));
		mAnimator->Create(L"Boss_Goddness_Stomp", Image2, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 4, Vector2(269.f, 308.f));
		mAnimator->Create(L"Boss_Goddness_EnergyballStart", Image3, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 11, Vector2(269.f, 308.f));
		mAnimator->Create(L"Boss_Goddness_EnergyballEnd", Image4, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 5, Vector2(269.f, 308.f));
		mAnimator->Create(L"Boss_Goddness_Die", Image5, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 16, Vector2(269.f, 308.f));


		// ==== 2������ ��
	

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

		// ===== ������ٵ�



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


		if (mBossType == eBossType::STATUE)
		{
			switch (mCurBoss2_Phase1_State)
			{
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
				break;

			case ss::eBoss2_Phase2::INTRO_END:
				break;

			case ss::eBoss2_Phase2::IDLE:
				break;

			case ss::eBoss2_Phase2::MOVE_FRONT:
				break;

			case ss::eBoss2_Phase2::MOVE_BACK:
				break;

			case ss::eBoss2_Phase2::DIAGONAL_ATTACK_DIAGONAL:
				break;

			case ss::eBoss2_Phase2::DIAGONAL_ATTACK_VERTICAL:
				break;

			case ss::eBoss2_Phase2::ENERGYBALL_START:
				break;

			case ss::eBoss2_Phase2::ENERGYBALL_ING:
				break;

			case ss::eBoss2_Phase2::ENERGYBALL_END:
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

			case ss::eBoss2_Phase2::SUMMONSPEAR:
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



	void GoddnessScript::Idle()
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

					if(!mbFirstStomp)
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
	


	
		
		
		
		/*if (mStatueState == eStatueState::MOVING_UP && miStompCount == 1)
		{
			ChangeState(eBoss2_Phase1::STOMP_ING);
		}*/
		
	}

	void GoddnessScript::Energyball_Start()
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


			float moveSpeed = 130.0f;  // ���ϴ� �ӵ� ���� �����ϼ���.

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
	void GoddnessScript::Energyball_ing()
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

		if (mEngeryball->GetSpawnCount() == 12 && m_fTime > 13.1f)
		{
			ChangeState(eBoss2_Phase1::ENERGYBALL_END);
			mbEnergySpawn = false;
			mEngeryball->InitializeSpawnCount();
			m_fTime = 0.0f;
		}



	}
	void GoddnessScript::Energyball_End()
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
	void GoddnessScript::Wind_Ready()
	{
	}

	void GoddnessScript::Dead()
	{

		SetHit(false); // dead ���� �������� �� �ǰݴ��ص� ������ ������ �ʵ��� �Ѵ�. 
		GetOwner()->RemoveComponent<Collider2D>(); // dead ���¿��� ��� ���������ű� ������ Ȥ�ø� �Ͽ� ����� ������Ʈ�� ���д�. 

		mCamera->GetComponent<Camera>()->SetTargetSize(5.3f);


		Vector3 CameraPos = mCamera->GetComponent<Transform>()->GetPosition();
		mCamera->GetComponent<Transform>()->SetPosition(CameraPos.x, CameraPos.y - 0.02f, CameraPos.z);

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
			mCamera->GetComponent<Camera>()->SetTargetSize(2.3f); // �� �ƿ��� �� �ε巴�� �ǵ��� �ص� 
		}

		mAnimator->PlayAnimation(L"Boss_Goddness_Die", false);




		// �ִϸ��̼� ����� ������ 
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			
			//mAttackColliderObj->SetState(GameObject::eState::Dead);
		
			mCamera->GetComponent<Transform>()->SetPosition(CameraPos.x, CameraPos.y, CameraPos.z);
	
			GetOwner()->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(L"tempMtrl"));

			mBossHPFrame->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(L"tempMtrl")); // ��� �������ٰ� spawn�� �ٽ� ���� 

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