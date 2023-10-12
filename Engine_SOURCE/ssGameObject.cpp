#include "ssGameObject.h"
#include "ssRenderer.h"
#include "ssGraphicDevice_Dx11.h"
#include "ssTransform.h"

namespace ss
{
	GameObject::GameObject()
		: mState(eState::Active)
	
	{
		// ���ӿ�����Ʈ�� ������ �� �⺻������ Ʈ�������� ���� �ְ��� 
		AddComponent<Transform>();
	}

	GameObject::~GameObject()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue; // nullptr�̸� ��� for���� ������ ��

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
		//������۷� ��ġ���� ũ������, ����, ������Ʈ ������Ѵ�.

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
		// ���� �ݺ��� ���� �� �������� ����Ű���� ��
		std::vector<GameObject*>::iterator iter = mChildren.begin();


		// 1. ���ͷ����ͷ� ĥ�己�� ���۰� ������ �ݺ����� ������.
		for (iter; iter != mChildren.end();)
		{
			// ���ڷ� ���� child�� iter�� ����Ű�� ���� ������
			if (*iter == child)
				// iter�� ����Ű�� ���� �����. erase�� ���� �ݵ�� ������ ����
				// erase�� ��ȯ�ϴ� iter���� �ٽ� �޾Ƽ� ������ �簻���ؾߵȴ�.
				iter = mChildren.erase(iter); 

			else // ã�� ���� ���ٸ�
				iter++; // iter�� �����̳��� ���� ��ҷ� �Ѿ���� �Ѵ�.
		}


	}
}
