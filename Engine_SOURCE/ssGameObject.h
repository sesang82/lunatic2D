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
			Dead, // 모았다가 Update가 끝난 후 삭제
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

		// 부모 자식
		GameObject* mParent;
		std::vector<GameObject*> mChildren; // 자식들을 모아두는 공간


	public:
		virtual void SetState(eState state) { mState = state; }
		virtual eState GetState() { return mState; }

		// 이거 나는 스크립트를 &로 안돌리고 있어서 나중에 문제되면 & 지우기
		std::vector<Script*>& GetScripts() { return mScripts; }


		ss::enums::eLayerType GetLayerType() { return mLayerType; }


		// ==== 부모 자식 관련(Transform 클래스에 있는 것과 별개)
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

		// 게임 오브젝트 객체에 연결된 컴포넌트를 인식시켜주는 역할을 한다. 
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

		// 위에 있는 GetComponent 함수는 만약에 충돌체가 2개 겹쳐 있는 걸 갖고 오고 싶을 때 쓴다면
		// 충돌체를 1개만 가져온 후 바로 retun처리 되버려서 최종적으로 1개만 갖고 올 수 있을 것이다.
		// 이 때문에 아래에 '여러 개의 컴포넌트들'을 가져올 수 있는 함수도 만들어둠
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
			
			// 컴포넌트가 다 돌고나서야, 맨 마지막에 script를 추가해야한다.
			// Script는 컴포넌트 전에 반복문을 돌거나, 아니면 맨 마지막에 반복문을 돌아야한다.
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
			// 컴포넌트 타입을 찾습니다.
			for (auto it = mComponents.begin(); it != mComponents.end();)
			{
				T* comp = dynamic_cast<T*>(*it);
				if (comp)
				{
					// 컴포넌트를 컨테이너에서 제거합니다.
					it = mComponents.erase(it);

					// 메모리를 해제합니다.
					delete comp;
				}
				else
				{
					++it;
				}
			}

			// 스크립트와 같은 특별한 컴포넌트는 별도의 컨테이너에서 관리할 수 있으므로, 그것도 확인해야 할 수 있습니다.
			for (auto it = mScripts.begin(); it != mScripts.end();)
			{
				T* script = dynamic_cast<T*>(*it);
				if (script)
				{
					// 스크립트를 컨테이너에서 제거합니다.
					it = mScripts.erase(it);

					// 메모리를 해제합니다.
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
