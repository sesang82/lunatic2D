#include "ssPlatform.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"


namespace ss
{
	Platform::Platform()
	{
	}

	Platform::~Platform()
	{
	}
	void Platform::Initialize()
	{
		mCollider = AddComponent<Collider2D>();
	

		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"DebugMaterial"));		

		GameObject::Initialize();
	}
	void Platform::Update()
	{
		GameObject::Update();
	}
	void Platform::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void Platform::Render()
	{
		GameObject::Render();
	}
}