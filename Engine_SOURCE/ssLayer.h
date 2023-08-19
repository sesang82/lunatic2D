#pragma once
#include "ssGameObject.h"

namespace ss
{
	class Layer
	{
	public:
		Layer();
		~Layer();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
		virtual void Destory();

		void AddGameObject(GameObject* gameObj);
		const std::vector<GameObject*> GetGameObjects() 
		{ 
			return mGameObjects; 
		}

		void DeleteGameObject(GameObject* gameObj);

		GameObject* FindGameObjectByName(const std::wstring& name);
		
	private:
		// 추가된 게임 오브젝트들은 여기있음
		std::vector<GameObject*> mGameObjects;
	};
}
