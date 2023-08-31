#include "ssBackground.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssMeshRenderer.h"
#include "ssAnimator.h"

namespace ss
{
	Background::Background()
	{
		mr = AddComponent<MeshRenderer>();
	}
	Background::~Background()
	{
	}
	void Background::Initialize()
	{
		// 게임 오브젝트 객체에 연결된 컴포넌트를 인식시켜줘야하기 때문에 아래처럼 해야한다. 
		mr = GetComponent<MeshRenderer>();

		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimationMaterial"));

		AddComponent<Animator>();

		GameObject::Initialize();
	}
	void Background::Update()
	{
		GameObject::Update();
	}
	void Background::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void Background::Render()
	{
		GameObject::Render();
	}
}