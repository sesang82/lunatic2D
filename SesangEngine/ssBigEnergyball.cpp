#include "ssBigEnergyball.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssTime.h"
#include "ssSmallEnergyballScript.h"
#include "ssAnimator.h"
#include "ssobject.h"
#include "ssGoddnessScript.h"
#include "ssPlayer.h"
#include "ssBigEnergyballScript.h"


namespace ss
{
	int BigEnergyball::miSpawnedBallCount = 0;
	bool BigEnergyball::mbSpawn = false;

	BigEnergyball::BigEnergyball()
	
	{
	

	}
	BigEnergyball::~BigEnergyball()
	{
	}
	void BigEnergyball::Initialize()
	{
		mTransform = GetComponent<Transform>();
		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mAnimator = AddComponent<Animator>();


		mTransform->SetScale(Vector3(219.f, 251.f, 1.0f));
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"BigEnergyBulletMtrl"));

		Collider2D* col = AddComponent<Collider2D>();
		col->SetSize(Vector2(1.f, 1.f));
		col->SetName(L"BigEnergyballCol");


		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_B_noParrying_Spawn_Effect");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_B_noParrying_Energying_Effect");
		std::shared_ptr<ss::graphics::Texture> Image3 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_B_noParrying_End_Effect");

		mAnimator->Create(L"Energyball_B_NoParrying_Spawn", Image1, Vector2(0.f, 0.f), Vector2(144.f, 135.f), 6, Vector2(144.f, 135.f), Vector2::Zero, 0.08f);
		mAnimator->Create(L"Energyball_B_NoParrying_Energying", Image2, Vector2(0.f, 0.f), Vector2(144.f, 135.f), 6, Vector2(144.f, 135.f), Vector2::Zero, 0.08f);
		mAnimator->Create(L"Energyball_B_NoParrying_End", Image3, Vector2(0.f, 0.f), Vector2(219.f, 251.f), 6, Vector2(219.f, 251.f), Vector2::Zero, 0.08f);


		mAnimator->PlayAnimation(L"Energyball_B_NoParrying_Spawn", false); // trigger 완성하면 지우기 

		AddComponent<BigEnergyballScript>();


		Bullet::Initialize();
	}
	void BigEnergyball::Update()
	{
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			mAnimator->PlayAnimation(L"Energyball_B_NoParrying_Energying", true); // trigger 완성하면 지우기 

			if (!mbSpawn)
			{
				mbSpawn = true; // 다음 상태로 넘어갈 때  false처리 해주기 


				CreateBalls();

			}
		}

		mfTime += Time::DeltaTime();

		if (mfTime > 2.2f)
		{
			if (GetName() == L"BigEnergyballs" && mbSpawn)
			{
				AddComponent<Collider2D>();
			}

			float speed = 30.f;


			Transform* tr = GetComponent<Transform>();
			Vector3 Pos = tr->GetPosition();

			Pos += GetDir() * speed * Time::DeltaTime();
			tr->SetPosition(Pos);

			//mfTime = 0.0f; => 0으로 초기화하는거는 다른 상태로 넘어갈 떄 그냥 하자 (여러 객체를 써서 이 떄문에 문제가 되는 듯)
		}

		Bullet::Update();
	}
	void BigEnergyball::LateUpdate()
	{
		Bullet::LateUpdate();
	}
	void BigEnergyball::Render()
	{
		Bullet::Render();
	}

	void BigEnergyball::CreateBalls()
	{
		Vector3 mDirections[] =
		{
			Vector3(-1, 1, 0),
			Vector3(1, 1, 0),
			Vector3(-1, -1, 0),
			Vector3(1, -1, 0)
		};


		const int numBalls = 4;  // 한번에 생성할 에너지볼의 개수

		for (int i = 0; i < numBalls; ++i)
		{
			// 에너지볼 생성 코드
			BigEnergyball* ball = object::Instantiate<BigEnergyball>
				(Vector3(2.3f, -25.f, mTransform->GetPosition().z), eLayerType::Collision, L"BigEnergyballs"); // 필요한 다른 파라미터들도 채워야 합니다.

			ball->Initialize();

			// 해당 에너지볼에 방향 설정
			ball->SetDir(mDirections[i]);
		}


	}

}