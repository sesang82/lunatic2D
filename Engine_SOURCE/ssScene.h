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

		GameObject* FindGameObject(eLayerType _Type, GameObject* _GameObj);
		GameObject* FindGameObject(eLayerType _Type, std::wstring _Name);

		// ==���� �߰��� ������Ʈ�� �߿����� Ư�� ������Ʈ�� ���� �� �� �ְ� ���ִ� �Լ�
		// ex) ���͸� ���� ���� ���� ���, <>�� ���� Ŭ���� ������ �� �� 
		// Ÿ���� �´��� �ƴ����� üũ�� ���� dynamic_cast�� ���� �� ���̴�.
		// �׷��� ���ӿ�����Ʈ�� ��� vector�� ��� �ִ� �༮�� 'LayerŬ����'��.
		// ������ Layer Ŭ������ �����ؼ� �ش� ���ӿ�����Ʈ vector�� �ݺ��� ������.
		template <typename T>
		std::vector<T*> FindObjectsOfType()
		{
			std::vector<T*> findObjs = {};
			for (Layer* layer : mLayers)
			{
				// ==== ���̾ ��� ���ӿ�����Ʈ���� �ݺ��� ������.
				auto gameObjs = layer->GetGameObjects();

				// �ݺ��� ������ ���ӿ�����Ʈ �߿����� �ش� Ÿ�Կ� �ش��ϴ� ������Ʈ���� ã�Ƴ���.
				for (GameObject* obj : gameObjs)
				{
					T* buff = dynamic_cast<T*>(obj);
					if (buff != nullptr)
						findObjs.push_back(buff);
				}
			}

			return findObjs;
		}
		Layer& GetLayer(eLayerType type) { return mLayers[(UINT)type]; }


		// ���� Ȱ��ȭ�� ������ Ư�� Ÿ���� ������Ʈ�� ã�Ƽ� ��ȯ���ִ� �Լ�
		template <typename T>
		T* FindObjInActiveLayer(eLayerType type)
		{
			Layer& activeLayer = GetLayer(type);
			auto gameObj = activeLayer.GetGameObjects();


			for (GameObject* obj : gameObj)
			{
				T* result = dynamic_cast<T*> (obj);

				if (nullptr != result)
					return result;
			}
			return nullptr;
		}



	private:
		std::vector<Layer> mLayers;
	};
}
