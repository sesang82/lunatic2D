#include "ssEnergyball.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssTime.h"
#include "ssEnergyballScript.h"
#include "ssAnimator.h"
#include "ssobject.h"
#include "ssGoddnessScript.h"

namespace ss
{
	int Energyball::miSpawnedBallCount = 0;

	Energyball::Energyball()
		: mAnimator(nullptr)
		, mTransform(nullptr)
		, mbTest(false)
		, miMaxSpawnedBalls(11)
	{

		spawnPatterns =
		{
			// LT
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


		mTransform->SetScale(Vector3(58.f, 54.f, 1.0f));
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimationMaterial"));

		Collider2D* col = AddComponent<Collider2D>();
		col->SetSize(Vector2(1.f, 1.f));
		col->SetName(L"EnergyballCol");


		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_S_Parrying_Spawn_Effect");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_S_Parrying_AfterSpawn_Effect");

		mAnimator->Create(L"Energyball_S_Parrying_Spawn", Image1, Vector2(0.f, 0.f), Vector2(58.f, 54.f), 6, Vector2(58.f, 54.f));
		mAnimator->Create(L"Energyball_S_Parrying_Energying", Image2, Vector2(0.f, 0.f), Vector2(58.f, 54.f), 6, Vector2(58.f, 54.f));


		mAnimator->PlayAnimation(L"Energyball_S_Parrying_Spawn", false); // trigger 완성하면 지우기 

		

		Bullet::Initialize();
	}
	void Energyball::Update()
	{
		
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			mAnimator->PlayAnimation(L"Energyball_S_Parrying_Energying", true);

			
			//GoddnessScript* boss = mOwner->GetComponent<GoddnessScript>();

			//Vector3 bossPos = mOwner->GetComponent<Transform>()->GetPosition();


			if (!mbTest)
			{
				CreateEnergyball();
				
				//object::Instantiate<Energyball>(Vector3(mTransform->GetPosition().x, mTransform->GetPosition().y - 35.f, 300.f), eLayerType::Collision, L"Parrying_S_EnergyballObj2"); // 아래
				//object::Instantiate<Energyball>(Vector3(mTransform->GetPosition().x + 60.f, mTransform->GetPosition().y + 35.f, 300.f), eLayerType::Collision, L"Parrying_S_EnergyballObj3"); // 오른쪽


			//	object::Instantiate<Energyball>(Vector3(mTransform->GetPosition().x, mTransform->GetPosition().y - 35.f, 300.f), eLayerType::Collision, L"Parrying_S_EnergyballObj2"); // 오른쪽


				mbTest = true;
			}



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


		object::Instantiate<Energyball>(Vector3(mTransform->GetPosition().x + offset.x, mTransform->GetPosition().y + offset.y, 300.f),
			eLayerType::Collision, L"Parrying_S_EnergyballObj"); // 아래

		++miSpawnedBallCount;


	}





	
}