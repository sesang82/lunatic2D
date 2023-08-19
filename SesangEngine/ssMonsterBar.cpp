#include "ssMonsterBar.h"
#include "ssMeshRenderer.h"
#include "ssCharacterState.h"
#include "ssResources.h"


namespace ss
{
	MonsterBar::MonsterBar()
	{
	}
	MonsterBar::~MonsterBar()
	{
	}
	void MonsterBar::Initialize()
	{
		mTransform = GetComponent<Transform>();
		mTransform->SetScale(Vector3(20.f, 5.f, 1.f));


		mMeshRenderer = AddComponent<MeshRenderer>();
		mMeshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mMeshRenderer->SetMaterial(Resources::Find<Material>(L"MonsterFrameMtrl")); // 	

	

		Progressbar::Initialize();
	}
	void MonsterBar::Update()
	{
		Progressbar::Update();
	}
	void MonsterBar::LateUpdate()
	{
		Progressbar::LateUpdate();
	}
	void MonsterBar::Render()
	{

		Progressbar::Render();
	}
}