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
		SmallEnergyball::Initialize();
	}
	SmallEnergyball::~SmallEnergyball()
	{
	}
	void SmallEnergyball::Initialize()
	{
		mTransform = GetComponent<Transform>();
		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mAnimator = AddComponent<Animator>();


		mTransform->SetScale(Vector3(58.f, 54.f, 1.0f));
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimationMaterial"));

		Collider2D* col = AddComponent<Collider2D>();
		col->SetSize(Vector2(1.f, 1.f));
		col->SetName(L"EnergyballCol");


		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_S_Parrying_Spawn_Effect");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_S_Parrying_AfterSpawn_Effect");

		mAnimator->Create(L"Energyball_S_Parrying_Spawn", Image1, Vector2(0.f, 0.f), Vector2(58.f, 54.f), 6, Vector2(58.f, 54.f), Vector2::Zero, 0.08f);
		mAnimator->Create(L"Energyball_S_Parrying_Energying", Image2, Vector2(0.f, 0.f), Vector2(58.f, 54.f), 6, Vector2(58.f, 54.f), Vector2::Zero, 0.08f);


		mAnimator->PlayAnimation(L"Energyball_S_Parrying_Spawn", false); // trigger 완성하면 지우기 

		

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
				mbSpawn = true; // dead 처리될 때 false처리 해주기 
				CreateCircleBalls();
			}


			float speed = 200.f;

			Vector3 newPos = mTransform->GetPosition() + mDir * speed * Time::DeltaTime();
			mTransform->SetPosition(newPos);
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

	void SmallEnergyball::CreateCircleBalls()
	{
		const int numBalls = 15;
		const float radius = 100.0f; // 원하는 반지름의 크기로 변경 가능

		float angleStep = 360.0f / (numBalls + 1); // 22.5도 간격으로 에너지볼을 생성하기 위한 각도 스텝

		Vector3 bossPosition = Vector3(2.5f, -45.2f, mTransform->GetPosition().z);

		for (int i = 0; i < numBalls; ++i)
		{

			const float PI = 3.14159265358979323846f;

			float angle = angleStep *(i + 1); // 처음 생성된 에너지볼을 고려하여 각도 조정
			float x = bossPosition.x + (radius * cosf(ToRadian(angle)));
			float y = bossPosition.y + (radius * sinf(ToRadian(angle)));

			// 에너지볼 생성
			Vector3 spawnPos = Vector3(x, y, bossPosition.z);
			SmallEnergyball* ball = object::Instantiate<SmallEnergyball>(spawnPos, eLayerType::Collision, L"S_EnergyballObj"); // 필요한 다른 파라미터들도 채워야 합니다.

			// 에너지볼에 방향 벡터 설정
			Vector3 Balldir = (spawnPos - bossPosition);
			Balldir.Normalize();
			mDir = Balldir;

		}
	}



}
