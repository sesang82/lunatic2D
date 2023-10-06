#include "ssLayer.h"


namespace ss
{
	Layer::Layer()
	{
	}
	Layer::~Layer()
	{
		/*for (GameObject* gameObj : mGameObjects)
		{
			if (gameObj == nullptr)
				continue;

			delete gameObj;
			gameObj = nullptr;
		}*/


		for (int i = 0; i < mGameObjects.size(); ++i)
		{
			GameObject* gameObj = mGameObjects[i];
			if (gameObj == nullptr)
				continue;

			delete gameObj;
			mGameObjects[i] = nullptr;
		}

	}
	void Layer::Initialize()
	{
	}
	void Layer::Update()
	{
		//for (GameObject* gameObj : m_GameObjects)
		//{
		//	if (gameObj->GetState()
		//		!= GameObject::eState::Active)
		//		continue;

		//	gameObj->Update();
		//}

		for (int i = 0; i < mGameObjects.size(); ++i)
		{
			GameObject* gameObj = mGameObjects[i];

			if (gameObj->GetState() != GameObject::eState::Active)
				continue;

			gameObj->Update();
		}
	}

	void Layer::LateUpdate()
	{
		//for (GameObject* gameObj : m_GameObjects)
		//{
		//	if (gameObj->GetState()
		//		!= GameObject::eState::Active)
		//		continue;

		//	gameObj->LateUpdate();
		//}

		for (int i = 0; i < mGameObjects.size(); ++i)
		{
			GameObject* gameObj = mGameObjects[i];

			if (gameObj->GetState() != GameObject::eState::Active)
				continue;

			gameObj->LateUpdate();
		}
	}

	void Layer::Render()
	{
		//for (GameObject* gameObj : m_GameObjects)
		//{
		//	if (gameObj->GetState()
		//		!= GameObject::eState::Active)
		//		continue;

		//	gameObj->Render();
		//}

		for (int i = 0; i < mGameObjects.size(); ++i)
		{
			GameObject* gameObj = mGameObjects[i];

			if (gameObj->GetState() != GameObject::eState::Active)
				continue;

			gameObj->Render();
		}
	}

	void Layer::Destory()
	{
		// dead ������Ʈ ��Ƶα�
		std::set<GameObject*> deleteGameObj = {};
		for (int i = 0; i < mGameObjects.size(); ++i)
		{
			GameObject* gameObj = mGameObjects[i];

			if (gameObj->GetState() == GameObject::eState::Dead)
				deleteGameObj.insert(gameObj);
		}

		// daed ������Ʈ ���ܽ�Ű�� layer����
		typedef std::vector<GameObject*>::iterator GameObjectIter;
		for (GameObjectIter iter = mGameObjects.begin()
			; iter != mGameObjects.end(); )
		{
			std::set<GameObject*>::iterator deleteIter
				= deleteGameObj.find(*(iter));

			if (deleteIter != deleteGameObj.end())
			{
				iter = mGameObjects.erase(iter);
				continue;
			}

			iter++;
		}

		//�޸� ����
		for (GameObject* gameObj : deleteGameObj)
		{
			delete gameObj;
			gameObj = nullptr;
		}
	}

	void Layer::AddGameObject(GameObject* gameObj)
	{
		mGameObjects.push_back(gameObj);
	}

	void Layer::DeleteGameObject(GameObject* gameObj)
	{
		std::vector<GameObject*>::iterator iter = mGameObjects.begin();
		for (; iter != mGameObjects.end(); iter++)
		{
			if (gameObj == *iter)
			{
				mGameObjects.erase(iter);
				return;
			}
		}
	}
	GameObject* Layer::FindGameObjectByName(const std::wstring& name)
	{
		for (GameObject* obj : mGameObjects) {
			if (obj->GetName() == name) {
				return obj;
			}
		}
		return nullptr; // �̸��� ��ġ�ϴ� ��ü�� ������ nullptr ��ȯ

	}
}
