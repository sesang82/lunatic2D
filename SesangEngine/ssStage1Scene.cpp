#include "ssStage1Scene.h"

#include "ssInput.h"
#include "ssSceneManager.h"
#include "ssTransform.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssCamera.h"
#include "ssRenderer.h"
#include "ssBackground.h"
#include "ssObject.h"
#include "ssPlatform.h"
#include "ssGroundScript.h"
#include "ssCamera.h"
#include "ssCameraScript.h"
#include "ssPlayer.h"
#include "ssMonster.h"
#include "ssStoneEyeScript.h"
#include "ssCollisionManager.h"
#include "ssPlayerScript.h"
#include "ssStoneEye.h"
#include "ssStoneEyeScript.h"
#include "ssAttackCollider.h"
#include "ssPlayerAttackColScript.h"
#include "ssMonsterBar.h"
#include "ssSkeletonArcherScript.h"
#include "ssSkeletonLizardScript.h"
#include "ssWoodGolemScript.h"
#include "ssWolfScript.h"
#include "ssZombieScript.h"

namespace ss
{
	Stage1Scene::Stage1Scene()
	{
	}
	Stage1Scene::~Stage1Scene()
	{
	}
	void Stage1Scene::Initialize()
	{

		// 레이어 충돌을 담당하는 함수 

		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Ground, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Wall, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Collision, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Mon_Bullet, true);

		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Monster, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Boss, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Collision, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Etc, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Mon_Bullet, true);
		CollisionManager::SetLayer(eLayerType::Guard, eLayerType::Mon_Bullet, true);

		// 달 배경
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"DungeonMoon");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Moon2Mtrl"));


			bg->GetComponent<Transform>()->SetPosition(Vector3(-0.4f, 0.4f, 950.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1.6f, 1.6f, 1.0f));


		}

		// 나무1

		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"DungeonTree1");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Tree1Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.24f, 850.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1.6f, 2.f, 1.0f));
		}

		// 나무 2 
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"DungeonTree2");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Tree2Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.24f, 800.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1.6f, 2.f, 1.0f));
		}

		// 나무 3
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"DungeonTree3");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Tree3Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.24f, 750.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1.6f, 2.f, 1.0f));
		}

		// 나무 4 
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"DungeonTree4");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Tree4Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.24f, 700.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1.6f, 2.f, 1.0f));
		}

		// 맵 배경	
		//
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"DungeonMap");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Stage1Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.f, 650.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1600.f, 900.f, 1.0f));


		}


		 //캐릭터
		mPlayer = object::Instantiate<Player>(eLayerType::Player, L"Player");
		mPlayer->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함

		Transform* Playertr = mPlayer->GetComponent<Transform>();
		Playertr->SetPosition(Vector3(-400.f, -366.0f, 500.f)); // 이건 플레이어의 처음 위치임 ... 

		{
			// 스톤아이
			StoneEye* Stone = object::Instantiate<StoneEye>(eLayerType::Monster, L"StoneEye");
			Stone->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
			Transform* eyetr = Stone->GetComponent<Transform>();
			eyetr->SetPosition(Vector3(-200.f, -528.f, 500.f));

			StoneEyeScript* stonescript = Stone->GetComponent<StoneEyeScript>();
			stonescript->SetFirstPos(eyetr->GetPosition());

			// 몬스터 체력바 틀
			MonsterBar* mosnterbar = object::Instantiate<MonsterBar>(eLayerType::UI, L"StoneEyeBar");
			mosnterbar->Initialize();

			Transform* monsterbartr = mosnterbar->GetComponent<Transform>();
			mosnterbar->SetParent(Stone);
			monsterbartr->SetPosition(Vector3(-36.f, 38.f, 500.f));


			// 몬스터 HP
			Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::UI, L"StoneEyeHP");
			stoneHP->SetParent(Stone);
			Transform* stonehptr = stoneHP->GetComponent<Transform>();
			//stoneHP->SetOffset(-36.0f);
			stonehptr->SetPosition(Vector3(-36.f, 38.f, 500.f));
			stoneHP->Initialize();

		}

		{
			// 해골 Archer (궁수)
			Monster* Stone = object::Instantiate<Monster>(eLayerType::Monster, L"Archer");
			Stone->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
			Transform* eyetr = Stone->GetComponent<Transform>();
			eyetr->SetPosition(Vector3(-111.f, -166.f, 500.f));

			SkeletonArcherScript* ArcherScript = Stone->AddComponent<SkeletonArcherScript>();
			ArcherScript->SetFirstPos(eyetr->GetPosition());


			// ===== hp틀 그냥 넣지 말기 
			// 몬스터 HP (UI로 넣지말기. 그럼 UI카메라에 의해 플레이어 따라다니는 것처럼 움직임) 
			Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::Etc, L"ArcherHP");
			stoneHP->SetParent(Stone);

		
			Transform* stonehptr = stoneHP->GetComponent<Transform>();
			stonehptr->SetPosition(Vector3(-5.f, 38.f, 500.f));

			stoneHP->Initialize();

		}


		{
			// 해골 Lizard (근접 공격만 하는 애)
			Monster* Liazrd = object::Instantiate<Monster>(eLayerType::Monster, L"Lizard");
			Liazrd->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
			Transform* eyetr = Liazrd->GetComponent<Transform>();
			eyetr->SetPosition(Vector3(40.f, 185.f, 500.f));
		


			SkeletonLizardScript* ArcherScript = Liazrd->AddComponent<SkeletonLizardScript>();
			ArcherScript->SetFirstPos(eyetr->GetPosition());


			// ===== hp틀 그냥 넣지 말기 
			// 몬스터 HP (UI로 넣지말기. 그럼 UI카메라에 의해 플레이어 따라다니는 것처럼 움직임) 
			Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::Etc, L"LizardHP");
			stoneHP->SetParent(Liazrd);


			Transform* stonehptr = stoneHP->GetComponent<Transform>();
			stonehptr->SetPosition(Vector3(50.f, 25.f, 500.f));

			stoneHP->Initialize();

		}


		{
			// Wood Golem 몬스터 
			Monster* Golem = object::Instantiate<Monster>(eLayerType::Monster, L"WoodGolemObj");
			Golem->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
			Transform* eyetr = Golem->GetComponent<Transform>();
			eyetr->SetPosition(Vector3(-240.f, 70.f, 500.f));

			WoodGolemScript* ArcherScript = Golem->AddComponent<WoodGolemScript>();
			ArcherScript->SetFirstPos(eyetr->GetPosition());

			// 몬스터 체력바 틀
		/*	MonsterBar* mosnterbar = object::Instantiate<MonsterBar>(eLayerType::Etc, L"WoodHPBar");
			mosnterbar->Initialize();

			Transform* monsterbartr = mosnterbar->GetComponent<Transform>();
			mosnterbar->SetParent(Golem);
			monsterbartr->SetPosition(Vector3(50.f, 40.f, 500.f));*/


			// 몬스터 HP (UI로 넣지말기. 그럼 UI카메라에 의해 플레이어 따라다니는 것처럼 움직임) 
			Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::Etc, L"WoodHP");
			stoneHP->SetParent(Golem);
			stoneHP->SetOffset(100.f);


			Transform* stonehptr = stoneHP->GetComponent<Transform>();
			stonehptr->SetPosition(Vector3(50.f, 40.f, 500.f));

			stoneHP->Initialize();

		}

		{
			// 미니 wolf 몬스터 
			Monster* Wolf = object::Instantiate<Monster>(eLayerType::Monster, L"WolfObj");
			Wolf->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
			
			
			Transform* eyetr = Wolf->GetComponent<Transform>();
			eyetr->SetPosition(Vector3(10.f, 185.f, 500.f));
			//eyetr->SetPosition(Vector3(-350.f, -340.f, 500.f));

			WolfScript* wolfScript = Wolf->AddComponent<WolfScript>();
			wolfScript->SetFirstPos(eyetr->GetPosition());


			// 몬스터 체력바 틀
		/*	MonsterBar* mosnterbar = object::Instantiate<MonsterBar>(eLayerType::Etc, L"WolfHPBar");
			mosnterbar->Initialize();

			Transform* monsterbartr = mosnterbar->GetComponent<Transform>();
			mosnterbar->SetParent(Wolf);
			monsterbartr->SetPosition(Vector3(50.f, 25.f, 500.f));*/

			// ===== hp틀 그냥 넣지 말기 
			// 몬스터 HP (UI로 넣지말기. 그럼 UI카메라에 의해 플레이어 따라다니는 것처럼 움직임) 
			Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::Etc, L"WolfHP");
			stoneHP->SetParent(Wolf);
		
			Transform* stonehptr = stoneHP->GetComponent<Transform>();
			stonehptr->SetPosition(Vector3(50.f, 25.f, 500.f));

			stoneHP->Initialize();

		}

		
		//	// Zombi 몬스터 (해당 씬에는 더 이상 둘 곳이 없어서 보류)
		//	Monster* Wolf = object::Instantiate<Monster>(eLayerType::Monster, L"ZombiObj");
		//	Wolf->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


		//	Transform* eyetr = Wolf->GetComponent<Transform>();
		//	//eyetr->SetPosition(Vector3(10.f, 185.f, 500.f));
		//	eyetr->SetPosition(Vector3(-350.f, -340.f, 500.f));

		//	ZombieScript* wolfScript = Wolf->AddComponent<ZombieScript>();
		//	wolfScript->SetFirstPos(eyetr->GetPosition());


		//	// 몬스터 체력바 틀
		///*	MonsterBar* mosnterbar = object::Instantiate<MonsterBar>(eLayerType::Etc, L"WolfHPBar");
		//	mosnterbar->Initialize();

		//	Transform* monsterbartr = mosnterbar->GetComponent<Transform>();
		//	mosnterbar->SetParent(Wolf);
		//	monsterbartr->SetPosition(Vector3(50.f, 25.f, 500.f));*/

		//	// ===== hp틀 그냥 넣지 말기 
		//	// 몬스터 HP (UI로 넣지말기. 그럼 UI카메라에 의해 플레이어 따라다니는 것처럼 움직임) 
		//	Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::Etc, L"ZombiHP");
		//	stoneHP->SetParent(Wolf);

		//	Transform* stonehptr = stoneHP->GetComponent<Transform>();
		//	stonehptr->SetPosition(Vector3(50.f, 25.f, 500.f));

		//	stoneHP->Initialize();

		//}
			// =====
		// 테스트용으로 좋은 위치 			eyetr->SetPosition(Vector3(-350.f, -340.f, 500.f));


		////==================
	   // Collision


   // 충돌체 (왼쪽 바닥)
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
		

			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-300.f, -487.f, 500.f));
			tr->SetScale(Vector3(416.f, 200.f, 1.f));


		}

		// 충돌체 (왼쪽 바닥)
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함

			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-80.f, -338.f, 500.f));
			tr->SetScale(Vector3(95.f, 95.f, 1.f));


		}

		// 충돌체 (맨 아래 오른쪽 바닥 )
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
	

			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(43.f, -396.f, 500.f));
			tr->SetScale(Vector3(150.f, 150.f, 1.f));


		}


		// 충돌체 (2층 통나무 바닥  )
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-111.f, -198.f, 500.f));
			tr->SetScale(Vector3(222.f, 5.f, 1.f));


		}


		// 충돌체 (3층 통나무 바닥  )
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-240.f, -68.f, 500.f));
			tr->SetScale(Vector3(220.f, 5.f, 1.f));


		}

		// 충돌체 (3층 통나무 옆 바닥  )
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-384.f, -98.f, 500.f));
			tr->SetScale(Vector3(62.f, 5.f, 1.f));


		}



		// 충돌체 (4층 통나무 바닥  )
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-240.f, 28.f, 500.f));
			tr->SetScale(Vector3(222.f, 5.f, 1.f));


		}


		// 충돌체 (5층 통나무 바닥  )
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(70.f, 158.f, 500.f));
			tr->SetScale(Vector3(520.f, 5.f, 1.f));


		}



	

		// ======================================

		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"PlayerFrameMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -195.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(1.5f, 0.3f);
			bg->GetComponent<Transform>()->SetScale(Vector3(308.f, 57.f, 1.f));
		}

		//플레이어 HP바
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"PlayerBarMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(-103.f, -205.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(99.f, 8.f, 1.f));
		}

		//플레이어 SP바
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"PlayerSPBarMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(103.f, -205.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(99.f, 8.f, 1.f));
		}




		// === 카메라

	 // 메인 카메라 생성 (반드시 씬마다 각기 다른 메인 카메라가 하나씩 있어야함!) 
		{
			GameObject* camera = new GameObject();
			AddGameObject(eLayerType::Camera, camera);

			mCamera = camera->AddComponent<Camera>();
			mCamera->TurnLayerMask(eLayerType::UI, false);

			CameraScript* camerscript = camera->AddComponent<CameraScript>();

		}

		//UI Camera
		{
			GameObject* UIcamera = new GameObject();
			AddGameObject(eLayerType::Camera, UIcamera);
			UIcamera->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 970.0f));
			Camera* cameraComp0 = UIcamera->AddComponent<Camera>();
			cameraComp0->DisableLayerMasks();
			cameraComp0->TurnLayerMask(eLayerType::UI);
			renderer::cameras.push_back(cameraComp0);
		}



		Scene::Initialize();
	}
	void Stage1Scene::Update()
	{
		if (Input::GetKeyDown(eKeyCode::ENTER))
		{

			SceneManager::LoadScene(L"Boss1Scene");
		}


		Scene::Update();
	}
	void Stage1Scene::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Stage1Scene::Render()
	{
		Scene::Render();
	}
	void Stage1Scene::OnEnter()
	{
		renderer::mainCamera = mCamera;
		mCamera->SetSize(2.3f);

		CameraScript* camerscript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
		camerscript->SetTarget(mPlayer);
	}
	void Stage1Scene::OnExit()
	{
	}
}