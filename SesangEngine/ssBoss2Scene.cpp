#include "ssBoss2Scene.h"
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
#include "ssAnimator.h"
#include "ssBigWolfScript.h"
#include "ssMonster.h"
#include "ssCameraScript.h"
#include "ssPlayer.h"
#include "ssPlayerScript.h"
#include "ssRigidbody2D.h"
#include "ssPlatform.h"
#include "ssTriggerScript.h"
#include "ssParalloxScript.h"
#include "ssCollisionManager.h"
#include "ssProgressbar.h"
#include "ssEffect.h"
#include "ssGoddnessScript.h"
#include "ssWallScript.h"

namespace ss
{

	Boss2Scene::Boss2Scene()
		
	{
	}
	Boss2Scene::~Boss2Scene()
	{
	}
	void Boss2Scene::Initialize()
	{// =========== 캐릭터들






		//// ====== 배경
		//// 둥근 달

		Background* bg = object::Instantiate<Background>(eLayerType::BG, L"BG_BackGround_0");
		bg->Initialize();

		MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
		mr->SetMaterial(Resources::Find<Material>(L"Boss2_BG_0_Mtrl"));

		bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.f, 850.f));
		bg->GetComponent<Transform>()->SetScale(Vector3(1024.f, 900.f, 1.0f));

		ParalloxScript* parallox = bg->AddComponent<ParalloxScript>();
		parallox->SetPlayer(mPlayer);
		parallox->SetSpeed(Vector2(0.18f, 0.18f));

		parallox->SetMinX(-31.0f);
		parallox->SetMaxX(33.0f);

		parallox->SetMinY(-30.0f);
		parallox->SetMaxY(30.0f);


		// 나무1

		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"BG_BackGround_1");
			bg->Initialize();


			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Boss2_BG_0_Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.0f, 800.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1024.f, 867.f, 1.0f));

			ParalloxScript* parallox = bg->AddComponent<ParalloxScript>();
			parallox->SetPlayer(mPlayer);
			parallox->SetSpeed(Vector2(0.3f, 0.3f));

			parallox->SetMinX(-48.0f);
			parallox->SetMaxX(48.0f);

			parallox->SetMinY(-150.0f);
			parallox->SetMaxY(-150.0f);

		}

		// 나무 2 
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"BG_BackGround_2");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Boss2_BG_2_Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.f, 750.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1024.f, 286.f, 1.0f));



			ParalloxScript* parallox = bg->AddComponent<ParalloxScript>();
			parallox->SetPlayer(mPlayer);
			parallox->SetSpeed(Vector2(0.5f, 0.f));

			parallox->SetMinX(-30.0f);
			parallox->SetMaxX(30.0f);

			parallox->SetMinY(-150.0f);
			parallox->SetMaxY(-140.0f);


		}



		// 나무 4
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"BG_BackGround_4");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Boss2_BG_4_Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -100.f, 700.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1024.f, 375.f, 1.0f));


			ParalloxScript* parallox = bg->AddComponent<ParalloxScript>();
			parallox->SetPlayer(mPlayer);
			parallox->SetSpeed(Vector2(0.55f, 0.f));

			parallox->SetMinX(-20.0f);
			parallox->SetMaxX(80.0f);

			parallox->SetMinY(-150.0f);
			parallox->SetMaxY(-140.0f);


		}

		// 나무 3
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"BG_BackGround_3");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Boss2_BG_3_Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -26.f, 700.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(698.f, 394.f, 1.0f));


	


		}
		
		{
			Background * bg = object::Instantiate<Background>(eLayerType::BG, L"Boss2_Tile");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Boss2_Tile_Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.f, 100.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1600.f, 896.f, 1.0f));

		}

		// 횃불

		
			Background* Fire1 = object::Instantiate<Background>(eLayerType::BG, L"Boss2_Fire");
			Fire1->Initialize();
			Fire1->GetComponent<Transform>()->SetPosition(Vector3(-243.f, 0.f, 700.f));
			Fire1->GetComponent<Transform>()->SetScale(Vector3(48.f, 64.f, 1.0f));


			
			
			Background* Fire2 = object::Instantiate<Background>(eLayerType::BG, L"Boss2_Fire");
			Fire2->Initialize();
			Fire2->GetComponent<Transform>()->SetPosition(Vector3(-81.f, 0.f, 700.f));
			Fire2->GetComponent<Transform>()->SetScale(Vector3(48.f, 64.f, 1.0f));

	

			Background* Fire3 = object::Instantiate<Background>(eLayerType::BG, L"Boss2_Fire");
			Fire3->Initialize();
			Fire3->GetComponent<Transform>()->SetPosition(Vector3(81.f, 0.f, 700.f));
			Fire3->GetComponent<Transform>()->SetScale(Vector3(48.f, 64.f, 1.0f));

	
			Background* Fire4 = object::Instantiate<Background>(eLayerType::BG, L"Boss2_Fire");
			Fire4->Initialize();
			Fire4->GetComponent<Transform>()->SetPosition(Vector3(243.f, 0.f, 700.f));
			Fire4->GetComponent<Transform>()->SetScale(Vector3(48.f, 64.f, 1.0f));

		





		// ==== 캐릭터
			//// 보스
		mBoss1 = object::Instantiate<Monster>(eLayerType::Boss, L"B_GodObj");
		mBoss1->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


		Transform* eyetr = mBoss1->GetComponent<Transform>();
		eyetr->SetPosition(Vector3(0.f, -87.f, 500.f));

		GoddnessScript* wolfScript = mBoss1->AddComponent<GoddnessScript>();
		wolfScript->SetFirstPos(eyetr->GetPosition());

	




		//캐릭터
		mPlayer = object::Instantiate<Player>(eLayerType::Player, L"Player");
		mPlayer->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함

		Transform* Playertr = mPlayer->GetComponent<Transform>();
		Playertr->SetPosition(Vector3(-290.f, -208.0f, 400.f));
		PlayerScript* playerScript = mPlayer->GetComponent<PlayerScript>();
		playerScript->SetMonster(mBoss1);


		wolfScript->SetPlayer(mPlayer);



		// ================ 벽, 천장 
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Wall, L"Wall_R_Obj");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(334.f, -38.f, 500.f));
			tr->SetScale(Vector3(30.f, 389.f, 1.f));

			WallScript* wallscript  = col_Floor->AddComponent<WallScript>();
			wallscript->SetPlayer(mPlayer);


		}

		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Wall, L"Wall_L_Obj");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-300.f, -487.f, 500.f));
			tr->SetScale(Vector3(416.f, 200.f, 1.f));


			WallScript* wallscript = col_Floor->AddComponent<WallScript>();
			wallscript->SetPlayer(mPlayer);


		}

		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Wall, L"Wall_Up_Obj");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-300.f, -487.f, 500.f));
			tr->SetScale(Vector3(416.f, 200.f, 1.f));


			WallScript* wallscript = col_Floor->AddComponent<WallScript>();
			wallscript->SetPlayer(mPlayer);


		}



		//==== UI
