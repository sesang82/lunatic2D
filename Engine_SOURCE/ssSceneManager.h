#pragma once
#include "ssScene.h"

namespace ss
{

	
	// 기존에는 씬 매니저를 통해서 씬을 생성해줬지만, 이제는 세상 엔진에서 생성하는 걸로 바꿈
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
			mActiveScene = scene; // 마지막에 추가된 씬을 활성화 씬으로 설정해둔다.

			scene->Initialize(); // Create 해둔 씬들의 Initialize를 호출해준다.
			return true;
		}
		static Scene* LoadScene(std::wstring name);
		static Scene* GetActiveScene() { return mActiveScene; }


	private:
		static Scene* mActiveScene;
		// 여러 씬들을 관리하기 편하게 map으로 저장해둠
		static std::map<std::wstring, Scene*> mScenes;

		static class Player* mPlayer;
		//static Boss* mBoss;
	};
}
