#include "ssItem.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssMeshRenderer.h"
#include "ssAnimator.h"
#include "ssItemScript.h"

namespace ss
{
	Item::Item()
	{
		Item::Initialize(); // 생성자에서 안해주면 렌더링 가려짐 
	}
	Item::~Item()
	{
	}
	void Item::Initialize()
	{
		Transform* tr = GetComponent<Transform>();
		tr->SetScale(1.f, 1.f, 1.f);
		tr->SetPosition(0.f, 0.f, 350.f);

		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimationMaterial"));


		Animator* ani = AddComponent<Animator>();
		AddComponent<Collider2D>();


		GameObject::Initialize();

	}
	void Item::Update()
	{
		GameObject::Update();
	}
	void Item::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void Item::Render()
	{
		GameObject::Render();
	}
}