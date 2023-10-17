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
#include "ssPlatform.h"
#include "ssParalloxScript.h"
#include "ssUIFrameScript.h"
#include "ssProgressbar.h"
#include "ssAnimator.h"

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


		Scene::Initialize();


		// ====== ���
	// �ձ� ��

		Background* bg = object::Instantiate<Background>(eLayerType::BG, L"BG_Moon");
		bg->Initialize();

		MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
		mr->SetMaterial(Resources::Find<Material>(L"Moon2Mtrl"));

		bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.f, 850.f));
		bg->GetComponent<Transform>()->SetScale(Vector3(1024.f, 900.f, 1.0f));

		ParalloxScript* parallox = bg->AddComponent<ParalloxScript>();
		parallox->SetPlayer(mPlayer);
		parallox->SetSpeed(Vector2(0.18f, 0.18f));

		parallox->SetMinX(-31.0f);
		parallox->SetMaxX(33.0f);

		parallox->SetMinY(-100.0f);
		parallox->SetMaxY(50.0f);


		// ����1

		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"BG_Tree1");
			bg->Initialize();


			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Tree1Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -300.0f, 800.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1024.f, 618.f, 1.0f));

			ParalloxScript* parallox = bg->AddComponent<ParalloxScript>();
			parallox->SetPlayer(mPlayer);
			parallox->SetSpeed(Vector2(0.3f, 0.3f));

			parallox->SetMinX(-48.0f);
			parallox->SetMaxX(48.0f);

			parallox->SetMinY(-180.0f);
			parallox->SetMaxY(-180.0f);

		}

		// ���� 2 
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"BG_Tree2");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Tree2Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -400.f, 750.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1024.f, 618.f, 1.0f));



			ParalloxScript* parallox = bg->AddComponent<ParalloxScript>();
			parallox->SetPlayer(mPlayer);
			parallox->SetSpeed(Vector2(0.4f, 0.4f));

			parallox->SetMinX(-30.0f);
			parallox->SetMaxX(30.0f);

			parallox->SetMinY(-200.0f);
			parallox->SetMaxY(-200.0f);


		}

			// ��� �� ���� 
		{
			Background * bg = object::Instantiate<Background>(eLayerType::BG, L"EntranceMap");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"EntryMtrl"));
			//mr->Initialize();

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.55f, 700.0f));
			bg->GetComponent<Transform>()->SetScale(1600.f, 896.f, 1.f);
		}


		// ��� �� 
		{


			GameObject* Library = new GameObject();
			AddGameObject(eLayerType::BG, Library);
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = Library->AddComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Moon2StatueMtrl"));
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));

			Library->GetComponent<Transform>()->SetPosition(Vector3(0.0f, -298.f, 730.f));
			Library->GetComponent<Transform>()->SetScale(Vector3(156.f, 169.f, 1.0f));

		}

		// ���� �� 
		{

			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"Left_Statue");
			bg->Initialize();

			bg->GetComponent<Transform>()->SetPosition(Vector3(-80.0f, -325.f, 730.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(212.f, 124.f, 1.0f));
		
		}

		// ������ �� 
		{


			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"Right_Statue");
			bg->Initialize();

			bg->GetComponent<Transform>()->SetPosition(Vector3(80.0f, -325.f, 730.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(212.f, 124.f, 1.0f));

		}

		// ĳ����
		mPlayer  = object::Instantiate<Player>(eLayerType::Player, L"Player");
		mPlayer->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
		Transform* tr = mPlayer->GetComponent<Transform>();
		tr->SetPosition(Vector3(-200.f, -362.f, 500.f));




		// �浹ü (�ٴ�)
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
			tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(5.f, -393.f, 450.f));
			tr->SetScale(Vector3(950.f, 20.f, 1.f));


		}


		// =======================================
//		// �浹ü (���� �� �Ѿ��)
		{
			Platform* col_Door = object::Instantiate<Platform>(eLayerType::Collision, L"L_col_Door3");
			col_Door->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������

			Transform* tr = col_Door->GetComponent<Transform>();
			tr->SetPosition(Vector3(490.f, -350.f, 450.f));
			tr->SetScale(Vector3(20.f, 41.f, 1.f));

		}




		 //==== UI (UI ��ġ ������ �� ���� �ִ� UI �������� �ϱ�)
		 //�÷��̾� UI Frame (Hp�ٴ� ����ȭ�� �� UI Frame�� �ڽ����� �����صα�)
		{
			GameObject* bg = new GameObject();
			AddGameObject(eLayerType::UI, bg);
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = bg->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"PlayerFrameMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -200.f, 100.f));
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

			bg->GetComponent<Transform>()->SetPosition(Vector3(-103.f, -209.f, 100.f));
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




		// ==== ī�޶�
// ī�޶� ����
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



	}
	void EntryScene::Update()
	{

		Scene::Update();
	}
	void EntryScene::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void EntryScene::Render()
	{
		Scene::Render();
	}
	void EntryScene::OnEnter()
	{
		Scene::OnEnter();

		renderer::mainCamera = mCamera;

		CameraScript* camerscript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
		camerscript->SetTarget(mPlayer);
	
	}
	void EntryScene::OnExit()
	{
		Scene::OnExit();
	}
}