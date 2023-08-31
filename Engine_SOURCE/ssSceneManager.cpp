#include "ssSceneManager.h"
//#include "yaPlayScene.h"



namespace ss
{
	// ===== static 초기화
	Scene* SceneManager::mActiveScene = nullptr;
	eWeaponType SceneManager::WeaponInfo = eWeaponType::SWORD;


	// 씬 모아두는 map
	std::map<std::wstring, Scene*> SceneManager::mScenes;

	void SceneManager::Initialize()
	{

	}
	void SceneManager::Update()
	{
		mActiveScene->Update();
	}
	void SceneManager::LateUpdate()
	{
		mActiveScene->LateUpdate();
	}
	void SceneManager::Render()
	{
		mActiveScene->Render();
	}

	void SceneManager::Destroy()
	{
		mActiveScene->Destroy();
	}

	void SceneManager::Release()
	{
		// auto는 참조를 추론할 수 없어 의도치 않은 복사본이 발생되고 있다는 메시지 때문에
		// 레퍼런스를 붙여주심
		for (auto& iter : mScenes)
		{
			delete iter.second;
			iter.second = nullptr;
		}
	}

	Scene* SceneManager::LoadScene(std::wstring name)
	{
		// map을 iterator로 돌림
		std::map<std::wstring, Scene*>::iterator iter
			= mScenes.find(name);

		if (iter == mScenes.end())
			return nullptr;
	
		// 맵에 저장된 씬이 있다면 일단 활성화중이던 씬을 나가고
		mActiveScene->OnExit();

		// map에서 반환된 씬 메모리 주소를 활성화 씬으로 대체해준다. 
		mActiveScene = iter->second;
		// 새로 활성화 된 씬의 OnEnter로 들어간다. 
		mActiveScene->OnEnter();

		return iter->second;
	}
}
