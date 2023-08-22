#include "ssUI.h"
#include "ssMeshRenderer.h"
#include "ssCharacterState.h"

namespace ss
{
	UI::UI(eUIType type)
		: mUIType(type)
		, mAnimator(nullptr)
		, mRigidbody(nullptr)
		, mTransform(nullptr)
		, mCollider(nullptr)
		, mMeshRenderer(nullptr)
		, mCharacterState(nullptr)
	{
	}


	UI::~UI()
	{
	}

	void UI::Initialize()
	{


	GameObject::Initialize();


	}
	void UI::Update()
	{
		GameObject::Update();
	}
	void UI::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void UI::Render()
	{
		GameObject::Render();
	}
}