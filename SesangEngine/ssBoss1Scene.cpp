#include "ssBoss1Scene.h"
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

namespace ss
{
	Boss1Scene::Boss1Scene()
	{
	}
	Boss1Scene::~Boss1Scene()
	{
	}
	void Boss1Scene::Initialize()
	{

		// ���� ��
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"Boss1_Map");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Boss1Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.f, 900.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1600.f, 896.f, 1.0f));
		}

		// �ձ� ��
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"Boss1_Moon");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Moon2Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.f, 850.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1024.f, 900.f, 1.0f));
		}

		// ����1

		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::BG, bg);
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"Tree1Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.0f, 800.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1024.f, 618.f, 1.0f));
		}

		// ���� 2 
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::BG, bg);
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"Tree2Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.24f, 750.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1.6f, 2.f, 1.0f));
		}

		// ���� 3
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::BG, bg);
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"Tree3Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.24f, 700.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1.6f, 2.f, 1.0f));
		}

		// ���� 4 
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::BG, bg);
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"Tree4Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.24f, 650.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1.6f, 2.f, 1.0f));
		}



	//==== UI
	//�÷��̾� UI Frame
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"PlayerFrameMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -194.f, 400.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(1.5f, 0.3f);
			bg->GetComponent<Transform>()->SetScale(Vector3(308.f, 57.f, 1.f));
		}

		//�÷��̾� HP��
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"PlayerBarMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(-103.f, -204.f, 350.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(99.f, 8.f, 1.f));
		}


		// ���� HP Frame
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"BossHpFrameMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 198.f, 300.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(1.5f, 0.3f);
			bg->GetComponent<Transform>()->SetScale(Vector3(266.f, 28.f, 1.f));
		}


		// ���� hp��
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"BossHPBarMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 196.f, 250.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(1.5f, 0.3f);
			bg->GetComponent<Transform>()->SetScale(Vector3(262.f, 6.f, 1.f));
		}



		// === ī�޶�

		// ī�޶� ����
		{
			GameObject* camera = new GameObject();
			AddGameObject(eLayerType::Camera, camera);
			camera->GetComponent<Transform>()->SetPosition(Vector3(-80.f, -210.f, 980.f));
			Camera* cameraComp = camera->AddComponent<Camera>();
			cameraComp->TurnLayerMask(eLayerType::UI, false);
			

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
	void Boss1Scene::Update()
	{
		Scene::Update();
	}
	void Boss1Scene::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Boss1Scene::Render()
	{
		Scene::Render();
	}
	void Boss1Scene::OnEnter()
	{
	}
	void Boss1Scene::OnExit()
	{
	}
}