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

		// ī�޶�� Ÿ��Ʋ������ �ѹ� �����Ǹ� �� ���Ŀ� �װɷ� ��� �������°��� 
		// �̰� ī�޶� ���� �Ӽ� ���� �ȵǼ� �ϴ� �� �κ��� �� �����غ��� �ϱ� 
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
		//	renderer::mainCamera = mCamera; // ���� ī�޶�� �̷��� �� ��Ƶα�!	

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
		// UI�� ���� ī�޶� ������ �ʰ� �ؾ��� (�������ֱ�)
		// **************

		// �÷��̾� hp��
		{

			mPlayerHP = object::Instantiate<GameObject>(eLayerType::UI, L"PlayerHP_Bar");
			MeshRenderer* mr = mPlayerHP->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"PlayerHPBarMtrl"));

			mPlayerHP->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			mPlayerHP->GetComponent<Transform>()->SetScale(Vector3(165.0f, 14.0f, 0.0f));





		}

		// �÷��̾� sp��
		{
			mPlayerSP = object::Instantiate<GameObject>(eLayerType::UI, L"PlayerSP_Bar");
			MeshRenderer* mr = mPlayerSP->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"PlayerSPBarMtrl"));

			mPlayerSP->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			mPlayerSP->GetComponent<Transform>()->SetScale(Vector3(165.0f, 14.0f, 0.0f));
		}

		// �Ϲ� ���� hp��
		{
			mMonsterHP = object::Instantiate<GameObject>(eLayerType::UI, L"MonsterHP_Bar");
			MeshRenderer* mr = mMonsterHP->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"MonsterHPBarMtrl"));

			mMonsterHP->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			mMonsterHP->GetComponent<Transform>()->SetScale(Vector3(18.0f, 3.0f, 0.0f));
		}

		// ���� hp��
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