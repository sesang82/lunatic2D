#pragma once
#include "ssScene.h"

namespace ss
{

	
	// �������� �� �Ŵ����� ���ؼ� ���� ������������, ������ ���� �������� �����ϴ� �ɷ� �ٲ�
	class SceneManager
	{
	public:
		static void Initialize();
		static void Update();
		static void LateUpdate();
		static void Render();
		static void Destroy();
		static void Release();
		
		template <typename T>
		static bool CreateScene(std::wstring name)
		{
			T* scene = new T();

			std::map<std::wstring, Scene*>::iterator iter
				= mScenes.find(name);

			if (iter != mScenes.end())
				return false;

			mScenes.insert(std::make_pair(name, scene));
			mActiveScene = scene; // �������� �߰��� ���� Ȱ��ȭ ������ �����صд�.

			scene->Initialize(); // Create �ص� ������ Initialize�� ȣ�����ش�.
			return true;
		}
		static Scene* LoadScene(std::wstring name);
		static Scene* GetActiveScene() { return mActiveScene; }


	private:
		static Scene* mActiveScene;
		// ���� ������ �����ϱ� ���ϰ� map���� �����ص�
		static std::map<std::wstring, Scene*> mScenes;

		static class Player* mPlayer;
		//static Boss* mBoss;
	};
}
