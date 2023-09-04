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


		


		// ��� (�ڿ� �� ������ ���� ���� ���°� ������ ���⵵�� �����ϱ�) 
		{
			Background* bg = object::Instantiate<Background>(eLayerType::BG, L"EntryBG");
			bg->Initialize();

			MeshRenderer* mr = bg->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"EntryMtrl"));

			bg->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.45f, 800.f));
			bg->GetComponent<Transform>()->SetScale(Vector3(1600.f, 896.f, 0.9f));
		}

		// ĳ����
		mPlayer  = object::Instantiate<Player>(eLayerType::Player, L"Player");
		mPlayer->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
		Transform* tr = mPlayer->GetComponent<Transform>();
		tr->SetPosition(Vector3(-200.f, -362.f, 500.f));



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

		BaseScene::Initialize();

	}
	void EntryScene::Update()
	{
		if (Input::GetKeyDown(eKeyCode::ENTER))
		{

			SceneManager::LoadScene(L"Stage1Scene");
		}

		BaseScene::Update();
	}
	void EntryScene::LateUpdate()
	{
		BaseScene::LateUpdate();
	}
	void EntryScene::Render()
	{
		BaseScene::Render();
	}
	void EntryScene::OnEnter()
	{
		renderer::mainCamera = mCamera;

		CameraScript* camerscript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
		camerscript->SetTarget(mPlayer);
	
	}
	void EntryScene::OnExit()
	{
	}
}