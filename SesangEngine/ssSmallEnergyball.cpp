#include "ssSmallEnergyball.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssTime.h"
#include "ssSmallEnergyballScript.h"
#include "ssAnimator.h"
#include "ssobject.h"
#include "ssGoddnessScript.h"
#include "ssPlayer.h"
#include "ssAudioClip.h"
#include "ssAudioListener.h"
#include "ssAudioSource.h"
#include "ssSoundMgrScript.h"

namespace ss
{
	bool SmallEnergyball::mbSpawn = false;



	SmallEnergyball::SmallEnergyball()
		: mAnimator(nullptr)
		, mTransform(nullptr)
		, mbSpawnComplete(false)
		, mStorePlayerPos(false)
		, miMaxSpawnedBalls(12)
		, mPlayerPos(Vector3::Zero)
		, mfTime(0.0f)

	{
		
	}
	SmallEnergyball::~SmallEnergyball()
	{
	}
	void SmallEnergyball::Initialize()
	{
		mTransform = GetComponent<Transform>();
		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mAnimator = AddComponent<Animator>();


		mTransform->SetScale(Vector3(71.f, 64.f, 1.0f));
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"SmallEnergyBulletMtrl"));
	

		Collider2D* col = AddComponent<Collider2D>();
		col->SetSize(Vector2(0.1f, 0.1f));
		col->SetName(L"SmallEnergyballCol");


		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_S_Parrying_Spawn_Effect");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_S_Parrying_AfterSpawn_Effect");
		std::shared_ptr<ss::graphics::Texture> Image3 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_S_Parrying_End_Effect");


		mAnimator->Create(L"Energyball_S_Parrying_Spawn", Image1, Vector2(0.f, 0.f), Vector2(71.f, 64.f), 6, Vector2(71.f, 64.f), Vector2::Zero, 0.08f);
		mAnimator->Create(L"Energyball_S_Parrying_Energying", Image2, Vector2(0.f, 0.f), Vector2(71.f, 64.f), 6, Vector2(71.f, 64.f), Vector2::Zero, 0.08f);
		mAnimator->Create(L"Energyball_S_Parrying_End", Image3, Vector2(0.f, 0.f), Vector2(71.f, 64.f), 6, Vector2(71.f, 64.f), Vector2::Zero, 0.08f);


		mAnimator->PlayAnimation(L"Energyball_S_Parrying_Spawn", false);

		mAnimator->EndEvent(L"Energyball_S_Parrying_End") = std::bind(&SmallEnergyball::HitEnd, this);
	


		AddComponent<SmallEnergyballScript>();


		Bullet::Initialize();
	}
	void SmallEnergyball::Update()
	{
		
		

		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			mAnimator->PlayAnimation(L"Energyball_S_Parrying_Energying", true); // trigger 완성하면 지우기 

			if (!mbSpawn)
			{
				mbSpawn = true; // 다음 상태로 넘어갈 때  false처리 해주기 
			
				
				CreateCircleBalls();
				
			}
		}

		
		mfTime += Time::DeltaTime();

		if (mfTime > 2.f)
		{
			if (GetName() == L"S_EnergyballObjs" && mbSpawn)
			{
				Collider2D* col = AddComponent<Collider2D>();
				col->SetSize(Vector2(0.8f, 0.8f));
			}

			float speed = 170.f;

		
			Transform* tr = GetComponent<Transform>();
			Vector3 Pos = tr->GetPosition();

			Pos += GetDir() * speed * Time::DeltaTime();
			tr->SetPosition(Pos);

			//mfTime = 0.0f; => 0으로 초기화하는거는 다른 상태로 넘어갈 떄 그냥 하자 (여러 객체를 써서 이 떄문에 문제가 되는 듯)
		}



		Bullet::Update();
	}
	void SmallEnergyball::LateUpdate()
	{
		Bullet::LateUpdate();
	}
	void SmallEnergyball::Render()
	{
		Bullet::Render();
	}

	void SmallEnergyball::HitStart()
	{

		RemoveComponent<Collider2D>();

	}


	void SmallEnergyball::Spawn_sfx()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
		pSFX->SetClip(Resources::Find<AudioClip>(L"GoddnessObj_Ball_Spawn_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.1f);
	}

	void SmallEnergyball::Fire_sfx()
	{

		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
		pSFX->SetClip(Resources::Find<AudioClip>(L"GoddnessObj_Ball_SpawnFire_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.2f);
	}

	void SmallEnergyball::CreateCircleBalls()
	{
		const int numBalls = 16;
		const float radius = 90.0f; // 원하는 반지름의 크기로 변경 가능

		float angleStep = 360.0f / numBalls; // 22.5도 간격으로 에너지볼을 생성하기 위한 각도 스텝

		Vector3 bossPosition = Vector3(2.3f, -32.f, mTransform->GetPosition().z); // 45.2

		const int offsetIndex = 14;  // 기존 13번 인덱스를 0번으로 만들기 위한 오프셋 


		for (int i = 0; i < numBalls; ++i)
		{
			// 14번 인덱스를 0번으로 설정 (0번 인덱스였던 녀석 말고 14번 인덱스에 있던 녀석을 0번으로 바꾸고 싶어서 i -2로 하면 범위 벗어나서 오류남.
			// 때문에 모듈러 연산을 이용 
			int adjustedIndex = (i + offsetIndex) % numBalls;


			const float angleOffset = 14.0f;  // 왼쪽으로 30도 오프셋

			const float PI = 3.14159265358979323846f;

			float angle = angleStep * i + angleOffset; // 오프셋을 추가한 각도 계산 // 처음 생성된 에너지볼을 고려하여 각도 조정
			float x = bossPosition.x + (radius * cosf(ToRadian(angle)));
			float y = bossPosition.y + (radius * sinf(ToRadian(angle)));

			// 에너지볼 생성
			Vector3 spawnPos = Vector3(x, y, bossPosition.z);
			SmallEnergyball* ball = object::Instantiate<SmallEnergyball>(spawnPos, eLayerType::Collision, L"S_EnergyballObjs"); // 필요한 다른 파라미터들도 채워야 합니다.
			ball->Initialize();
			ball->RemoveComponent<Collider2D>(); // 날라갈 때 충돌체 입히기 

			// 에너지볼에 방향 벡터 설정

			if (adjustedIndex < 4) // 상단의 4개
				ball->SetDir(Vector3(0, 1, 0));

			else if (adjustedIndex < 8) // 왼쪽 4개
				ball->SetDir(Vector3(-1, 0, 0));

			else if (adjustedIndex < 12) // 오른쪽 4개
				ball->SetDir(Vector3(0, -1, 0));

			else 
				ball->SetDir(Vector3(1, 0, 0));



		}
	
	}

	void SmallEnergyball::HitEnd()
	{

		SetState(GameObject::eState::Dead);

	}



}
