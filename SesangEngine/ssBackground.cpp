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
		// ���� ������Ʈ ��ü�� ����� ������Ʈ�� �νĽ�������ϱ� ������ �Ʒ�ó�� �ؾ��Ѵ�. 
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