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
#include "ssAnimator.h"
#include "ssBigWolfScript.h"
#include "ssMonster.h"
#include "ssCameraScript.h"
#include "ssPlayer.h"
#include "ssPlayerScript.h"
#include "ssRigidbody2D.h"
#include "ssPlatform.h"
#include "ssTriggerScript.h"
#include "ssParalloxScript.h"

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





		// ���� Ÿ�� (�̰� �÷��̾�� �� �տ� �־���ؼ� 400���� ��) 
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"Boss1_Map");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"Boss1TileMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.f, 400.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1600.f, 896.f, 1.0f));
		}



		// =========== ĳ���͵�



		// ����
		mBoss1 = object::Instantiate<Monster>(eLayerType::Boss, L"B_WolfObj");
		mBoss1->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������


		Transform* eyetr = mBoss1->GetComponent<Transform>();
			//eyetr->SetPosition(Vector3(10.f, 185.f, 500.f));
		eyetr->SetPosition(Vector3(10.f, -183.f, 500.f)); // -10

		BigWolfScript* wolfScript = mBoss1->AddComponent<BigWolfScript>();
		wolfScript->SetFirstPos(eyetr->GetPosition());


		//ĳ����
		mPlayer = object::Instantiate<Player>(eLayerType::Player, L"Player");
		mPlayer->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������

		Transform* Playertr = mPlayer->GetComponent<Transform>();
		//Playertr->SetPosition(Vector3(-450.f, -298.0f, 500.f)); // �̰� �÷��̾��� ó�� ��ġ�� ... 
		Playertr->SetPosition(Vector3(-450.f, -298.0f, 500.f));
		PlayerScript* playerScript = mPlayer->GetComponent<PlayerScript>();
		playerScript->SetMonster(mBoss1);


		wolfScript->SetPlayer(mPlayer);

		

		// ====== ���
		// �ձ� ��
		
		//Background* bg = object::Instantiate<Background>(eLayerType::BG, L"BG_Moon");
		//bg->Initialize();

		//MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
		//mr->SetMaterial(Resources::Find<Material>(L"Moon2Mtrl"));

		//bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 50.f, 850.f));
		//bg->GetComponent<Transform>()->SetScale(Vector3(1024.f, 900.f, 1.0f));

		//ParalloxScript* parallox = bg->AddComponent<ParalloxScript>();
		//parallox->SetPlayer(mPlayer);
		//parallox->SetSpeed(Vector2(0.18f, 0.18f));

		//parallox->SetMinX(-31.0f);
		//parallox->SetMaxX(33.0f);

		//parallox->SetMinY(-30.0f);
		//parallox->SetMaxY(30.0f);
		//

		//// ����1

		//{
		//	Background* bg = object::Instantiate<Background>(eLayerType::BG, L"BG_Tree1");
		//	bg->Initialize();


		//	MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
		//	mr->SetMaterial(Resources::Find<Material>(L"Tree1Mtrl"));

		//	bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.0f, 800.f));
		//	bg->GetComponent<Transform>()->SetScale(Vector3(1024.f, 618.f, 1.0f));

		//	ParalloxScript* parallox = bg->AddComponent<ParalloxScript>();
		//	parallox->SetPlayer(mPlayer);
		//	parallox->SetSpeed(Vector2(0.3f, 0.3f));

		//	parallox->SetMinX(-48.0f);
		//	parallox->SetMaxX(48.0f);

		//	parallox->SetMinY(-150.0f);
		//	parallox->SetMaxY(-150.0f);

		//}

		//// ���� 2 
		//{
		//	Background* bg = object::Instantiate<Background>(eLayerType::BG, L"BG_Tree2");
		//	bg->Initialize();

		//	MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
		//	mr->SetMaterial(Resources::Find<Material>(L"Tree2Mtrl"));

		//	bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -50.f, 750.f));
		//	bg->GetComponent<Transform>()->SetScale(Vector3(1024.f, 618.f, 1.0f));



		//	ParalloxScript* parallox = bg->AddComponent<ParalloxScript>();
		//	parallox->SetPlayer(mPlayer);
		//	parallox->SetSpeed(Vector2(0.5f, 0.f));

		//	parallox->SetMinX(-30.0f);
		//	parallox->SetMaxX(30.0f);

		//	parallox->SetMinY(-150.0f);
		//	parallox->SetMaxY(-140.0f);


		//}

		//// ���� 3
		//{
		//	Background* bg = object::Instantiate<Background>(eLayerType::BG, L"BG_Tree3");
		//	bg->Initialize();

		//	MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
		//	mr->SetMaterial(Resources::Find<Material>(L"Tree3Mtrl"));

		//	bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, -100.f, 700.f));
		//	bg->GetComponent<Transform>()->SetScale(Vector3(1200.f, 577.f, 1.0f));


		//	ParalloxScript* parallox = bg->AddComponent<ParalloxScript>();
		//	parallox->SetPlayer(mPlayer);
		//	parallox->SetSpeed(Vector2(0.55f, 0.f));

		//	parallox->SetMinX(-20.0f);
		//	parallox->SetMaxX(80.0f);

		//	parallox->SetMinY(-150.0f);
		//	parallox->SetMaxY(-140.0f);

		//
		//}

		//// ���� 4 
		//{
		//	Background* bg = object::Instantiate<Background>(eLayerType::BG, L"BG_Tree4");
		//	bg->Initialize();

		//	MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
		//	mr->SetMaterial(Resources::Find<Material>(L"Tree4Mtrl"));

		//	bg->GetComponent<Transform>()->SetPosition(Vector3(20.f, -140.f, 650.f));
		//	bg->GetComponent<Transform>()->SetScale(Vector3(1024.f, 589.f, 1.0f));
		//	ParalloxScript* parallox = bg->AddComponent<ParalloxScript>();
		//	parallox->SetPlayer(mPlayer);
		//	parallox->SetSpeed(Vector2(0.8f, 0.f));

		//	parallox->SetMinX(-30.0f);
		//	parallox->SetMaxX(10.0f);

		//	parallox->SetMinY(-150.0f);
		//	parallox->SetMaxY(-140.0f);


		//	
		//}

		// ���� �� ȶ��
		{
			Background* Fire = object::Instantiate<Background>(eLayerType::BG, L"WolfFireObj");
			Fire->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������

			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = Fire->GetComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));

			Fire->GetComponent<Transform>()->SetPosition(Vector3(-190.f, -265.f, 650.f));
			Fire->GetComponent<Transform>()->SetScale(Vector3(41.f, 110.f, 1.0f));

			std::shared_ptr<Texture> Image1 = Resources::Find<Texture>(L"wolfFire");

			Animator* anim = Fire->GetComponent<Animator>();
			anim->Create(L"Boss1_Fire", Image1, Vector2(0.f, 0.f), Vector2(41.f, 110.f), 8, Vector2(41.f, 110.f));
			anim->PlayAnimation(L"Boss1_Fire", true);
		}

		// ���� �ι�° ȶ��
		{
			Background* Fire = object::Instantiate<Background>(eLayerType::BG, L"WolfFireObj");
			Fire->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������

			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = Fire->GetComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));

			Fire->GetComponent<Transform>()->SetPosition(Vector3(-110.f, -265.f, 650.f));
			Fire->GetComponent<Transform>()->SetScale(Vector3(41.f, 110.f, 1.0f));

			std::shared_ptr<Texture> Image1 = Resources::Find<Texture>(L"wolfFire");

			Animator* anim = Fire->GetComponent<Animator>();
			anim->Create(L"Boss1_Fire", Image1, Vector2(0.f, 0.f), Vector2(41.f, 110.f), 8, Vector2(41.f, 110.f));
			anim->PlayAnimation(L"Boss1_Fire", true);
		}

		// ������ ù��° ȶ��
		{
			Background* Fire = object::Instantiate<Background>(eLayerType::BG, L"WolfFireObj");
			Fire->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������

			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = Fire->GetComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));

			Fire->GetComponent<Transform>()->SetPosition(Vector3(120.f, -265.f, 650.f));
			Fire->GetComponent<Transform>()->SetScale(Vector3(41.f, 110.f, 1.0f));

			std::shared_ptr<Texture> Image1 = Resources::Find<Texture>(L"wolfFire");

			Animator* anim = Fire->GetComponent<Animator>();
			anim->Create(L"Boss1_Fire", Image1, Vector2(0.f, 0.f), Vector2(41.f, 110.f), 8, Vector2(41.f, 110.f));
			anim->PlayAnimation(L"Boss1_Fire", true);
		}


		// ������ �ι�° ȶ��
		{
			Background* Fire = object::Instantiate<Background>(eLayerType::BG, L"WolfFireObj");
			Fire->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������

			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = Fire->GetComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));

			Fire->GetComponent<Transform>()->SetPosition(Vector3(200.f, -265.f, 650.f));
			Fire->GetComponent<Transform>()->SetScale(Vector3(41.f, 110.f, 1.0f));

			std::shared_ptr<Texture> Image1 = Resources::Find<Texture>(L"wolfFire");

			Animator* anim = Fire->GetComponent<Animator>();
			anim->Create(L"Boss1_Fire", Image1, Vector2(0.f, 0.f), Vector2(41.f, 110.f), 8, Vector2(41.f, 110.f));
			anim->PlayAnimation(L"Boss1_Fire", true);
		}

		// ���� �� �� 
		{
			Background* Fire = object::Instantiate<Background>(eLayerType::BG, L"WolfFireObj");
			Fire->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������


			// AddComponent�Լ� ��ü�� ��ȯ���� T*�̶� �Ʒ�ó�� �ؼ� mr�� �޴°� ������ ��
			MeshRenderer* mr = Fire->GetComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"WolfStatueMtrl"));

			Fire->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.1f, 650.f));
			Fire->GetComponent<Transform>()->SetScale(Vector3(1600.f, 896.f, 1.0f));
		}

		// ======== �浹ü

		// �ٴ�
		{
			Platform* col_Floor = object::Instantiate<Platform>(eLayerType::Ground, L"col_Floor");
			col_Floor->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������


			Transform* tr = col_Floor->GetComponent<Transform>();
			tr->SetPosition(Vector3(-10.f, -363.f, 500.f));
			tr->SetScale(Vector3(929.f, 88.f, 1.f));


		}

		// ���� ��ȯ�ϴ� �浹ü
		
			Platform* col_Spawn = object::Instantiate<Platform>(eLayerType::Collision, L"col_SpawnBoss1");
			col_Spawn->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������

			Transform* tr = col_Spawn->GetComponent<Transform>();
			tr->SetPosition(Vector3(-400.f, -298.0f, 500.f));
			tr->SetScale(Vector3(20.f, 20.f, 1.f));

			TriggerScript* script = col_Spawn->AddComponent<TriggerScript>();
			script->SetMonster(mBoss1);
			script->SetPlayer(mPlayer);



			// ���� ���� �����ϴ� �浹ü 

			Platform* col_startBoss = object::Instantiate<Platform>(eLayerType::Collision, L"col_startBoss1StageObj");
			col_startBoss->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������

			Transform* spawntr = col_startBoss->GetComponent<Transform>();
			spawntr->SetPosition(Vector3(-200.f, -298.0f, 500.f));
			spawntr->SetScale(Vector3(20.f, 20.f, 1.f));

			playerScript->SetBossStartColObj(col_startBoss);

		/*	TriggerScript* script = col_Spawn->AddComponent<TriggerScript>();
			script->SetMonster(mBoss1);
			script->SetPlayer(mPlayer);*/
		

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

		// ī�޶� ����
		{
			GameObject* camera = new GameObject();
			AddGameObject(eLayerType::Camera, camera);
			camera->GetComponent<Transform>()->SetPosition(Vector3(-80.f, -210.f, 980.f));

			mCamera = camera->AddComponent<Camera>();
			mCamera->TurnLayerMask(eLayerType::UI, false);
			mCamera->SetSize(2.3f); // ����ī�޶� 2.3f�� ���� 

			camera->AddComponent<CameraScript>();
		
			script->SetCamera(camera); // trigger ��ũ��Ʈ���� ���� ī�޶� ���� ���� ���� ���� �� 
			

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

			script->SetUICamera(UIcamera); // trigger ��ũ��Ʈ���� ���� ī�޶� ���� ���� ���� ���� �� 
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
		// ���� ī�޶�� �������ش�. 
		renderer::mainCamera = mCamera;

		CameraScript* camerscript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
		camerscript->SetTarget(mPlayer);

	}





	void Boss1Scene::OnExit()
	{
	}
}