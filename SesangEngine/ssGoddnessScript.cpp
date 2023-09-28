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
	
		// ==== 1페이즈 석상 
		mAnimator->Create(L"Boss_Goddness_Idle", Image1, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 2, Vector2(269.f, 308.f));
		mAnimator->Create(L"Boss_Goddness_Stomp", Image2, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 4, Vector2(269.f, 308.f));
		mAnimator->Create(L"Boss_Goddness_EnergyballStart", Image3, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 11, Vector2(269.f, 308.f));
		mAnimator->Create(L"Boss_Goddness_EnergyballEnd", Image4, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 5, Vector2(269.f, 308.f));
		mAnimator->Create(L"Boss_Goddness_Die", Image5, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 22, Vector2(269.f, 308.f));


		// ==== 2페이즈 신
	

		mAnimator->PlayAnimation(L"Boss_Goddness_Idle", true); // trigger 완성하면 지우기 
		// ======
	// 애니메이션 방향에 관한 기준	을 잡아준다.


		mDir = -mTransform->Right();



		// 몬스터의 초기 상태는 부여해주지 않는다. (보스만) 


		// ===== 기본 충돌체 Hit 받는 용도 
		mCollider->SetName(L"Goddness_HitCol");
		mCollider->SetType(eColliderType::Rect);
		mCollider->SetSize(Vector2(0.6f, 0.36f));
		mCollider->SetCenter(Vector2(-4.f, -84.f));

		// ===== 리지드바디



		////==== 근접 공격 특정 인덱스 충돌체 
		////충돌체는 여기서 바로 넣지 말고 해당 인덱스 때 넣었다가 빼는 식으로 하기 
		//mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"BigWolf_AttackColObj");
		//mAttackColliderObj->Initialize();

		//mAttackColTr = mAttackColliderObj->GetComponent<Transform>();


		// trigger 완성하면 이거 씌우기 
		//mMeshRenderer->SetMaterial(Resources::Find<Material>(L"tempMtrl"));

	}
	void GoddnessScript::Update()
	{

		// 이동->상태변환->애니메이션


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



		// 랜덤으로 상태를 변경한다. 
		int randomValue = rand() % 2; // 0 또는 1을 생성 (랜덤으로) 


		// 랜덤 숫자가 stomp Ready에 해당하는거라면 상태를 바꾸기 전에 아래와 같은 사항을 추가해준다.
		// 카메라가 플레이어 쪽으로 점점 더 확대된다. (stomp 패턴 암시) 
		m_fTime += Time::DeltaTime();


	
		if (mTest)
		{
			ChangeState(eBoss2_Phase1::STOMP_READY);
			mPrevBoss2_Phase1_State = eBoss2_Phase1::IDLE;

			m_fTime = 0.f;

			
		}

		else if (!mTest && m_fTime > 3.0f)
		{

			//if (mPrevBoss2_Phase1_State == eBoss2_Phase1::STOMP_END) // 테스트 완료. 에너지볼 기능 구현되면 이거 다시 되살리기 
			//{
			//	// 석상이 중앙으로 이동한다. 
			//	Vector3 StatuePos = mTransform->GetPosition();
			//	Vector3 FirstPos = Vector3(0.f, -87.f, 500.f);


			//	Vector3 TargetPos = Vector3(0.f, FirstPos.y + 67.f, FirstPos.z);


			//	// 석상에서 목표 위치로의 방향 벡터를 계산합니다.
			//	Vector3 dir = TargetPos - StatuePos;
			//	float distance = dir.Length();  // 석상과 목표 위치 사이의 거리를 계산합니다.
			//	dir.Normalize();  // 방향 벡터를 정규화합니다.


			//	float moveSpeed = 150.0f;  // 원하는 속도 값을 설정하세요.

			//	Vector3 moveAmount = dir * moveSpeed * Time::DeltaTime();  // 프레임당 움직일 양을 계산합니다.

			//	StatuePos += moveAmount;  // 현재 위치를 업데이트합니다.
			//	mTransform->SetPosition(StatuePos);  // 업데이트된 위치를 설정합니다.
			//}




			// == 초기값 위치에서 y값만 어느 정도 위로 올린다. 
			Vector3 StatuePos = mTransform->GetPosition();

			float moveSpeed = 100.0f;  // 원하는 속도 값을 설정하세요.

			float moveAmountY = moveSpeed * Time::DeltaTime();  // 프레임당 움직일 양을 계산합니다.

	
			// 올라가는 위치 제한 
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




		// 만약에 에너지볼 상태로 넘어간다면 
		// 이전 상태가 stomp 상태였다면 일단 가운데로 위치를 옮긴다.
		// 그게 아니라면 씬에 배치했던 첫 위치에서 위로 조금만 위치를 올린다.
		// 그 다음에 상태를 전환한다. 



	}
	void GoddnessScript::Hit()
	{
	}
	void GoddnessScript::Stomp_Ready()
	{

		// 확대됐던 카메라가 stomp 애니메이션 첫 인덱스에 다시 원래 사이즈인 2.3값으로 돌아온다. 

		// 잠시 애니메이션 재생을 멈춘다. (0인덱스에 머무르게), 양 옆에 벽에 가시가 나온다.
		 
		mAnimator->PlayAnimation(L"Boss_Goddness_Stomp", false);
		 
		// 벽에 가시가 다 나오면 플레이어의 위치 위에 떠있는다 (체감상 2초 정도?)
		bool isGround = mPlayer->GetComponent<Rigidbody2D>()->IsGround();

		if (!mbStomp && isGround)
		{
			// hit ground를 띄운다. 
			// hit ground 쪽으로 이동한다.
		
			Vector3 PlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();


			// hit ground를 띄운다.
			mHitGround = object::Instantiate<Effect>(Vector3(PlayerPos.x, PlayerPos.y - 20.f, PlayerPos.z), eLayerType::Collision, L"StompHitGroundObj");

			HitGroundScript* script = mHitGround->AddComponent<HitGroundScript>();
			script->SetMonsterOwner((Monster*)mTransform->GetOwner());

			mbStomp = true; 


		}

		if (nullptr != mHitGround)
		{

			// 석상이 대각선으로 이동한다. 
			Vector3 StatuePos = mTransform->GetPosition();
			Vector3 HitGroundPos = mHitGround->GetComponent<Transform>()->GetPosition();


			Vector3 TargetPos = Vector3(HitGroundPos.x, HitGroundPos.y + 250.f, HitGroundPos.z);


			// 석상에서 목표 위치로의 방향 벡터를 계산합니다.
			Vector3 dir = TargetPos - StatuePos;
			float distance = dir.Length();  // 석상과 목표 위치 사이의 거리를 계산합니다.
			dir.Normalize();  // 방향 벡터를 정규화합니다.


			float moveSpeed = 250.0f;  // 원하는 속도 값을 설정하세요.

			Vector3 moveAmount = dir * moveSpeed * Time::DeltaTime();  // 프레임당 움직일 양을 계산합니다.

			StatuePos += moveAmount;  // 현재 위치를 업데이트합니다.
			mTransform->SetPosition(StatuePos);  // 업데이트된 위치를 설정합니다.

			

			m_fTime += Time::DeltaTime();
			// 허공에 다다르면 상태를 바꾼다. 

			if (m_fTime > 1.5f)
			{
				ChangeState(eBoss2_Phase1::STOMP_ING);
				m_fTime = 0.0f;
				mbStomp = false;
			}

		}
	



		// 2초가 지나면 연달아 3번 그 위치에 도장찍듯이 떨어진다. 

		// 3번 횟수 때는 바닥에 붙어있는 상태로 유지한다. 몇 초 후에, 플레이어의 위치를 다시 체크한다.

		//몇 초가 지나면 연달아 3번 또 찍는다.

		// 이 패턴이 4번 지속되면 Idle 상태로 돌아간다.



		
		




	}
	void GoddnessScript::Stomp_Ing()
	{


		float StatuePosY = mTransform->GetPosition().y;
		float HitGroundPosY = mHitGround->GetComponent<Transform>()->GetPosition().y;
		float TargetPosY = 0.0f;

		// 상태에 따른 목표 위치 설정
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


		// 상태 전환 로직
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
		// 카메라를 좀 더 확대시켜서 딜 타임임을 알려준다. 



	    miStompCount = 0; // 0을 언제 초기화할지 좀 생각해보기 
		mAnimator->PlayAnimation(L"Boss_Goddness_Idle", true); 

		m_fTime += Time::DeltaTime();


	
		if (m_fTime > 2.0f)
		{
			// 상태가 바뀔 때 카메라를 다시 원래대로 돌려놓는다. 

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