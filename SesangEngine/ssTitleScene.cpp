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
#include "ssCollisionManager.h"
#include "ssAudioSource.h"
#include "ssObject.h"
#include "ssSoundMgrScript.h"
#include "ssAudioListener.h"
#include "ssAudioClip.h"


ss::TitleScene::TitleScene()
{
}

ss::TitleScene::~TitleScene()
{
}

void ss::TitleScene::Initialize()
{
	Scene::Initialize();

	CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Ground, true);
	CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Wall, true);
	CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Collision, true);
	CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Mon_Bullet, true);

	CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, true);
	CollisionManager::SetLayer(eLayerType::Player, eLayerType::Wall, true);
	CollisionManager::SetLayer(eLayerType::Player, eLayerType::Monster, true);
	CollisionManager::SetLayer(eLayerType::Player, eLayerType::Boss, true);
	CollisionManager::SetLayer(eLayerType::Player, eLayerType::Collision, true);
	CollisionManager::SetLayer(eLayerType::Player, eLayerType::Etc, true);
	CollisionManager::SetLayer(eLayerType::Player, eLayerType::Mon_Bullet, true);
	CollisionManager::SetLayer(eLayerType::Player, eLayerType::Item, true);
	
	CollisionManager::SetLayer(eLayerType::Boss, eLayerType::Collision, true);

	CollisionManager::SetLayer(eLayerType::Guard, eLayerType::Mon_Bullet, true);
	CollisionManager::SetLayer(eLayerType::Guard, eLayerType::Collision, true);

	CollisionManager::SetLayer(eLayerType::Wall, eLayerType::Collision, true);
	CollisionManager::SetLayer(eLayerType::Wall, eLayerType::Mon_Bullet, true);
	CollisionManager::SetLayer(eLayerType::Ground, eLayerType::Collision, true);


	// ----------------
	// 배경
	{
		Background* MainBG = object::Instantiate<Background>(eLayerType::BG, L"Title");
		MainBG->Initialize(); // 초기화 함수를 알아서 못 불러오므로 수동으로 불러와줘야함
		MeshRenderer* mr = MainBG->GetComponent<MeshRenderer>();
		mr->SetMaterial(Resources::Find<Material>(L"TitleBG"));
		
		MainBG->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.f, 0.0f));
		MainBG->GetComponent<Transform>()->SetScale(Vector3(1600.f, 900.f, 1.0f));
		//MainBG->GetComponent<Transform>()->SetRotation(Vector3(0.0f, 0.0f, 45.0f));

	}
	
	// 카메라 생성
	{
		mCam = new GameObject();
		AddGameObject(eLayerType::Camera, mCam);
		Camera* cameraComp = mCam->AddComponent<Camera>();
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraComp->SetSize(1.f);
	}

	// 사운드 매니저
	{
		GameObject* pObject = object::Instantiate<GameObject>(eLayerType::Grid, L"SoundMgr");
		SceneManager::DontDestroyOnLoad(pObject);

		SoundMgrScript* pSoundMgr = pObject->AddComponent<SoundMgrScript>();
		pSoundMgr->SetListener(pObject->AddComponent<AudioListener>());
		pSoundMgr->SetBGM(pObject->AddComponent<AudioSource>());
		pSoundMgr->SetSFX(pObject->AddComponent<AudioSource>());


	}

	// 로고
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
	Scene::Update();

	if (Input::GetKeyDown(eKeyCode::ENTER))
	{
		
		SceneManager::LoadScene(L"LibraryScene");
	}



}

void ss::TitleScene::LateUpdate()
{
	Scene::LateUpdate();
}

void ss::TitleScene::Render()
{
	Scene::Render();
}

void ss::TitleScene::Destroy()
{
	Scene::Destroy();
}

void ss::TitleScene::OnEnter()
{
	Scene::OnEnter();

	AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetBGM();
	pBGM->SetClip(Resources::Find<AudioClip>(L"Title_Bgm"));
	pBGM->Play();
	pBGM->SetLoop(true);
	pBGM->SetVolume(1.f);

}


void ss::TitleScene::OnExit()
{
	Scene::OnExit();

	AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundMgrScript>()->GetBGM();
	pBGM->GetClip()->Stop();

}
