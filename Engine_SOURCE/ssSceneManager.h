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


		static Scene* GetSceneByName(const std::wstring& sceneName)
		{
			auto it = mScenes.find(sceneName);
			if (it != mScenes.end())
			{
				return it->second; // �� ������ ��ȯ
			}

			return nullptr; // �ش� �̸��� ���� ������ nullptr ��ȯ
		}


		static Scene* LoadScene(std::wstring name);
		static Scene* GetActiveScene() { return mActiveScene; }



	private:
		static Scene* mActiveScene;
		// ���� ������ �����ϱ� ���ϰ� map���� �����ص�
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
