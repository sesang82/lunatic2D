#include "ssEntranceScene.h"
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
#include "ssPlayer.h"
#include "ssCollisionManager.h"
#include "ssGroundScript.h"
#include "ssPlatform.h"
#include "ssCameraScript.h"
#include "ssStoneEyeScript.h"
#include "ssMonster.h"

namespace ss
{
	EntranceScene::EntranceScene()
	{
	}
	EntranceScene::~EntranceScene()
	{
	}
	void EntranceScene::Initialize()
	{
	  

		// 배경 맵 설정 
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"EntranceMap");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"EntranceMtrl"));
			//mr->Initialize();

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.55f, 1000.0f));
			bg->GetComponent<Transform>()->SetScale(1600.f, 896.f, 1.f);
		}

		// 캐릭터
		Player* player = object::Instantiate<Player>(eLayerType::Player, L"player");
		player->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
		Transform* tr = player->GetComponent<Transform>();
		tr->SetPosition(Vector3(-588.f, 0.f, 500.f));


		//				// 몬스터
		Monster* StoneEye = object::Instantiate<Monster>(eLayerType::Monster, L"StoneEye");
		StoneEye->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
		Transform* eyetr = StoneEye->GetComponent<Transform>();
		eyetr->SetPosition(Vector3(-235.f, 180.f, 0.8f));
		StoneEye->AddComponent<StoneEyeScript>();

		// 충돌체 (왼쪽 바닥)
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
			col_Floor->AddComponent<GroundScript>();

			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetScale(Vector3(416.f, 200.f, 1.f));
			tr->SetPosition(Vector3(-400.f, -292.f, 1.f));

		}

		// 충돌체 (오른쪽 바닥)
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
			col_Floor->AddComponent<GroundScript>();

			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetScale(Vector3(1000.f, 200.f, 1.f));
			tr->SetPosition(Vector3(200.f, -419.f, 1.f));

		}

		// 레이어 충돌을 담당하는 함수 
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, true);




		// 포탈
		// 두번째 obj 생성
		//{
		//	GameObject* bg = new GameObject();
		//	AddGameObject(eLayerType::BG, bg);
		//	// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
		//	MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
		//	mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		//	mr->SetMaterial(Resources::Find<Material>(L"PotalMtrl"));

		//	bg->GetComponent<Transform>()->SetPosition(Vector3(0.35f, -0.31f, 0.9f));
		//	//bg->GetComponent<Transform>()->SetVecrtexScale(0.8f, 0.2f);
		//	//bg->GetComponent<Transform>()->SetRotation(Vector3(0.0f, 0.0f, 45.0f));
		//	bg->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 1.f);
		//}

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

		// === 카메라

	// 카메라 생성
		{
			GameObject* camera = new GameObject();
			AddGameObject(eLayerType::Camera, camera);

			mCamera = camera->AddComponent<Camera>();
			mCamera->TurnLayerMask(eLayerType::UI, false);

			CameraScript* camerscript = camera->AddComponent<CameraScript>();
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


		Scene::Initialize();
	}
	void EntranceScene::Update()
	{
		if (Input::GetKeyDown(eKeyCode::ENTER))
		{

			SceneManager::LoadScene(L"EntryScene");
		}


		Scene::Update();
	}
	void EntranceScene::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void EntranceScene::Render()
	{
		Scene::Render();
	}
	void EntranceScene::OnEnter()
	{
		renderer::mainCamera = mCamera;
		mCamera->SetSize(2.f);
	}
	void EntranceScene::OnExit()
	{
	}
}