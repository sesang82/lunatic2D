#pragma once
#include "ssEntity.h"
#include "ssLayer.h"

namespace ss
{
	class Scene : public Entity
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
		virtual void Destroy();

		virtual void OnEnter();
		virtual void OnExit();

		void AddGameObject(eLayerType type, GameObject* gameObj);
		//std::vector<GameObject*> Get

		void DeleteGameObject(eLayerType type, GameObject* gameObj);


		// ==씬에 추가된 오브젝트들 중에서도 특정 오브젝트만 갖고 올 수 있게 해주는 함수
		// ex) 몬스터만 갖고 오고 싶을 경우, <>에 몬스터 클래스 넣으면 될 것 
		// 타입이 맞는지 아닌지를 체크할 때는 dynamic_cast를 쓰면 될 것이다.
		// 그런데 게임오브젝트가 담긴 vector를 들고 있는 녀석은 'Layer클래스'다.
		// 때문에 Layer 클래스에 접근해서 해당 게임오브젝트 vector를 반복문 돌린다.
		template <typename T>
		std::vector<T*> FindObjectsOfType()
		{
			std::vector<T*> findObjs = {};
			for (Layer* layer : mLayers)
			{
				// ==== 레이어에 담긴 게임오브젝트들을 반복문 돌린다.
				auto gameObjs = layer->GetGameObjects();

				// 반복문 돌리는 게임오브젝트 중에서도 해당 타입에 해당하는 오브젝트들을 찾아낸다.
				for (GameObject* obj: gameObjs)
				{
					T* buff = dynamic_cast<T*>(obj);
					if (buff != nullptr)
						findObjs.push_back(buff);
				}
			}

			return findObjs;
		}
		Layer& GetLayer(eLayerType type) { return mLayers[(UINT)type]; }


		// 현재 활성화된 씬에서 특정 타입의 오브젝트를 찾아서 반환해주는 함수
		template <typename T>
		T* FindObjInActiveLayer()
		{
			Layer& activeLayer = GetLayer(eLayerType::Player);
			auto gameObj = activeLayer.GetGameObjects();


			for (GameObject* obj : gameObj)
			{
				T* result = dynamic_cast<T*> (obj);

				if (nullptr != result)
					return result;
			}
			return nullptr;
		}



		GameObject* FindGameObjectByName(const std::wstring& name) 
		{
			for (Layer& layer : mLayers) 
			{
				GameObject* obj = layer.FindGameObjectByName(name);
				if (obj) {
					return obj; // 이름이 일치하는 객체를 찾았다면 반환
				}
			}
			return nullptr; // 이름이 일치하는 객체가 없으면 nullptr 반환
		}



	private:
		std::vector<Layer> mLayers;
	};
}
