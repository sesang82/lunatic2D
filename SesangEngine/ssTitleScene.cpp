#include "ssTitleScene.h"
#include "ssInput.h"
#include "ssSceneManager.h"
#include "ssTransform.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssCamera.h"
#include "ssLibraryScene.h"
#include "ssRenderer.h"
#include "ssCameraScript.h"
#include "ssObject.h"
#include "ssBackground.h"


ss::TitleScene::TitleScene()
{
}

ss::TitleScene::~TitleScene()
{
}

void ss::TitleScene::Initialize()
{
	// ���
	{
		Background* MainBG = object::Instantiate<Background>(eLayerType::BG, L"Title");
		MainBG->Initialize(); // �ʱ�ȭ �Լ��� �˾Ƽ� �� �ҷ����Ƿ� �������� �ҷ��������
		MeshRenderer* mr = MainBG->GetComponent<MeshRenderer>();
		mr->SetMaterial(Resources::Find<Material>(L"TitleBG"));
		
		MainBG->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.f, 0.0f));
		MainBG->GetComponent<Transform>()->SetScale(Vector3(1600.f, 900.f, 1.0f));
		//MainBG->GetComponent<Transform>()->SetRotation(Vector3(0.0f, 0.0f, 45.0f));

	}
	
	// ī�޶� ����
	{
		GameObject* camera = new GameObject();
		AddGameObject(eLayerType::Camera, camera);
		Camera* cameraComp = camera->AddComponent<Camera>();
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraComp->SetSize(1.f);

		CameraScript* camerscript = camera->AddComponent<CameraScript>();
	}


	// �ΰ�
	//{
	//	GameObject* Logo = new GameObject;
	//	AddGameObject(eLayerType::BG, Logo);

	//	MeshRenderer* logo = Logo->AddComponent<MeshRenderer>();
	//	logo->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
	//	logo->SetMaterial(Resources::Find<Material>(L"TitleLogo"));
	//}
	


}

void ss::TitleScene::Update()
{
	if (Input::GetKeyDown(eKeyCode::ENTER))
	{
		
		SceneManager::LoadScene(L"LibraryScene");
	}


	BaseScene::Update();
}

void ss::TitleScene::LateUpdate()
{
	BaseScene::LateUpdate();
}

void ss::TitleScene::Render()
{
	BaseScene::Render();
}

void ss::TitleScene::OnEnter()
{


}

void ss::TitleScene::OnExit()
{
}
