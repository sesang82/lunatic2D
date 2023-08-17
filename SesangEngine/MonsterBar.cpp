#include "MonsterBar.h"
#include "ssMeshRenderer.h"
#include "ssCharacterState.h"
#include "ssResources.h"


namespace ss
{
	MonsterBar::MonsterBar()
	: UI(eUIType::Progressbar)
	{
	}
	MonsterBar::~MonsterBar()
	{
	}
	void MonsterBar::Initialize()
	{
		mMeshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mMeshRenderer->SetMaterial(Resources::Find<Material>(L"MonsterFrameeMtrl")); // 	

		mTransform->SetScale(Vector3(20.f, 5.f, 1.f));

		GameObject::Initialize();
	}
	void MonsterBar::Update()
	{
		GameObject::Update();
	}
	void MonsterBar::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void MonsterBar::Render()
	{

		GameObject::Render();
	}
}