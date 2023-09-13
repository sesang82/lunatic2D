#include "ssUI.h"
#include "ssMeshRenderer.h"
#include "ssCharacterState.h"
#include "ssResources.h"

namespace ss
{
	UI::UI()
		: mAnimator(nullptr)
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