#include "ssEntryScene.h"
#include "ssInput.h"
#include "ssSceneManager.h"
#include "ssTransform.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssCamera.h"
#include "ssPlayer.h"
#include "ssCameraScript.h"
#include "ssRenderer.h"
#include "ssBackground.h"
#include "ssObject.h"
#include "ssCameraScript.h"


namespace ss
{

	
	EntryScene::EntryScene()
	{
	}
	EntryScene::~EntryScene()
	{
	}
	void EntryScene::Initialize()
	{


		


		// 배경 (뒤에 맨 마지막 나무 덩쿨 나온거 밑으로 땡기도록 수정하기) 
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"EntryBG");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"EntryMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.45f, 800.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1600.f, 896.f, 0.9f));
		}

		// 캐릭터
		mPlayer  = object::Instantiate<Player>(eLayerType::Player, L"Player");
		mPlayer->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
		Transform* tr = mPlayer->GetComponent<Transform>();
		tr->SetPosition(Vector3(-200.f, -362.f, 500.f));



		 //==== UI (UI 위치 기준은 이 씬에 있는 UI 기준으로 하기)
		 //플레이어 UI Frame (Hp바는 구조화할 때 UI Frame의 자식으로 설정해두기)
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent함수 자체가 반환형이 T*이라서 아래처럼 해서 mr에 받는게 가능한 것
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"PlayerFrameMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -200.f, 100.f));
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

			bg->GetComponent<Transform>()->SetPosition(Vector3(-103.f, -209.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(99.f, 8.f, 1.f));
		}



		// ==== 카메라
// 카메라 생성
		{
			GameObject* camera = new GameObject();
			AddGameObject(eLayerType::Camera, camera);

			mCamera = camera->AddComponent<Camera>();
			mCamera->TurnLayerMask(eLayerType::UI, false);
			mCamera->SetSize(2.3f);

			CameraScript* camerscript = camera->AddComponent<CameraScript>();
		

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

		BaseScene::Initialize();

	}
	void EntryScene::Update()
	{
		if (Input::GetKeyDown(eKeyCode::ENTER))
		{

			SceneManager::LoadScene(L"Stage1Scene");
		}

		BaseScene::Update();
	}
	void EntryScene::LateUpdate()
	{
		BaseScene::LateUpdate();
	}
	void EntryScene::Render()
	{
		BaseScene::Render();
	}
	void EntryScene::OnEnter()
	{
		renderer::mainCamera = mCamera;

		CameraScript* camerscript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
		camerscript->SetTarget(mPlayer);
	
	}
	void EntryScene::OnExit()
	{
	}
}