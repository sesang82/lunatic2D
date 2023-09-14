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
#include "ssSkeletonArcherScript.h"
#include "ssSkeletonLizardScript.h"
#include "ssWoodGolemScript.h"
#include "ssWolfScript.h"
#include "ssZombieScript.h"

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
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Wall, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Collision, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Mon_Bullet, true);

		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Monster, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Boss, true);
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
		mPlayer = object::Instantiate<Player>(eLayerType::Player, L"Player");
		mPlayer->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������

		Transform* Playertr = mPlayer->GetComponent<Transform>();
		Playertr->SetPosition(Vector3(-400.f, -366.0f, 500.f)); // �̰� �÷��̾��� ó�� ��ġ�� ... 

		{
			// �������
			StoneEye* Stone = object::Instantiate<StoneEye>(eLayerType::Monster, L"StoneEye");
			Stone->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
			Transform* eyetr = Stone->GetComponent<Transform>();
			eyetr->SetPosition(Vector3(-200.f, -528.f, 500.f));

			StoneEyeScript* stonescript = Stone->GetComponent<StoneEyeScript>();
			stonescript->SetFirstPos(eyetr->GetPosition());

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
			//stoneHP->SetOffset(-36.0f);
			stonehptr->SetPosition(Vector3(-36.f, 38.f, 500.f));
			stoneHP->Initialize();

		}

		{
			// �ذ� Archer (�ü�)
			Monster* Stone = object::Instantiate<Monster>(eLayerType::Monster, L"Archer");
			Stone->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
			Transform* eyetr = Stone->GetComponent<Transform>();
			eyetr->SetPosition(Vector3(-111.f, -166.f, 500.f));

			SkeletonArcherScript* ArcherScript = Stone->AddComponent<SkeletonArcherScript>();
			ArcherScript->SetFirstPos(eyetr->GetPosition());


			// ===== hpƲ �׳� ���� ���� 
			// ���� HP (UI�� ��������. �׷� UIī�޶� ���� �÷��̾� ����ٴϴ� ��ó�� ������) 
			Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::Etc, L"ArcherHP");
			stoneHP->SetParent(Stone);

		
			Transform* stonehptr = stoneHP->GetComponent<Transform>();
			stonehptr->SetPosition(Vector3(-5.f, 38.f, 500.f));

			stoneHP->Initialize();

		}


		{
			// �ذ� Lizard (���� ���ݸ� �ϴ� ��)
			Monster* Liazrd = object::Instantiate<Monster>(eLayerType::Monster, L"Lizard");
			Liazrd->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
			Transform* eyetr = Liazrd->GetComponent<Transform>();
			eyetr->SetPosition(Vector3(40.f, 185.f, 500.f));
		


			SkeletonLizardScript* ArcherScript = Liazrd->AddComponent<SkeletonLizardScript>();
			ArcherScript->SetFirstPos(eyetr->GetPosition());


			// ===== hpƲ �׳� ���� ���� 
			// ���� HP (UI�� ��������. �׷� UIī�޶� ���� �÷��̾� ����ٴϴ� ��ó�� ������) 
			Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::Etc, L"LizardHP");
			stoneHP->SetParent(Liazrd);


			Transform* stonehptr = stoneHP->GetComponent<Transform>();
			stonehptr->SetPosition(Vector3(50.f, 25.f, 500.f));

			stoneHP->Initialize();

		}


		{
			// Wood Golem ���� 
			Monster* Golem = object::Instantiate<Monster>(eLayerType::Monster, L"WoodGolemObj");
			Golem->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
			Transform* eyetr = Golem->GetComponent<Transform>();
			eyetr->SetPosition(Vector3(-240.f, 70.f, 500.f));

			WoodGolemScript* ArcherScript = Golem->AddComponent<WoodGolemScript>();
			ArcherScript->SetFirstPos(eyetr->GetPosition());

			// ���� ü�¹� Ʋ
		/*	MonsterBar* mosnterbar = object::Instantiate<MonsterBar>(eLayerType::Etc, L"WoodHPBar");
			mosnterbar->Initialize();

			Transform* monsterbartr = mosnterbar->GetComponent<Transform>();
			mosnterbar->SetParent(Golem);
			monsterbartr->SetPosition(Vector3(50.f, 40.f, 500.f));*/


			// ���� HP (UI�� ��������. �׷� UIī�޶� ���� �÷��̾� ����ٴϴ� ��ó�� ������) 
			Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::Etc, L"WoodHP");
			stoneHP->SetParent(Golem);
			stoneHP->SetOffset(100.f);


			Transform* stonehptr = stoneHP->GetComponent<Transform>();
			stonehptr->SetPosition(Vector3(50.f, 40.f, 500.f));

			stoneHP->Initialize();

		}

		{
			// �̴� wolf ���� 
			Monster* Wolf = object::Instantiate<Monster>(eLayerType::Monster, L"WolfObj");
			Wolf->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
			
			
			Transform* eyetr = Wolf->GetComponent<Transform>();
			eyetr->SetPosition(Vector3(10.f, 185.f, 500.f));
			//eyetr->SetPosition(Vector3(-350.f, -340.f, 500.f));

			WolfScript* wolfScript = Wolf->AddComponent<WolfScript>();
			wolfScript->SetFirstPos(eyetr->GetPosition());


			// ���� ü�¹� Ʋ
		/*	MonsterBar* mosnterbar = object::Instantiate<MonsterBar>(eLayerType::Etc, L"WolfHPBar");
			mosnterbar->Initialize();

			Transform* monsterbartr = mosnterbar->GetComponent<Transform>();
			mosnterbar->SetParent(Wolf);
			monsterbartr->SetPosition(Vector3(50.f, 25.f, 500.f));*/

			// ===== hpƲ �׳� ���� ���� 
			// ���� HP (UI�� ��������. �׷� UIī�޶� ���� �÷��̾� ����ٴϴ� ��ó�� ������) 
			Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::Etc, L"WolfHP");
			stoneHP->SetParent(Wolf);
		
			Transform* stonehptr = stoneHP->GetComponent<Transform>();
			stonehptr->SetPosition(Vector3(50.f, 25.f, 500.f));

			stoneHP->Initialize();

		}

		
		//	// Zombi ���� (�ش� ������ �� �̻� �� ���� ��� ����)
		//	Monster* Wolf = object::Instantiate<Monster>(eLayerType::Monster, L"ZombiObj");
		//	Wolf->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������


		//	Transform* eyetr = Wolf->GetComponent<Transform>();
		//	//eyetr->SetPosition(Vector3(10.f, 185.f, 500.f));
		//	eyetr->SetPosition(Vector3(-350.f, -340.f, 500.f));

		//	ZombieScript* wolfScript = Wolf->AddComponent<ZombieScript>();
		//	wolfScript->SetFirstPos(eyetr->GetPosition());


		//	// ���� ü�¹� Ʋ
		///*	MonsterBar* mosnterbar = object::Instantiate<MonsterBar>(eLayerType::Etc, L"WolfHPBar");
		//	mosnterbar->Initialize();

		//	Transform* monsterbartr = mosnterbar->GetComponent<Transform>();
		//	mosnterbar->SetParent(Wolf);
		//	monsterbartr->SetPosition(Vector3(50.f, 25.f, 500.f));*/

		//	// ===== hpƲ �׳� ���� ���� 
		//	// ���� HP (UI�� ��������. �׷� UIī�޶� ���� �÷��̾� ����ٴϴ� ��ó�� ������) 
		//	Progressbar* stoneHP = object::Instantiate<Progressbar>(eLayerType::Etc, L"ZombiHP");
		//	stoneHP->SetParent(Wolf);

		//	Transform* stonehptr = stoneHP->GetComponent<Transform>();
		//	stonehptr->SetPosition(Vector3(50.f, 25.f, 500.f));

		//	stoneHP->Initialize();

		//}
			// =====
		// �׽�Ʈ������ ���� ��ġ 			eyetr->SetPosition(Vector3(-350.f, -340.f, 500.f));


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
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������

			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-80.f, -338.f, 500.f));
			tr->SetScale(Vector3(95.f, 95.f, 1.f));


		}

		// �浹ü (�� �Ʒ� ������ �ٴ� )
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
	

			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(43.f, -396.f, 500.f));
			tr->SetScale(Vector3(150.f, 150.f, 1.f));


		}


		// �浹ü (2�� �볪�� �ٴ�  )
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
			col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-111.f, -198.f, 500.f));
			tr->SetScale(Vector3(222.f, 5.f, 1.f));


		}


		// �浹ü (3�� �볪�� �ٴ�  )
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
			col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-240.f, -68.f, 500.f));
			tr->SetScale(Vector3(220.f, 5.f, 1.f));


		}

		// �浹ü (3�� �볪�� �� �ٴ�  )
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-384.f, -98.f, 500.f));
			tr->SetScale(Vector3(62.f, 5.f, 1.f));


		}



		// �浹ü (4�� �볪�� �ٴ�  )
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
			col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-240.f, 28.f, 500.f));
			tr->SetScale(Vector3(222.f, 5.f, 1.f));


		}


		// �浹ü (5�� �볪�� �ٴ�  )
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_SpecialFloor");
			col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(70.f, 158.f, 500.f));
			tr->SetScale(Vector3(520.f, 5.f, 1.f));


		}



	

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
		mCamera->SetSize(2.3f);

		CameraScript* camerscript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
		camerscript->SetTarget(mPlayer);
	}
	void Stage1Scene::OnExit()
	{
	}
}