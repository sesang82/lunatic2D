#include "ssItemScript.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssGameObject.h"
#include "ssAnimator.h"
#include "ssMonster.h"
#include "ssSceneManager.h"
#include "ssrenderer.h"
#include "ssInput.h"
#include "ssGameState.h"

namespace ss
{
	ItemScript::ItemScript()
		: mOwnerObj(nullptr)
	{


	}
	ItemScript::~ItemScript()
	{
	}

	void ItemScript::Initialize()
	{
		MeshRenderer* mr = GetOwner()->GetComponent<MeshRenderer>();
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Animator* anim = GetOwner()->GetComponent<Animator>();
		Collider2D* col = GetOwner()->GetComponent<Collider2D>();

		mState = GameState::GetInst().GetState(L"Player");

		if (GetOwner()->GetName() == L"overload_Item")
		{
			mr->SetMaterial(Resources::Find<Material>(L"Item_OverloadMtrl"));
			tr->SetScale(Vector3(24.f, 24.f, 0.f));
			col->SetSize(Vector2(1.f, 1.3f));
		}

	}


	void ItemScript::Update()
	{


		if (GetOwner()->GetName() == L"overload_Item")
		{
			if (Input::GetKeyState(eKeyCode::F) == eKeyState::Down)
			{
				mState->SetCurOverload(100);
				GetOwner()->SetState(GameObject::eState::Dead);
		    }
		}



	}
	void ItemScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void ItemScript::OnCollisionStay(Collider2D* other)
	{
	}
	void ItemScript::OnCollisionExit(Collider2D* other)
	{
	}
}