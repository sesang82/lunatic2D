#include "ssSceneManager.h"
//#include "yaPlayScene.h"



namespace ss
{
	// ===== static �ʱ�ȭ
	Scene* SceneManager::mActiveScene = nullptr;

	// �� ��Ƶδ� map
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
		// auto�� ������ �߷��� �� ���� �ǵ�ġ ���� ���纻�� �߻��ǰ� �ִٴ� �޽��� ������
		// ���۷����� �ٿ��ֽ�
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

		// ���� ������ DontDestroyOnLoad�� ������ ��ü�� �� ������ �̵�
		for (size_t i = 0; i < mDontDestroyObjects.size(); ++i)
		{
			GameObject* dontDestroyObject = mDontDestroyObjects[i];

			// ���� ������ ��ü ����
			if (mActiveScene)
			{
				mActiveScene->DeleteGameObject((eLayerType)dontDestroyObject->GetLayerIndex(), dontDestroyObject);
			}

			// �� ������ �ش� ��ü ã��
			GameObject* foundObject = iter->second->FindGameObject((eLayerType)dontDestroyObject->GetLayerIndex(), dontDestroyObject);

			// ��ü�� �̹� ���ο� ���� �ִ��� Ȯ��
			if (!foundObject)
			{
				// ���ο� ���� ��ü �߰�
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
		// m_DontDestroyObjects�� �ݺ������� ���� ������ �̸��� ��ü�� ã�´�
		for (auto& object : mDontDestroyObjects)
		{
			// ��ü�� �̸��� ���´�
			std::wstring name = object->GetName();

			// ������ �̸��� ��ü�� ã���� ���
			if (name == objectName)
			{
				// ȣ���� ������ ����
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
