#include "ssScene.h"
#include "ssrenderer.h"

namespace ss
{
	Scene::Scene()
	{
		// 씬이 생성될 때 레이어 타입 갯수에 맞게 vector 크기를 resize 시키게 해두심
		mLayers.resize((int)ss::enums::eLayerType::End);
	}
	Scene::~Scene()
	{
	}

	void Scene::Initialize()
	{

	}

	void Scene::Update()
	{
		for (Layer& layer : mLayers)
		{
			layer.Update();
		}
	}

	void Scene::LateUpdate()
	{
		for (Layer& layer : mLayers)
		{
			layer.LateUpdate();
		}
	}
	void Scene::Render()
	{
		for (Layer& layer : mLayers)
		{
			layer.Render();
		}
	}

	void Scene::Destroy()
	{
		for (Layer& layer : mLayers)
		{
			layer.Destory();
		}
	}

	void Scene::OnEnter()
	{
	
	}
	void Scene::OnExit()
	{
	}

	void Scene::AddGameObject(eLayerType type, GameObject* gameObj)
	{
		mLayers[(int)type].AddGameObject(gameObj);
	}
	void Scene::DeleteGameObject(eLayerType type, GameObject* gameObj)
	{
		mLayers[(int)type].DeleteGameObject(gameObj);
	}
	GameObject* Scene::FindGameObject(eLayerType _Type, GameObject* _GameObj)
	{
		Layer& layer = mLayers[(int)_Type];

		for (GameObject* obj : layer.GetGameObjects())
		{
			if (obj == _GameObj)
			{
				return obj;
			}
		}

		return nullptr;
	}
	GameObject* Scene::FindGameObject(eLayerType _Type, std::wstring _Name)
	{
		Layer& layer = mLayers[(int)_Type];

		for (GameObject* obj : layer.GetGameObjects())
		{
			if (obj->GetName() == _Name)
			{
				return obj;
			}
		}

		return nullptr;
	}
}
