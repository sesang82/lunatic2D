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

		// ������
		{
			Background* Library = object::Instantiate<Background>(eLayerType::BG, L"libraryMap");
			Library->Initialize();
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = Library->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"LibraryMtrl"));

			Library->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.f, 900.f));
			mr->Initialize(); // Ʈ������ �������� ���������ϹǷ� transform ���� �� �� ���ֱ� 
			//Library->GetComponent<Transform>()->SetScale(Vector3(1600.f, 900.f, 1.0f));
	
		}


		// ����
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"libraryWeaponTree");
			bg->Initialize();
	
			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"WeaponTreeMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.0f, -115.f, 800.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(243.f, 283.f, 1.0f));

		}

		// ���� Ȱ
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"SmallWeaponBow");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"w3Mtrl"));

			std::shared_ptr<Material> mtrl = mr->GetMaterial();
		

			bg->GetComponent<Transform>()->SetPosition(Vector3(-41.f, -170.f, 750.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(22.f, 24.f, 1.0f));

		}

		// ���� ��
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"SmallWeaponPistol");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"w2Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(-83.0f, -120.f, 700.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(22.f, 24.f, 1.0f));

		}


		// ���� Į
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"SmallWeaponKnife");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"w1Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(-10.0f, -85.f, 650.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(23.f, 24.f, 1.0f));

		}
		
		// ĳ����
		Player* player = object::Instantiate<Player>(eLayerType::Player, L"Player");
		player->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
		Transform* tr = player->GetComponent<Transform>();
		tr->SetPosition(Vector3(50.f, 100.f, 550.f));
//		
//				// ����
		StoneEye* Stone = object::Instantiate<StoneEye>(eLayerType::Monster, L"StoneEye");
		Stone->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
		Transform* eyetr = Stone->GetComponent<Transform>();
		//eyetr->SetPosition(Vector3(-235.f, -350.f, 500.f));
		 eyetr->SetPosition(Vector3(-235.f, 70.f, 500.f));
	
//
//
//#pragma region Collision
		// =======================================
		// �浹ü (�ٴ�)
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
			tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(0.f, -307.f, 450.f));
			tr->SetScale(Vector3(835.f, 100.f, 1.f));

	
		}

		// �浹ü ()
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
			tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(0.f, -307.f, 450.f));
			tr->SetScale(Vector3(835.f, 100.f, 1.f));


		}

//
		// ======= ����
	// �浹ü (������ �ִ� ��) 
		{
			Platform* col = object::Instantiate<Platform>(eLayerType::Ground, L"col_TreeSword");
			col->Initialize();

			Transform* tr = col->GetComponent<Transform>();
			tr->SetPosition(Vector3(-42.f, -170.f, 450.f));
			tr->SetScale(Vector3(25.f, 25.f, 1.f));


		}


		// �浹ü (���� ���� - ��)
		{
			Platform* col = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
			col->Initialize();

			Transform* tr = col->GetComponent<Transform>();
			tr->SetPosition(Vector3(-38.f, -190.f, 450.f));
			tr->SetScale(Vector3(34.f, 2.f, 1.f));


		}



		// =======
	// �浹ü (������ �ִ� ��)
		{
			Platform* col = object::Instantiate<Platform>(eLayerType::Ground, L"col_TreePistol");
			col->Initialize();

			Transform* tr = col->GetComponent<Transform>();
			tr->SetPosition(Vector3(-87.f, -120.f, 450.f));
			tr->SetScale(Vector3(25.f, 25.f, 1.f));


		}


		// �浹ü (���� ���� - ��)
		{
			Platform* col = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
			col->Initialize();

			Transform* tr = col->GetComponent<Transform>();
			tr->SetPosition(Vector3(-80.f, -140.f, 450.f));
			tr->SetScale(Vector3(34.f, 2.f, 1.f));


		}

		// =======
		// �浹ü (������ �ִ� ȭ��)
		{
			Platform* col = object::Instantiate<Platform>(eLayerType::Ground, L"col_TreeBorrow");
			col->Initialize();

			Transform* tr = col->GetComponent<Transform>();
			tr->SetPosition(Vector3(-10.f, -86.f, 450.f));
			tr->SetScale(Vector3(25.f, 25.f, 1.f));


		}


		// �浹ü (���� ���� - ȭ��)
		{
			Platform* col = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
			col->Initialize();

			Transform* tr = col->GetComponent<Transform>();
			tr->SetPosition(Vector3(-12.f, -104.f, 450.f));
			tr->SetScale(Vector3(34.f, 2.f, 1.f));


		}

//		// =======================================
//		// �浹ü (���� �� �Ѿ��)
		{
			Platform* col_Door = object::Instantiate<Platform>(eLayerType::Collision, L"L_col_Door");
			col_Door->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������

			Transform* tr = col_Door->GetComponent<Transform>();
			tr->SetPosition(Vector3(380.f, -225.f, 450.f));
			tr->SetScale(Vector3(40.f, 61.f, 1.f));

		}

		// ���̾� �浹�� ����ϴ� �Լ� 
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, true);
		// ���̾� �浹�� ����ϴ� �Լ� 
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Ground, true);
		
#pragma endregion

		//==== UI
	//�÷��̾� UI Frame
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"PlayerFrameMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -195.f, 100.f));
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

			bg->GetComponent<Transform>()->SetPosition(Vector3(-103.f, -205.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(99.f, 8.f, 1.f));
		}

		 // ���� ī�޶� ���� (�ݵ�� ������ ���� �ٸ� ���� ī�޶� �ϳ��� �־����!) 
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