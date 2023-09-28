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



namespace ss
{
	GoddnessScript::GoddnessScript()
		: miStompCount(0)
		, miCompleteStompCount(0)
		, mbStomp(false)
		, mTest(false)
		, mbEnergySpawn(false)
		, mStatueState(eStatueState::MOVING_DOWN)
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




		mCharacterState->SetMaxHP(110.f);
		mCharacterState->SetCurrentHP(110.f);

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
		mAnimator->Create(L"Boss_Goddness_Die", Image5, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 22, Vector2(269.f, 308.f));


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


		mAnimator->PlayAnimation(L"Boss_Goddness_Idle", true); 



		// �������� ���¸� �����Ѵ�. 
		int randomValue = rand() % 2; // 0 �Ǵ� 1�� ���� (��������) 


		// ���� ���ڰ� stomp Ready�� �ش��ϴ°Ŷ�� ���¸� �ٲٱ� ���� �Ʒ��� ���� ������ �߰����ش�.
		// ī�޶� �÷��̾� ������ ���� �� Ȯ��ȴ�. (stomp ���� �Ͻ�) 
		m_fTime += Time::DeltaTime();


	
		if (mTest)
		{
			ChangeState(eBoss2_Phase1::STOMP_READY);
			mPrevBoss2_Phase1_State = eBoss2_Phase1::IDLE;

			m_fTime = 0.f;

			
		}

		else if (!mTest && m_fTime > 3.0f)
		{

			//if (mPrevBoss2_Phase1_State == eBoss2_Phase1::STOMP_END) // �׽�Ʈ �Ϸ�. �������� ��� �����Ǹ� �̰� �ٽ� �ǻ츮�� 
			//{
			//	// ������ �߾����� �̵��Ѵ�. 
			//	Vector3 StatuePos = mTransform->GetPosition();
			//	Vector3 FirstPos = Vector3(0.f, -87.f, 500.f);


			//	Vector3 TargetPos = Vector3(0.f, FirstPos.y + 67.f, FirstPos.z);


			//	// ���󿡼� ��ǥ ��ġ���� ���� ���͸� ����մϴ�.
			//	Vector3 dir = TargetPos - StatuePos;
			//	float distance = dir.Length();  // ����� ��ǥ ��ġ ������ �Ÿ��� ����մϴ�.
			//	dir.Normalize();  // ���� ���͸� ����ȭ�մϴ�.


			//	float moveSpeed = 150.0f;  // ���ϴ� �ӵ� ���� �����ϼ���.

			//	Vector3 moveAmount = dir * moveSpeed * Time::DeltaTime();  // �����Ӵ� ������ ���� ����մϴ�.

			//	StatuePos += moveAmount;  // ���� ��ġ�� ������Ʈ�մϴ�.
			//	mTransform->SetPosition(StatuePos);  // ������Ʈ�� ��ġ�� �����մϴ�.
			//}




			// == �ʱⰪ ��ġ���� y���� ��� ���� ���� �ø���. 
			Vector3 StatuePos = mTransform->GetPosition();

			float moveSpeed = 100.0f;  // ���ϴ� �ӵ� ���� �����ϼ���.

			float moveAmountY = moveSpeed * Time::DeltaTime();  // �����Ӵ� ������ ���� ����մϴ�.

	
			// �ö󰡴� ��ġ ���� 
			if (StatuePos.y < -21.f)
			{

				StatuePos.y += moveAmountY;
				mTransform->SetPosition(StatuePos);

			
			}

			
			m_fTime += Time::DeltaTime();

			if (m_fTime > 10.0f)
			{

				ChangeState(eBoss2_Phase1::ENERGYBALL_READY);
				mPrevBoss2_Phase1_State = eBoss2_Phase1::IDLE;

				m_fTime = 0.f;
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
		 
		mAnimator->PlayAnimation(L"Boss_Goddness_Stomp", false);
		 
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

			// ������ �밢������ �̵��Ѵ�. 
			Vector3 StatuePos = mTransform->GetPosition();
			Vector3 HitGroundPos = mHitGround->GetComponent<Transform>()->GetPosition();


			Vector3 TargetPos = Vector3(HitGroundPos.x, HitGroundPos.y + 250.f, HitGroundPos.z);


			// ���󿡼� ��ǥ ��ġ���� ���� ���͸� ����մϴ�.
			Vector3 dir = TargetPos - StatuePos;
			float distance = dir.Length();  // ����� ��ǥ ��ġ ������ �Ÿ��� ����մϴ�.
			dir.Normalize();  // ���� ���͸� ����ȭ�մϴ�.


			float moveSpeed = 250.0f;  // ���ϴ� �ӵ� ���� �����ϼ���.

			Vector3 moveAmount = dir * moveSpeed * Time::DeltaTime();  // �����Ӵ� ������ ���� ����մϴ�.

			StatuePos += moveAmount;  // ���� ��ġ�� ������Ʈ�մϴ�.
			mTransform->SetPosition(StatuePos);  // ������Ʈ�� ��ġ�� �����մϴ�.

			

			m_fTime += Time::DeltaTime();
			// ����� �ٴٸ��� ���¸� �ٲ۴�. 

			if (m_fTime > 1.5f)
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
			break;

		case eStatueState::MOVING_UP:
			TargetPosY = HitGroundPosY + 250.f;
			break;
		}

		float dirY = TargetPosY - StatuePosY;

		float moveDistance = abs(dirY);
		dirY = dirY > 0 ? 1.0f : -1.0f;

		float moveSpeed = 250.0f;
		float moveAmountY = dirY * moveSpeed * Time::DeltaTime();

		StatuePosY += moveAmountY; 


		mTransform->SetPosition(Vector3(mTransform->GetPosition().x, StatuePosY, mTransform->GetPosition().z));


		// ���� ��ȯ ����
		if ((dirY > 0 && StatuePosY >= TargetPosY) || (dirY < 0 && StatuePosY <= TargetPosY)) 
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
				mHitGround->SetState(GameObject::eState::Dead);
			}

