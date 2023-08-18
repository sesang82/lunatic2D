#pragma once
#include "ssEntity.h"
#include "ssComponent.h"
#include "ssScript.h"

namespace ss
{
	class GameObject : public Entity
	{
	public:
		enum class eState
		{
			Active,
			Paused,
			Dead, // ��Ҵٰ� Update�� ���� �� ����
		};

	public:
		GameObject();
		virtual ~GameObject();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

	private:
		eState mState;
		std::vector<Component*> mComponents;
		std::vector<Script*> mScripts;

		ss::enums::eLayerType mLayerType;

		// �θ� �ڽ�
		GameObject* mParent;
		std::vector<GameObject*> mChildren; // �ڽĵ��� ��Ƶδ� ����


	public:
		virtual void SetState(eState state) { mState = state; }
		virtual eState GetState() { return mState; }

		// �̰� ���� ��ũ��Ʈ�� &�� �ȵ����� �־ ���߿� �����Ǹ� & �����
		std::vector<Script*>& GetScripts() { return mScripts; }


		ss::enums::eLayerType GetLayerType() { return mLayerType; }


		// ==== �θ� �ڽ� ����(Transform Ŭ������ �ִ� �Ͱ� ����)
		std::vector<GameObject*> GetChildren() { return mChildren; }
		void AddChild(GameObject* children) { mChildren.push_back(children); }
		void EraseChild(GameObject* child);

		GameObject* FindChild(const std::wstring& name)
		{
			for (GameObject* child : mChildren)
			{
				if (child->GetName() == name)
				{
					return child;
				}
			}

			return nullptr;
		}
	

		
		void SetParent(GameObject* obj)
		{
			mParent = obj;
			Transform* tr = obj->GetComponent<Transform>();
			GetComponent<Transform>()->SetParent(tr);
			mParent->AddChild(this);
		}

		GameObject* GetParent() { return mParent; }

	public:

		// ���� ������Ʈ ��ü�� ����� ������Ʈ�� �νĽ����ִ� ������ �Ѵ�. 
		template <typename T>
		T* GetComponent()
		{
			T* component;
			for (Component* comp : mComponents)
			{
				component = dynamic_cast<T*>(comp);
				if (nullptr != component)
					return component;
			}

			for (Script* script : mScripts)
			{
				component = dynamic_cast<T*>(script);
				if (nullptr != component)
					return component;
			}

			return nullptr;
		}

		// ���� �ִ� GetComponent �Լ��� ���࿡ �浹ü�� 2�� ���� �ִ� �� ���� ���� ���� �� ���ٸ�
		// �浹ü�� 1���� ������ �� �ٷ� retunó�� �ǹ����� ���������� 1���� ���� �� �� ���� ���̴�.
		// �� ������ �Ʒ��� '���� ���� ������Ʈ��'�� ������ �� �ִ� �Լ��� ������
		template <typename T>
		const std::vector<T*> GetComponents()
		{
			std::vector<T*> comps;

			T* component;
			for (Component* comp : mComponents)
			{
				component = dynamic_cast<T*>(comp);
				if (nullptr != component)
					comps.push_back(component);
			}

			for (Script* script : mScripts)
			{
				component = dynamic_cast<T*>(script);
				if (nullptr != component)
					comps.push_back(component);
			}

			return comps;
		}


		template <typename T>
		T* GetComponentByName(const std::wstring& name)
		{
			for (Component* comp : mComponents)
			{
				if (comp->GetName() == name)
				{
					T* component = dynamic_cast<T*>(comp);
					if (component)
						return component;
				}
			}

			for (Script* script : mScripts)
			{
				if (script->GetName() == name)
				{
					T* component = dynamic_cast<T*>(script);
					if (component)
						return component;
				}
			}

			return nullptr;
		}

		template <typename T>
		T* AddComponent()
		{
			T* comp = new T();
			
			// ������Ʈ�� �� ��������, �� �������� script�� �߰��ؾ��Ѵ�.
			// Script�� ������Ʈ ���� �ݺ����� ���ų�, �ƴϸ� �� �������� �ݺ����� ���ƾ��Ѵ�.
			Component* buff
				= dynamic_cast<Component*>(comp);
			Script* script
				= dynamic_cast<Script*>(buff);

			if (nullptr == buff)
				return nullptr;

			if (nullptr == script)
				mComponents.push_back(buff);
			else
				mScripts.push_back(script);
			
			comp->SetOwner(this);
			comp->Initialize();

			return comp;
		}

		template <typename T>
		void RemoveComponent()
		{
			// ������Ʈ Ÿ���� ã���ϴ�.
			for (auto it = mComponents.begin(); it != mComponents.end();)
			{
				T* comp = dynamic_cast<T*>(*it);
				if (comp)
				{
					// ������Ʈ�� �����̳ʿ��� �����մϴ�.
					it = mComponents.erase(it);

					// �޸𸮸� �����մϴ�.
					delete comp;
				}
				else
				{
					++it;
				}
			}

			// ��ũ��Ʈ�� ���� Ư���� ������Ʈ�� ������ �����̳ʿ��� ������ �� �����Ƿ�, �װ͵� Ȯ���ؾ� �� �� �ֽ��ϴ�.
			for (auto it = mScripts.begin(); it != mScripts.end();)
			{
				T* script = dynamic_cast<T*>(*it);
				if (script)
				{
					// ��ũ��Ʈ�� �����̳ʿ��� �����մϴ�.
					it = mScripts.erase(it);

					// �޸𸮸� �����մϴ�.
					delete script;
				}
				else
				{
					++it;
				}
			}
		}
	

	};
}
