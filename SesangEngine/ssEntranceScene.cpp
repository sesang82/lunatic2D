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
#include "ssProgressbar.h"
#include "ssItem.h"
#include "ssItemScript.h"
#include "ssUIFrameScript.h"
#include "ssWallScript.h"

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

		Scene::Initialize();

		// ��� �� ���� 
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"EntranceMap");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"EntranceMtrl"));
			//mr->Initialize();

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.55f, 1000.0f));
			bg->GetComponent<Transform>()->SetScale(1600.f, 896.f, 1.f);
		}

		// ĳ����
		mPlayer = object::Instantiate<Player>(eLayerType::Player, L"Player");
		mPlayer->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
		Transform* tr = mPlayer->GetComponent<Transform>();
		tr->SetPosition(Vector3(-588.f, -170.f, 500.f));




		// �浹ü (���� �ٴ�)
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������

			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetScale(Vector3(416.f, 200.f, 1.f));
			tr->SetPosition(Vector3(-400.f, -292.f, 500.f));

		}

		// �浹ü (������ �ٴ�)
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
	

			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetScale(Vector3(1000.f, 200.f, 1.f));
			tr->SetPosition(Vector3(200.f, -419.f, 500.f));

		}


		// ������ �����ε� 
		{
			Item* Item_overload = object::Instantiate<Item>(eLayerType::Item, L"overload_Item");
		

			ItemScript* script = Item_overload->AddComponent<ItemScript>();
			script->SetOwnerObj(mPlayer);

			Transform* tr = Item_overload->GetComponent<Transform>();
			tr->SetPosition(Vector3(-494.f, -140.f, 500.f));


			


		}


		// ��Ż
		// �ι�° obj ����
		//{
		//	GameObject* bg = new GameObject();
		//	AddGameObject(eLayerType::BG, bg);
		//	// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
		//	MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
		//	mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		//	mr->SetMaterial(Resources::Find<Material>(L"PotalMtrl"));

		//	bg->GetComponent<Transform>()->SetPosition(Vector3(0.35f, -0.31f, 0.9f));
		//	//bg->GetComponent<Transform>()->SetVecrtexScale(0.8f, 0.2f);
		//	//bg->GetComponent<Transform>()->SetRotation(Vector3(0.0f, 0.0f, 45.0f));
		//	bg->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 1.f);
		//}

		//==== UI
		

		// //�÷��̾� UI Frame
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

		// ���� ����1
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			bg->SetName(L"Weapon_Slot1");

			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"tempMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(-33.f, -197.f, 90.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(1.5f, 0.3f);
			bg->GetComponent<Transform>()->SetScale(Vector3(24.f, 24.f, 1.f));


			UIFrameScript* uiframe = bg->AddComponent<UIFrameScript>();
			uiframe->SetPlayer(mPlayer);


		}

		// ���� ����2
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			bg->SetName(L"Weapon_Slot2");

			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"tempMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -185.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(1.5f, 0.3f);
			bg->GetComponent<Transform>()->SetScale(Vector3(24.f, 24.f, 1.f));


			UIFrameScript* uiframe = bg->AddComponent<UIFrameScript>();
			uiframe->SetPlayer(mPlayer);
		}

		// ���� ����3
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			bg->SetName(L"Weapon_Slot3");

			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"tempMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(33.f, -197.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(1.5f, 0.3f);
			bg->GetComponent<Transform>()->SetScale(Vector3(24.f, 24.f, 1.f));


			UIFrameScript* uiframe = bg->AddComponent<UIFrameScript>();
			uiframe->SetPlayer(mPlayer);
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

		//�÷��̾� SP��
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"PlayerSPBarMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(103.f, -205.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(99.f, 8.f, 1.f));
		}



		//�÷��̾� �����ε� ��
		{
			Progressbar* overloadBar = object::Instantiate<Progressbar>(eLayerType::UI, L"overloadBar");
			overloadBar->SetOwner(mPlayer);

			Transform* tr = overloadBar->GetComponent<Transform>();
			tr->SetPosition(Vector3(0.f, -218.f, 95.f));

			overloadBar->Initialize();

		}


		//�÷��̾� �����ε� �� bar 
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"Overload_BarMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -218.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(48.f, 4.f, 1.f));
		}


		// Ű���� ����
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"Keyboard_A_Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(-33.f, -218.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(12.f, 12.f, 1.f));
		}


		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"Keyboard_S_Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -195.f, 80.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(12.f, 12.f, 1.f));
		}

		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"Keyboard_D_Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(33.f, -218.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(12.f, 12.f, 1.f));
		}

		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"Keyboard_F_Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -210.f, 100.f));
			//bg->GetComponent<Transform>()->SetVecrtexScale(0.49f, 0.1f);
			bg->GetComponent<Transform>()->SetScale(Vector3(12.f, 12.f, 1.f));
		}


		// ================ ��, õ�� 
		//{
		//	Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Wall, L"Wall_R_Obj");
		//	col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������


		//	Transform* tr = col_Floor->GetComponent<Transform>();
		//	tr->SetPosition(Vector3(334.f, -38.f, 500.f));
		//	tr->SetScale(Vector3(30.f, 389.f, 1.f));

		//	WallScript* wallscript = col_Floor->AddComponent<WallScript>();
		//	wallscript->SetPlayer(mPlayer);


		//}

		//{
		//	Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Wall, L"Wall_L_Obj");
		//	col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������


		//	Transform* tr = col_Floor->GetComponent<Transform>();
		//	tr->SetPosition(Vector3(-334.f, -38.f, 500.f));
		//	tr->SetScale(Vector3(30.f, 389.f, 1.f));


		//	WallScript* wallscript = col_Floor->AddComponent<WallScript>();
		//	wallscript->SetPlayer(mPlayer);


		//}

		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Wall, L"Wall_Up_Obj");
			col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-250.f, -53.f, 500.f));
			tr->SetScale(Vector3(640.f, 20.f, 1.f));


			WallScript* wallscript = col_Floor->AddComponent<WallScript>();
			wallscript->SetPlayer(mPlayer);


		}

		// =======================================
//		// �浹ü (���� �� �Ѿ��)
		{
			Platform* col_Door = object::Instantiate<Platform>(eLayerType::Collision, L"L_col_Door2");
			col_Door->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������

			Transform* tr = col_Door->GetComponent<Transform>();
			tr->SetPosition(Vector3(189.f, -270.f, 450.f));
			tr->SetScale(Vector3(40.f, 61.f, 1.f));

		}





		// === ī�޶�

		 // ���� ī�޶� ���� (�ݵ�� ������ ���� �ٸ� ���� ī�޶� �ϳ��� �־����!) 
		{
			GameObject* MainCamera = new GameObject();
			AddGameObject(eLayerType::Camera, MainCamera);

			mCamera = MainCamera->AddComponent<Camera>();
			mCamera->TurnLayerMask(eLayerType::UI, false);
			mCamera->SetSize(2.3f); // 2.3 

			CameraScript* camerscript = MainCamera->AddComponent<CameraScript>();

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
	void EntranceScene::Update()
	{
		Scene::Update();

		if (Input::GetKeyDown(eKeyCode::ENTER))
		{

			SceneManager::LoadScene(L"EntryScene");
		}


	
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
		Scene::OnEnter();
		renderer::mainCamera = mCamera;
		mCamera->SetSize(2.3f);

		CameraScript* camerscript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
		camerscript->SetTarget(mPlayer);



	}
	void EntranceScene::OnExit()
	{
		Scene::OnExit();
	}
}