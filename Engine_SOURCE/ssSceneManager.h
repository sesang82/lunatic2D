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


		static Scene* GetSceneByName(const std::wstring& sceneName)
		{
			auto it = mScenes.find(sceneName);
			if (it != mScenes.end())
			{
				return it->second; // 씬 포인터 반환
			}

			return nullptr; // 해당 이름의 씬이 없으면 nullptr 반환
		}


		static Scene* LoadScene(std::wstring name);
		static Scene* GetActiveScene() { return mActiveScene; }



	private:
		static Scene* mActiveScene;
		// 여러 씬들을 관리하기 편하게 map으로 저장해둠
		static std::map<std::wstring, Scene*> mScenes;

		static eWeaponType WeaponInfo;
		//static Boss* mBoss;
		static class Player* mPlayer;

		static bool mbOnSword;
		static bool mbOnPistol;
		static bool mbOnGauntlet;

	public:
		static void SetWeaponInfo(eWeaponType info) { WeaponInfo = info; }
		static eWeaponType GetWeaponInfo() { return WeaponInfo; }
		static void SetPlayer(Player* player) { mPlayer = player; }
		static Player* GetPlayer() { return mPlayer; }

		static void SetOnSword(bool on) { mbOnSword = on; }
		static void SetOnPistol(bool on) { mbOnPistol = on;}
		static void SetOnGauntlet(bool on) { mbOnGauntlet = on; }

		static bool IsOnSword() { return mbOnSword; }
		static bool IsOnPistol() { return mbOnPistol; }
		static bool IsOnGauntlet() { return mbOnGauntlet; }
	};
}
