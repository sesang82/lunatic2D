#include "ssLibraryScene.h"
#include "ssInput.h"
#include "ssSceneManager.h"
#include "ssTransform.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssCamera.h"
#include "ssCameraScript.h"
#include "ssRenderer.h"
#include "ssPlayer.h"
#include "ssObject.h"
#include "ssBackground.h"
#include "ssPlayerScript.h"
#include "ssCollisionManager.h"
#include "ssPlatform.h"
#include "ssGroundScript.h"
#include "ssMonster.h"
#include "ssStoneEyeScript.h"
#include "ssStoneEye.h"

namespace ss
{
	LibraryScene::LibraryScene()
	{
	}
	LibraryScene::~LibraryScene()
	{
	}
	void LibraryScene::Initialize()
	{

		//BaseScene::Initialize();

		// 도서관
		{
			Background* Library = object::Instantiate<Background>(eLayerType::BG, L"libraryMap");
			Library->Initialize();
			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = Library->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"LibraryMtrl"));

			Library->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.f, 900.f));
			mr->Initialize(); // 트랜스폼 정보까지 합해져야하므로 transform 설정 한 후 해주기 
			//Library->GetComponent<Transform>()->SetScale(Vector3(1600.f, 900.f, 1.0f));
	
		}


		// 나무
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"libraryWeaponTree");
			bg->Initialize();
	
			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"WeaponTreeMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.0f, -115.f, 800.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(243.f, 283.f, 1.0f));

		}

		// 무기 활
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"SmallWeaponBow");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"w3Mtrl"));

			std::shared_ptr<Material> mtrl = mr->GetMaterial();
		

			bg->GetComponent<Transform>()->SetPosition(Vector3(-41.f, -170.f, 750.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(22.f, 24.f, 1.0f));

		}

		// 무기 총
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"SmallWeaponPistol");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"w2Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(-83.0f, -120.f, 700.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(22.f, 24.f, 1.0f));

		}


		// 무기 칼
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"SmallWeaponKnife");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"w1Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(-10.0f, -85.f, 650.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(23.f, 24.f, 1.0f));

		}
		
		// 캐릭터
		Player* player = object::Instantiate<Player>(eLayerType::Player, L"Player");
		player->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
		Transform* tr = player->GetComponent<Transform>();
		tr->SetPosition(Vector3(50.f, 100.f, 550.f));
//		
//				// 몬스터
		StoneEye* Stone = object::Instantiate<StoneEye>(eLayerType::Monster, L"StoneEye");
		Stone->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
		Transform* eyetr = Stone->GetComponent<Transform>();
		//eyetr->SetPosition(Vector3(-235.f, -350.f, 500.f));
		 eyetr->SetPosition(Vector3(-235.f, 70.f, 500.f));
	
//
//
//#pragma region Collision
		// =======================================
		// 충돌체 (바닥)
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
			tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(0.f, -307.f, 450.f));
			tr->SetScale(Vector3(835.f, 100.f, 1.f));

	
		}

		// 충돌체 ()
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
			tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(0.f, -307.f, 450.f));
			tr->SetScale(Vector3(835.f, 100.f, 1.f));


		}

//
		// ======= 나무
	// 충돌체 (나무에 있는 검) 
		{
			Platform* col = object::Instantiate<Platform>(eLayerType::Ground, L"col_TreeSword");
			col->Initialize();

			Transform* tr = col->GetComponent<Transform>();
			tr->SetPosition(Vector3(-42.f, -170.f, 450.f));
			tr->SetScale(Vector3(25.f, 25.f, 1.f));


		}


		// 충돌체 (나무 발판 - 검)
		{
			Platform* col = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
			col->Initialize();

			Transform* tr = col->GetComponent<Transform>();
			tr->SetPosition(Vector3(-38.f, -190.f, 450.f));
			tr->SetScale(Vector3(34.f, 2.f, 1.f));


		}



		// =======
	// 충돌체 (나무에 있는 총)
		{
			Platform* col = object::Instantiate<Platform>(eLayerType::Ground, L"col_TreePistol");
			col->Initialize();

			Transform* tr = col->GetComponent<Transform>();
			tr->SetPosition(Vector3(-87.f, -120.f, 450.f));
			tr->SetScale(Vector3(25.f, 25.f, 1.f));


		}


		// 충돌체 (나무 발판 - 총)
		{
			Platform* col = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
			col->Initialize();

			Transform* tr = col->GetComponent<Transform>();
			tr->SetPosition(Vector3(-80.f, -140.f, 450.f));
			tr->SetScale(Vector3(34.f, 2.f, 1.f));


		}

		// =======
		// 충돌체 (나무에 있는 화살)
		{
			Platform* col = object::Instantiate<Platform>(eLayerType::Ground, L"col_TreeBorrow");
			col->Initialize();

			Transform* tr = col->GetComponent<Transform>();
			tr->SetPosition(Vector3(-10.f, -86.f, 450.f));
			tr->SetScale(Vector3(25.f, 25.f, 1.f));


		}


		// 충돌체 (나무 발판 - 화살)
		{
			Platform* col = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
			col->Initialize();

			Transform* tr = col->GetComponent<Transform>();
			tr->SetPosition(Vector3(-12.f, -104.f, 450.f));
			tr->SetScale(Vector3(34.f, 2.f, 1.f));


		}

//		// =======================================
//		// 충돌체 (다음 씬 넘어가는)
		{
			Platform* col_Door = object::Instantiate<Platform>(eLayerType::Collision, L"L_col_Door");
			col_Door->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함

			Transform* tr = col_Door->GetComponent<Transform>();
			tr->SetPosition(Vector3(380.f, -225.f, 450.f));
			tr->SetScale(Vector3(40.f, 61.f, 1.f));

		}

		// 레이어 충돌을 담당하는 함수 
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, true);
		// 레이어 충돌을 담당하는 함수 
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Ground, true);
		
#pragma endregion

		//==== UI
	//플레이어 UI Frame
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

		 // 메인 카메라 생성 (반드시 씬마다 각기 다른 메인 카메라가 하나씩 있어야함!) 
		{
			GameObject* camera = new GameObject();
			AddGameObject(eLayerType::Camera, camera);

			mCamera = camera->AddComponent<Camera>();
			mCamera->TurnLayerMask(eLayerType::UI, false);

			CameraScript* camerscript= camera->AddComponent<CameraScript>();
			camerscript->SetTarget(player);
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
		}

	

	
	}

	void LibraryScene::Update()
	{
	
		BaseScene::Update();
	}
	void LibraryScene::LateUpdate()
	{
		BaseScene::LateUpdate();
	}
	void LibraryScene::Render()
	{
		BaseScene::Render();
	}
	void LibraryScene::OnEnter()
	{
		renderer::mainCamera = mCamera;

		/*LibraryScene::Initialize();

		CameraScript* camerscript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
		camerscript->SetTarget(this->mPlayer);*/
	}
	void LibraryScene::OnExit()
	{
	}
}