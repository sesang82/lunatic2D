#include "ssBaseScene.h"
#include "ssObject.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssCamera.h"
#include "ssRenderer.h"
#include "ssCameraScript.h"

namespace ss
{

	BaseScene::BaseScene()
		: mCamera(nullptr)
	{
	}
	BaseScene::~BaseScene()
	{
		//renderer::mainCamera = nullptr;
	}
	void BaseScene::Initialize()
	{

		// 카메라는 타이틀씬에서 한번 생성되면 그 이후에 그걸로 계속 돌려쓰는거임 
		// 이거 카메라 개별 속성 적용 안되서 일단 이 부분은 좀 생각해보고 하기 
		//if (renderer::cameras.empty())
		//{
		//	
		//	GameObject* MainCamera = new GameObject();
		//	MainCamera->SetName(L"MainCamera");

		//	AddGameObject(eLayerType::Camera, MainCamera);
		//	mCamera = MainCamera->AddComponent<Camera>();
		//	mCamera->TurnLayerMask(eLayerType::UI, false);
		//	MainCamera->AddComponent<CameraScript>();
		//	renderer::cameras.push_back(mCamera);
		//	renderer::mainCamera = mCamera; // 메인 카메라는 이렇게 꼭 담아두기!	

		//}


		Scene::Initialize();
	}
	void BaseScene::Update()
	{
		//LoadUI();
		Scene::Update();
	}
	void BaseScene::LateUpdate()
	{

		////UI Camera
		//{
		//	GameObject* camera = new GameObject();
		//	AddGameObject(eLayerType::Camera, camera);
		//	camera->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, -10.0f));
		//	Camera* cameraComp = camera->AddComponent<Camera>();
		//	//cameraComp->TurnLayerMask(eLayerType::Player, false);
		//	cameraComp->TurnLayerMask(eLayerType::Monster, false);
		//	cameraComp->TurnLayerMask(eLayerType::Boss, false);
		//	//camera->AddComponent<CameraScript>();
		//}

		Scene::LateUpdate();
	}
	void BaseScene::Render()
	{
		Scene::Render();
	}
	void BaseScene::LoadUI()
	{
		// UI는 메인 카메라가 따라가지 않게 해야함 (설정해주기)
		// **************

		// 플레이어 hp바
		{

			mPlayerHP = object::Instantiate<GameObject>(eLayerType::UI, L"PlayerHP_Bar");
			MeshRenderer* mr = mPlayerHP->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"PlayerHPBarMtrl"));

			mPlayerHP->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			mPlayerHP->GetComponent<Transform>()->SetScale(Vector3(165.0f, 14.0f, 0.0f));





		}

		// 플레이어 sp바
		{
			mPlayerSP = object::Instantiate<GameObject>(eLayerType::UI, L"PlayerSP_Bar");
			MeshRenderer* mr = mPlayerSP->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"PlayerSPBarMtrl"));

			mPlayerSP->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			mPlayerSP->GetComponent<Transform>()->SetScale(Vector3(165.0f, 14.0f, 0.0f));
		}

		// 일반 몬스터 hp바
		{
			mMonsterHP = object::Instantiate<GameObject>(eLayerType::UI, L"MonsterHP_Bar");
			MeshRenderer* mr = mMonsterHP->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"MonsterHPBarMtrl"));

			mMonsterHP->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			mMonsterHP->GetComponent<Transform>()->SetScale(Vector3(18.0f, 3.0f, 0.0f));
		}

		// 보스 hp바
		{
			mBossHP = object::Instantiate<GameObject>(eLayerType::UI, L"BossHP_Bar");
			MeshRenderer* mr = mBossHP->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"BossHPBarMtrl"));

			mBossHP->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			mBossHP->GetComponent<Transform>()->SetScale(Vector3(260.0f, 6.0f, 0.0f));
		}

	}
}