#include "ssSceneManager.h"
//#include "yaPlayScene.h"



namespace ss
{
	// ===== static 초기화
	Scene* SceneManager::mActiveScene = nullptr;

	// 씬 모아두는 map
	std::map<std::wstring, Scene*> SceneManager::mScenes;

	std::vector<GameObject*> SceneManager::mDontDestroyObjects;

	Player* SceneManager::mPlayer = nullptr;

	eWeaponType SceneManager::WeaponInfo = eWeaponType::SWORD;

	bool SceneManager::mbOnSword = false;
	bool SceneManager::mbOnPistol = false;
	bool SceneManager::mbOnGauntlet = false;



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

		std::map<std::wstring, Scene*>::iterator iter = mScenes.find(name);

		if (iter == mScenes.end())
			return nullptr;

		// 이전 씬에서 DontDestroyOnLoad로 설정한 객체를 새 씬으로 이동
		for (size_t i = 0; i < mDontDestroyObjects.size(); ++i)
		{
			GameObject* dontDestroyObject = mDontDestroyObjects[i];

			// 현재 씬에서 객체 제거
			if (mActiveScene)
			{
				mActiveScene->DeleteGameObject((eLayerType)dontDestroyObject->GetLayerIndex(), dontDestroyObject);
			}

			// 새 씬에서 해당 객체 찾기
			GameObject* foundObject = iter->second->FindGameObject((eLayerType)dontDestroyObject->GetLayerIndex(), dontDestroyObject);

			// 객체가 이미 새로운 씬에 있는지 확인
			if (!foundObject)
			{
				// 새로운 씬에 객체 추가
				iter->second->AddGameObject((eLayerType)dontDestroyObject->GetLayerIndex(), dontDestroyObject);
			}
		}

		mActiveScene->OnExit();
		mActiveScene = iter->second;
		mActiveScene->OnEnter();

		return iter->second;
	}

	GameObject* SceneManager::FindSoundMgr()
	{
		for (auto& object : mDontDestroyObjects)
		{
			std::wstring name = object->GetName();

			if (L"SoundMgr" == name)
			{
				return object;
				break;
			}
		}

		return nullptr;
	}
	


	void SceneManager::RemoveFromDontDestroyOnLoad(GameObject* _GameObject)
	{
		auto iter = std::find(mDontDestroyObjects.begin(), mDontDestroyObjects.end(), _GameObject);

		if (iter != mDontDestroyObjects.end())
		{
			mDontDestroyObjects.erase(iter);
		}
	}

	void SceneManager::DontUseOnLoad(std::wstring objectName)
	{
		// m_DontDestroyObjects를 반복문으로 돌며 동일한 이름의 객체를 찾는다
		for (auto& object : mDontDestroyObjects)
		{
			// 객체의 이름을 얻어온다
			std::wstring name = object->GetName();

			// 동일한 이름의 객체를 찾았을 경우
			if (name == objectName)
			{
				// 호출한 씬에서 제외
				if (mActiveScene)
				{
					mActiveScene->DeleteGameObject((eLayerType)object->GetLayerIndex(), object);
				}
			}
		}
	}

	void SceneManager::DontUseOnLoad(eLayerType _LayerType)
	{
		for (auto& object : mDontDestroyObjects)
		{
			int iLayerType = object->GetLayerIndex();

			if ((int)_LayerType == iLayerType)
			{
				if (mActiveScene)
				{
					mActiveScene->DeleteGameObject((eLayerType)iLayerType, object);
				}
			}
		}
	}

}
