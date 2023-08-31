#include "ssSceneManager.h"
//#include "yaPlayScene.h"



namespace ss
{
	// ===== static �ʱ�ȭ
	Scene* SceneManager::mActiveScene = nullptr;
	eWeaponType SceneManager::WeaponInfo = eWeaponType::SWORD;


	// �� ��Ƶδ� map
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
		// map�� iterator�� ����
		std::map<std::wstring, Scene*>::iterator iter
			= mScenes.find(name);

		if (iter == mScenes.end())
			return nullptr;
	
		// �ʿ� ����� ���� �ִٸ� �ϴ� Ȱ��ȭ���̴� ���� ������
		mActiveScene->OnExit();

		// map���� ��ȯ�� �� �޸� �ּҸ� Ȱ��ȭ ������ ��ü���ش�. 
		mActiveScene = iter->second;
		// ���� Ȱ��ȭ �� ���� OnEnter�� ����. 
		mActiveScene->OnEnter();

		return iter->second;
	}
}
