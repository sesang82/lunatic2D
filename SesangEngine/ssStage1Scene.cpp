#include "ssStage1Scene.h"

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
#include "ssPlatform.h"
#include "ssGroundScript.h"
#include "ssCamera.h"
#include "ssCameraScript.h"
#include "ssPlayer.h"
#include "ssMonster.h"
#include "ssStoneEyeScript.h"
#include "ssCollisionManager.h"
#include "ssPlayerScript.h"
#include "ssStoneEye.h"
#include "ssStoneEyeScript.h"
#include "ssAttackCollider.h"
#include "ssPlayerAttackColScript.h"
#include "ssMonsterBar.h"

namespace ss
{
	Stage1Scene::Stage1Scene()
	{
	}
	Stage1Scene::~Stage1Scene()
	{
	}
	void Stage1Scene::Initialize()
	{

		// ���̾� �浹�� ����ϴ� �Լ� 

		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Ground, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Wall, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Collision, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Monster, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Collision, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Etc, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Mon_Bullet, true);
		CollisionManager::SetLayer(eLayerType::Guard, eLayerType::Mon_Bullet, true);

		// �� ���
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"DungeonMoon");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Moon2Mtrl"));


			bg->GetComponent<Transform>()->SetPosition(Vector3(-0.4f, 0.4f, 950.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1.6f, 1.6f, 1.0f));


		}

		// ����1

		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"DungeonTree1");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Tree1Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.24f, 850.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1.6f, 2.f, 1.0f));
		}

		// ���� 2 
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"DungeonTree2");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Tree2Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.24f, 800.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1.6f, 2.f, 1.0f));
		}

		// ���� 3
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"DungeonTree3");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Tree3Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.24f, 750.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1.6f, 2.f, 1.0f));
		}

		// ���� 4 
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"DungeonTree4");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Tree4Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.24f, 700.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1.6f, 2.f, 1.0f));
		}

		// �� ���	
		//
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"DungeonMap");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Stage1Mtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.f, 650.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1600.f, 900.f, 1.0f));


		}


		 //ĳ����
		Player* player = object::Instantiate<Player>(eLayerType::Player, L"Player");
		player->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������

		Transform* Playertr = player->GetComponent<Transform>();
		Playertr->SetPosition(Vector3(-300.f, -529.6f, 500.f)); // �̰� �÷��̾��� ó�� ��ġ�� ... 



		//// //=== ĳ���Ϳ� �ǰ� �浹ü 
		//AttackCollider* playerAttackCol = object::Instantiate<AttackCollider>(eLayerType::Collision, L"PlayerAttackCollider");



		// ����
		StoneEye* Stone = object::Instantiate<StoneEye>(eLayerType::Monster, L"StoneEye");
		Stone->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
		Transform* eyetr = Stone->GetComponent<Transform>();
		eyetr->SetPosition(Vector3(-200.f, -528.f, 500.f));

		StoneEyeScript* stonescript = Stone->GetComponent<StoneEyeScript>();
		stonescript->SetFirstPos(eyetr->GetPosition());



		// === �Ʒ��� ���װ� �־ �׳� ���ϱ�� �� 
		// ���� ü�¹� Ʋ
		MonsterBar* mosnterbar = object::Instantiate<MonsterBar>(eLayerType::UI, L"StoneEyeBar");
		mosnterbar->Initialize();

		Transform* monsterbartr = mosnterbar->GetComponent<Transform>();
		mosnterbar->SetParent(Stone);
		monsterbartr->SetPosition(Vector3(-36.f, 38.f, 500.f));


		// ���� HP
		Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::UI, L"StoneEyeHP");
		stoneHP->SetParent(Stone);
		Transform* stonehptr = stoneHP->GetComponent<Transform>();
		stonehptr->SetPosition(Vector3(-50.f, 38.f, 500.f));
		stoneHP->Initialize();







		////==================
	   // Collision


   // �浹ü (���� �ٴ�)
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
		

			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-300.f, -487.f, 500.f));
			tr->SetScale(Vector3(416.f, 200.f, 1.f));


		}

		// �浹ü (���� �ٴ�)
		//{
		//	Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
		//	col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������

		//	Transform* tr = col_Floor->GetComponent<Transform>();
		//	tr->SetPosition(Vector3(-80.f, -338.f, 500.f));
		//	tr->SetScale(Vector3(95.f, 95.f, 1.f));


		//}

		//// �浹ü (�� �Ʒ� ������ �ٴ� )
		//{
		//	Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
		//	col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
	

		//	Transform* tr = col_Floor->GetComponent<Transform>();
		//	tr->SetPosition(Vector3(43.f, -396.f, 500.f));
		//	tr->SetScale(Vector3(150.f, 150.f, 1.f));


		//}


		//// �浹ü (2�� �볪�� �ٴ�  )
		//{
		//	Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
		//	col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������


		//	Transform* tr = col_Floor->GetComponent<Transform>();
		//	tr->SetPosition(Vector3(-111.f, -198.f, 500.f));
		//	tr->SetScale(Vector3(222.f, 5.f, 1.f));


		//}


		//// �浹ü (3�� �볪�� �ٴ�  )
		//{
		//	Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
		//	col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������


		//	Transform* tr = col_Floor->GetComponent<Transform>();
		//	tr->SetPosition(Vector3(-240.f, -68.f, 500.f));
		//	tr->SetScale(Vector3(220.f, 5.f, 1.f));


		//}

		//// �浹ü (3�� �볪�� �� �ٴ�  )
		//{
		//	Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
		//	col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������


		//	Transform* tr = col_Floor->GetComponent<Transform>();
		//	tr->SetPosition(Vector3(-384.f, -98.f, 500.f));
		//	tr->SetScale(Vector3(62.f, 5.f, 1.f));


		//}



		//// �浹ü (4�� �볪�� �ٴ�  )
		//{
		//	Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
		//	col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������


		//	Transform* tr = col_Floor->GetComponent<Transform>();
		//	tr->SetPosition(Vector3(-240.f, 28.f, 500.f));
		//	tr->SetScale(Vector3(222.f, 5.f, 1.f));


		//}


		//// �浹ü (5�� �볪�� �ٴ�  )
		//{
		//	Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
		//	col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������


		//	Transform* tr = col_Floor->GetComponent<Transform>();
		//	tr->SetPosition(Vector3(70.f, 158.f, 500.f));
		//	tr->SetScale(Vector3(520.f, 5.f, 1.f));


		//}



		// ======================================

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




		// === ī�޶�

	 // ���� ī�޶� ���� (�ݵ�� ������ ���� �ٸ� ���� ī�޶� �ϳ��� �־����!) 
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
			UIcamera->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 970.0f));
			Camera* cameraComp0 = UIcamera->AddComponent<Camera>();
			cameraComp0->DisableLayerMasks();
			cameraComp0->TurnLayerMask(eLayerType::UI);
			renderer::cameras.push_back(cameraComp0);
		}



		Scene::Initialize();
	}
	void Stage1Scene::Update()
	{
		if (Input::GetKeyDown(eKeyCode::ENTER))
		{

			SceneManager::LoadScene(L"Boss1Scene");
		}


		Scene::Update();
	}
	void Stage1Scene::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Stage1Scene::Render()
	{
		Scene::Render();
	}
	void Stage1Scene::OnEnter()
	{
		renderer::mainCamera = mCamera;
		mCamera->SetSize(3.5f);
	}
	void Stage1Scene::OnExit()
	{
	}
}