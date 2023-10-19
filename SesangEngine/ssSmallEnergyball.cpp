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
			mAnimator->PlayAnimation(L"Energyball_S_Parrying_Energying", true); // trigger �ϼ��ϸ� ����� 

			if (!mbSpawn)
			{
				mbSpawn = true; // ���� ���·� �Ѿ ��  falseó�� ���ֱ� 
			
				
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

			//mfTime = 0.0f; => 0���� �ʱ�ȭ�ϴ°Ŵ� �ٸ� ���·� �Ѿ �� �׳� ���� (���� ��ü�� �Ἥ �� ������ ������ �Ǵ� ��)
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
		const float radius = 90.0f; // ���ϴ� �������� ũ��� ���� ����

		float angleStep = 360.0f / numBalls; // 22.5�� �������� ���������� �����ϱ� ���� ���� ����

		Vector3 bossPosition = Vector3(2.3f, -32.f, mTransform->GetPosition().z); // 45.2

		const int offsetIndex = 14;  // ���� 13�� �ε����� 0������ ����� ���� ������ 


		for (int i = 0; i < numBalls; ++i)
		{
			// 14�� �ε����� 0������ ���� (0�� �ε������� �༮ ���� 14�� �ε����� �ִ� �༮�� 0������ �ٲٰ� �; i -2�� �ϸ� ���� ����� ������.
			// ������ ��ⷯ ������ �̿� 
			int adjustedIndex = (i + offsetIndex) % numBalls;


			const float angleOffset = 14.0f;  // �������� 30�� ������

			const float PI = 3.14159265358979323846f;

			float angle = angleStep * i + angleOffset; // �������� �߰��� ���� ��� // ó�� ������ ���������� ����Ͽ� ���� ����
			float x = bossPosition.x + (radius * cosf(ToRadian(angle)));
			float y = bossPosition.y + (radius * sinf(ToRadian(angle)));

			// �������� ����
			Vector3 spawnPos = Vector3(x, y, bossPosition.z);
			SmallEnergyball* ball = object::Instantiate<SmallEnergyball>(spawnPos, eLayerType::Collision, L"S_EnergyballObjs"); // �ʿ��� �ٸ� �Ķ���͵鵵 ä���� �մϴ�.
			ball->Initialize();
			ball->RemoveComponent<Collider2D>(); // ���� �� �浹ü ������ 

			// ���������� ���� ���� ����

			if (adjustedIndex < 4) // ����� 4��
				ball->SetDir(Vector3(0, 1, 0));

			else if (adjustedIndex < 8) // ���� 4��
				ball->SetDir(Vector3(-1, 0, 0));

			else if (adjustedIndex < 12) // ������ 4��
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
