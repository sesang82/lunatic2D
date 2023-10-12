#include "ssGameObject.h"
#include "ssRenderer.h"
#include "ssGraphicDevice_Dx11.h"
#include "ssTransform.h"

namespace ss
{
	GameObject::GameObject()
		: mState(eState::Active)
	
	{
		// 게임오브젝트는 생성될 때 기본적으로 트랜스폼을 갖고 있게함 
		AddComponent<Transform>();
	}

	GameObject::~GameObject()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue; // nullptr이면 계속 for문을 돌도록 함

			delete comp;
			comp = nullptr;
		}

		for (Script* script : mScripts)
		{
			if (script == nullptr)
				continue;

			delete script;
			script = nullptr;
		}
	}

	void GameObject::Initialize()
	{

	}

	void GameObject::Update()
	{

		for (Component* comp : mComponents)
		{
			comp->Update();
		}

		for (Script* script : mScripts)
		{
			script->Update();
		}

		for (auto* child : mChildren)
		{
			child->Update();
		}
	}

	void GameObject::LateUpdate()
	{
		for (Component* comp : mComponents)
		{
			comp->LateUpdate();
		}

		for (Script* script : mScripts)
		{
			script->LateUpdate();
		}

		for (auto* child : mChildren)
		{
			child->LateUpdate();
		}
	}

	void GameObject::Render()
	{
		for (Component* comp : mComponents)
		{
			comp->Render();
		}
		//상수버퍼로 위치정보 크기정보, 색깔, 업데이트 해줘야한다.

		for (Script* script : mScripts)
		{
			script->Render();
		}

		for (auto* child : mChildren)
		{
			child->Render();
		}
	}
	void GameObject::EraseChild(GameObject* child)
	{
		// 벡터 반복자 선언 및 시작점을 가리키도록 함
		std::vector<GameObject*>::iterator iter = mChildren.begin();


		// 1. 이터레이터로 칠드런의 시작과 끝까지 반복문을 돌린다.
		for (iter; iter != mChildren.end();)
		{
			// 인자로 들어온 child와 iter가 가리키는 값이 같으면
			if (*iter == child)
				// iter가 가리키는 값을 지운다. erase할 때는 반드시 안전을 위해
				// erase가 반환하는 iter값을 다시 받아서 정보를 재갱신해야된다.
				iter = mChildren.erase(iter); 

			else // 찾는 값이 없다면
				iter++; // iter가 컨테이너의 다음 요소로 넘어가도록 한다.
		}


	}
}
