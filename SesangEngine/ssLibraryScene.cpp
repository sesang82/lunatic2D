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
#include "ssProgressbar.h"
#include "ssUIFrameScript.h"
#include "ssAudioSource.h"
#include "ssSoundMgrScript.h"
#include "ssAudioListener.h"
#include "ssAudioClip.h"

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


		// ������
		{


			GameObject* Library = new GameObject();
			AddGameObject(eLayerType::BG, Library);
			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = Library->AddComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"LibraryMtrl"));
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));

			Library->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.f, 850.f));			
			Library->GetComponent<Transform>()->SetScale(Vector3(1600.f, 900.f, 1.0f));
	
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
		mPlayer = object::Instantiate<Player>(eLayerType::Player, L"Player");
		mPlayer->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
		Transform* tr = mPlayer->GetComponent<Transform>();
		tr->SetPosition(Vector3(50.f, -236.f, 550.f));
//		
	
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
	// �浹ü (������ �ִ� �尩) 
		{
			Platform* col = object::Instantiate<Platform>(eLayerType::Ground, L"col_TreeGauntlet");
			col->Initialize();

			Transform* tr = col->GetComponent<Transform>();
			tr->SetPosition(Vector3(-42.f, -170.f, 450.f));
			tr->SetScale(Vector3(25.f, 25.f, 1.f));


		}


		// �浹ü (���� ���� - �尩)
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
			Platform* col = object::Instantiate<Platform>(eLayerType::Ground, L"col_TreeSword");
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



		 // ���� ī�޶� ���� (�ݵ�� ������ ���� �ٸ� ���� ī�޶� �ϳ��� �־����!) 
		{
			mCameraObj = new GameObject();
			AddGameObject(eLayerType::Camera, mCameraObj);

			mCamera = mCameraObj->AddComponent<Camera>();
			mCamera->TurnLayerMask(eLayerType::UI, false);
			mCamera->SetSize(2.3f); // 2.3 

			CameraScript* camerscript = mCameraObj->AddComponent<CameraScript>();


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

			UIFrameScript* uiframe = UIcamera->AddComponent<UIFrameScript>();
			uiframe->SetPlayer(mPlayer);

		}

	
	

	
	}

	void LibraryScene::Update()
	{
		Scene::Update();

	}
	void LibraryScene::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void LibraryScene::Render()
	{

		Scene::Render();
	}
	void LibraryScene::OnEnter()
	{
		Scene::OnEnter();

		renderer::mainCamera = mCamera;

		CameraScript* camerscript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
		camerscript->SetTarget(mPlayer);
		
		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetBGM();
		pBGM->SetClip(Resources::Find<AudioClip>(L"Library_Bgm"));
		pBGM->Play();
		pBGM->SetLoop(true);
		pBGM->SetVolume(0.3f);


	}

	void LibraryScene::OnExit()
	{
		Scene::OnExit();
	}
}