			else
			{
				ChangeState(eBoss2_Phase1::STOMP_ING);
			}
		}
	}



	void GoddnessScript::Stomp_End()
	{
		// ī�޶� �� �� Ȯ����Ѽ� �� Ÿ������ �˷��ش�. 



	    miStompCount = 0; // 0�� ���� �ʱ�ȭ���� �� �����غ��� 
		mAnimator->PlayAnimation(L"Boss_Goddness_Idle", true); 

		m_fTime += Time::DeltaTime();


	
		if (m_fTime > 2.0f)
		{
			// ���°� �ٲ� �� ī�޶� �ٽ� ������� �������´�. 

			if (miCompleteStompCount < 3)
			{

				ChangeState(eBoss2_Phase1::STOMP_READY);
				++miCompleteStompCount;

	
			}

			else if (miCompleteStompCount == 3)
			{		
				ChangeState(eBoss2_Phase1::IDLE);
				mPrevBoss2_Phase1_State = eBoss2_Phase1::STOMP_END;
			}
			
			m_fTime = 0.0f;

		}
	
		
		
		
		/*if (mStatueState == eStatueState::MOVING_UP && miStompCount == 1)
		{
			ChangeState(eBoss2_Phase1::STOMP_ING);
		}*/
		
	}

	void GoddnessScript::Energyball_Start()
	{

		Vector3 StatuePos = mTransform->GetPosition();


		if (!mbEnergySpawn)
		{
			mbEnergySpawn = true;
			mEngeryball = object::Instantiate<Energyball>(Vector3(StatuePos.x, StatuePos.y, 300.f), eLayerType::Collision, L"Parrying_S_EnergyballObj");
			mEngeryball->SetOriginOwenr(mTransform->GetOwner());

			EnergyballScript* script = mEngeryball->AddComponent<EnergyballScript>();

		}
	



	}
	void GoddnessScript::Energyball_ing()
	{
	}
	void GoddnessScript::Energyball_End()
	{
	}
	void GoddnessScript::Wind_Ready()
	{
	}
	void GoddnessScript::Dead()
	{
	}

}