//플레이어 UI Frame
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"PlayerFrameMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -194.f, 400.f));
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

			bg->GetComponent<Transform>()->SetPosition(Vector3(-103.f, -204.f, 350.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(99.f, 8.f, 1.f));

		}


		// 보스 HP Frame
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"BossHpFrameMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 198.f, 300.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(1.5f, 0.3f);
			bg->GetComponent<Transform>()->SetScale(Vector3(266.f, 28.f, 1.f));
			bg->SetName(L"BossHPFrame");


		}


		// 보스 hp바
		{
			Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::UI, L"Boss_Bar2");
			stoneHP->SetOwner(mBoss1);

			Transform* stonehptr = stoneHP->GetComponent<Transform>();
			stonehptr->SetPosition(Vector3(-50.f, 196.f, 250.f));

			stoneHP->Initialize();

		

		}

		//플레이어 오버로드 바
		{
			Progressbar* overloadBar = object::Instantiate<Progressbar>(eLayerType::UI, L"overloadBar");
			overloadBar->SetOwner(mPlayer);

			Transform* tr = overloadBar->GetComponent<Transform>();
			tr->SetPosition(Vector3(0.f, -218.f, 95.f));

			overloadBar->Initialize();

		}


		//플레이어 오버로드 빈 bar 
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"Overload_BarMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -218.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(48.f, 4.f, 1.f));
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


		//// ===== 충돌체
		//// 몬스터 소환하는 충돌체

		//Platform* col_Spawn = object::Instantiate<Platform>(eLayerType::Collision, L"col_SpawnBoss2");
		//col_Spawn->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함

		//Transform* tr = col_Spawn->GetComponent<Transform>();
		//tr->SetPosition(Vector3(-240.f, -150.0f, 500.f));
		//tr->SetScale(Vector3(20.f, 20.f, 1.f));

		//TriggerScript* script = col_Spawn->AddComponent<TriggerScript>();
		//script->SetMonster(mBoss1);
		//script->SetPlayer(mPlayer);



		//// 보스 패턴 시작하는 충돌체 

		//Platform* col_startBoss = object::Instantiate<Platform>(eLayerType::Collision, L"col_startBoss1StageObj");
		//col_startBoss->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함

		//Transform* spawntr = col_startBoss->GetComponent<Transform>();
		//spawntr->SetPosition(Vector3(-200.f, -298.0f, 500.f));
		//spawntr->SetScale(Vector3(20.f, 20.f, 1.f));

		//playerScript->SetBossStartColObj(col_startBoss);





		// 바닥
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(0.f, -275.f, 500.f));
			tr->SetScale(Vector3(640.f, 88.f, 1.f));


		}

		// === 카메라

		// 카메라 생성
		{
			GameObject* camera = new GameObject();
			AddGameObject(eLayerType::Camera, camera);
			camera->GetComponent<Transform>()->SetPosition(Vector3(-80.f, -210.f, 980.f));

			mCamera = camera->AddComponent<Camera>();
			mCamera->TurnLayerMask(eLayerType::UI, false);
			mCamera->SetSize(2.3f); // 메인카메라만 2.3f로 설정 

			camera->AddComponent<CameraScript>();

			mBoss1->GetComponent<MonsterScript>()->SetMainCamera(camera);
		
			//script->SetCamera(camera); // trigger 스크립트에서 메인 카메라 갖다 쓰기 위해 갖고 옴 


		}

		//UI Camera
		{
			GameObject* UIcamera = new GameObject();
			AddGameObject(eLayerType::Camera, UIcamera);
			UIcamera->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 980.0f));
			Camera* cameraComp0 = UIcamera->AddComponent<Camera>();
			cameraComp0->DisableLayerMasks();
			cameraComp0->TurnLayerMask(eLayerType::UI);
			renderer::cameras.push_back(cameraComp0);

			//script->SetUICamera(UIcamera); // trigger 스크립트에서 메인 카메라 갖다 쓰기 위해 갖고 옴 

			mBoss1->GetComponent<MonsterScript>()->SetUICamera(UIcamera);
		}




		Scene::Initialize();
	}
	void Boss2Scene::Update()
	{
		Scene::Update();
	}
	void Boss2Scene::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Boss2Scene::Render()
	{
		Scene::Render();
	}

	void Boss2Scene::OnEnter()
	{
		// 메인 카메라로 설정해준다. 
		renderer::mainCamera = mCamera;

		CameraScript* camerscript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
		camerscript->SetTarget(mPlayer);
	}

	void Boss2Scene::OnExit()
	{
	}



}