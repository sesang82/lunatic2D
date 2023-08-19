#pragma once
#include "ssLayer.h"
#include "ssGameObject.h"
#include "ssScene.h"
#include "ssSceneManager.h"
#include "ssTransform.h"


namespace ss::object
{
	template <typename T>
	static __forceinline T* Instantiate(enums::eLayerType layer, std::wstring name)
	{
		T* gameObj = new T();
		Scene* scene = SceneManager::GetActiveScene();
		scene->AddGameObject(layer, gameObj);
		gameObj->SetName(name);

		return gameObj;
	}

	template <typename T>
	static __forceinline T* Instantiate(Vector3 pos, enums::eLayerType layer, std::wstring name)
	{
		T* gameObj = new T();
		Transform* tr = gameObj->GetComponent<Transform>();
		tr->SetPosition(pos);

		Scene* scene = SceneManager::GetActiveScene();
		scene->AddGameObject(layer, gameObj);

		gameObj->SetName(name);

		return gameObj;
	}

	template <typename T>
	static __forceinline T* Instantiate(Vector3 pos, enums::eLayerType layer)
	{
		T* gameObj = new T();
		Transform* tr = gameObj->GetComponent<Transform>();
		tr->SetPosition(pos);

		Scene* scene = SceneManager::GetActiveScene();
		scene->AddGameObject(layer, gameObj);

		return gameObj;
	}

	template <typename T>
	static __forceinline T* Instantiate(Vector3 pos, Vector3 rotate, enums::eLayerType layer, std::wstring name)
	{
		T* gameObj = new T();
		Transform* tr = gameObj->GetComponent<Transform>();
		tr->SetPosition(pos);
		tr->SetRotation(rotate);

		Scene* scene = SceneManager::GetActiveScene();
		scene->AddGameObject(layer, gameObj);

		gameObj->SetName(name);

		return gameObj;
	}

	template <typename T>
	static __forceinline T* Instantiate(Vector3 pos, Vector3 rotate, Vector3 scale, enums::eLayerType layer, std::wstring name)
	{
		T* gameObj = new T();
		Transform* tr = gameObj->GetComponent<Transform>();
		tr->SetPosition(pos);
		tr->SetRotation(rotate);
		tr->SetScale(scale);

		Scene* scene = SceneManager::GetActiveScene();
		scene->AddGameObject(layer, gameObj);

		gameObj->SetName(name);

		return gameObj;
	}

	// 부모 설정 기능 추가 
	template <typename T>
	static T* Instantiate(enums::eLayerType layer, GameObject* parent, std::wstring name)
	{
		T* gameObj = new T();
		Scene* scene = SceneManager::GetActiveScene();
		scene.AddGameObject(gameObj, layer);

		gameObj->SetName(name);

		Transform* tr = gameObj->GetComponent<Transform>();
		tr->SetParent(parent); // 부모 설정 기능

		return gameObj;
	}

	static __forceinline void Destroy(GameObject* gameObj)
	{
		gameObj->SetState(ss::GameObject::eState::Dead);
	}

}