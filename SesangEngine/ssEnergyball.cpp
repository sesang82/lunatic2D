#include "ssEnergyball.h"
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
	int Energyball::miSpawnedBallCount = 0;

	Energyball::Energyball()
		: mAnimator(nullptr)
		, mTransform(nullptr)
		, mbTest(false)
		, mbSpawnComplete(false)
		, mStorePlayerPos(false)
		, miMaxSpawnedBalls(12)
		, mPlayerPos(Vector3::Zero)
		, mfTime(0.0f)
	{

		spawnPatterns =
		{
			// LT
			{0, 35},
			{0, -35}, // 아래
			{35, 35}, // 오른쪽


			// LB
			{-35, -105},  // 꼭짓점
			{0, 35}, // 위 
			{35, -35}, // 오른쪽

			// RT
			{70, 0}, // 꼭짓점
			{0, 35}, // 위
			{-35, -35}, // 아래 

			// RB
			{35, 105}, // 꼭짓점
			{0, -35}, // 아래
			{-35, 35}, // 위
		};

		Energyball::Initialize();


	}
	Energyball::~Energyball()
	{
	}
	void Energyball::Initialize()
	{
		mTransform = GetComponent<Transform>();
		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mAnimator = AddComponent<Animator>();


		mTransform->SetScale(Vector3(71.f, 64.f, 1.0f));
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimationMaterial"));

		AddComponent<Collider2D>();



		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_S_Parrying_Spawn_Effect");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_S_Parrying_AfterSpawn_Effect");
		std::shared_ptr<ss::graphics::Texture> Image3 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_S_Parrying_End_Effect");

		mAnimator->Create(L"Energyball_S_Parrying_Spawn", Image1, Vector2(0.f, 0.f), Vector2(71.f, 64.f), 6, Vector2(71.f, 64.f), Vector2::Zero, 0.07f);
		mAnimator->Create(L"Energyball_S_Parrying_Energying", Image2, Vector2(0.f, 0.f), Vector2(71.f, 64.f), 6, Vector2(71.f, 64.f), Vector2::Zero, 0.07f);
		mAnimator->Create(L"Energyball_S_Parrying_End", Image3, Vector2(0.f, 0.f), Vector2(71.f, 64.f), 6, Vector2(71.f, 64.f), Vector2::Zero, 0.07f);


		mAnimator->PlayAnimation(L"Energyball_S_Parrying_Spawn", false);
	
		mAnimator->EndEvent(L"Energyball_S_Parrying_End") = std::bind(&Energyball::HitEnd, this);
		mAnimator->StartEvent(L"Energyball_S_Parrying_End") = std::bind(&Energyball::Spawn_sfx, this);
		mAnimator->StartEvent(L"Energyball_S_Parrying_Energying") = std::bind(&Energyball::Fire_sfx, this);


		AddComponent<SmallEnergyballScript>();

		
		

		Bullet::Initialize();
	}
	void Energyball::Update()
	{

		int i = miSpawnedBallCount;

			mPlayer = SceneManager::GetPlayer();

			RemoveComponent<Collider2D>();


		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			mAnimator->PlayAnimation(L"Energyball_S_Parrying_Energying", true);

			if (!mbTest)
			{
				CreateEnergyball();
				
				//object::Instantiate<Energyball>(Vector3(mTransform->GetPosition().x, mTransform->GetPosition().y - 35.f, 300.f), eLayerType::Collision, L"Parrying_S_EnergyballObj2"); // 아래
				//object::Instantiate<Energyball>(Vector3(mTransform->GetPosition().x + 60.f, mTransform->GetPosition().y + 35.f, 300.f), eLayerType::Collision, L"Parrying_S_EnergyballObj3"); // 오른쪽


			//	object::Instantiate<Energyball>(Vector3(mTransform->GetPosition().x, mTransform->GetPosition().y - 35.f, 300.f), eLayerType::Collision, L"Parrying_S_EnergyballObj2"); // 오른쪽

				mbTest = true;
				mbSpawnComplete = true;
			}	

		}

		mfTime += Time::DeltaTime();

		if (mfTime > 7.f && mbSpawnComplete && miSpawnedBallCount == 12)
		{
			Collider2D* col = AddComponent<Collider2D>();
			col->SetSize(Vector2(0.8f, 0.8f));
			col->SetName(L"EnergyballCol");

			if (!mStorePlayerPos)
			{
				mPlayer = SceneManager::GetPlayer();
				mPlayerPos = mPlayer->GetComponent<Transform>()->GetPosition();
				mStorePlayerPos = true;
			
			}

			Vector3 TargetPos = Vector3(mPlayerPos.x, mPlayerPos.y, mPlayerPos.z);
			Vector3 EnergyballPos = mTransform->GetPosition();

			// 석상에서 목표 위치로의 방향 벡터를 계산합니다.
			Vector3 dir = TargetPos - EnergyballPos;
			float distance = dir.Length();  // 석상과 목표 위치 사이의 거리를 계산합니다.
			dir.Normalize();  // 방향 벡터를 정규화합니다.


			float moveSpeed = 400.0f;  //. 380

			Vector3 moveAmount = dir * moveSpeed * Time::DeltaTime();  // 프레임당 움직일 양을 계산합니다.

			EnergyballPos += moveAmount;  // 현재 위치를 업데이트합니다.
			mTransform->SetPosition(EnergyballPos);  // 업데이트된 위치를 설정합니다.

		}

		Bullet::Update();
	}
	void Energyball::LateUpdate()
	{
		Bullet::LateUpdate();
	}
	void Energyball::Render()
	{
		Bullet::Render();
	}

	void Energyball::CreateEnergyball()
	{

		// 12개까지만 생성되게 해둠 
		if (miMaxSpawnedBalls <= miSpawnedBallCount)
			return;


		Vector2 offset = spawnPatterns[miSpawnedBallCount]; // stiatc으로 해뒀기 때문에 객체가 생성되도 원하는 인덱스값의 위치 값 가져올 수 있음 


		object::Instantiate<Energyball>(Vector3(mTransform->GetPosition().x + offset.x, mTransform->GetPosition().y + offset.y, 400.f),
			eLayerType::Collision, L"Parrying_S_EnergyballObj"); // 아래

		++miSpawnedBallCount;


	}

	void Energyball::HitStart()
	{

		RemoveComponent<Collider2D>();

	}

	void Energyball::HitEnd()
	{

		SetState(GameObject::eState::Dead);

	}

	void Energyball::Spawn_sfx()
	{

		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
		if (pSFX->GetClip() == Resources::Find<AudioClip>(L"GoddnessObj_Ball_Spawn_Bgm"))
			return;

		pSFX->SetClip(Resources::Find<AudioClip>(L"GoddnessObj_Ball_Spawn_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.2f);
	}

	void Energyball::Fire_sfx()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetSFX();
		if (pSFX->GetClip() == Resources::Find<AudioClip>(L"GoddnessObj_Ball_SpawnFire_Bgm"))
			return;

		pSFX->SetClip(Resources::Find<AudioClip>(L"GoddnessObj_Ball_SpawnFire_Bgm"));
		pSFX->Play();
		pSFX->SetVolume(0.1f);

	}






	
}