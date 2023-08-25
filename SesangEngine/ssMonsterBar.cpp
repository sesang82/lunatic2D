#include "ssMonsterBar.h"
#include "ssMeshRenderer.h"
#include "ssCharacterState.h"
#include "ssResources.h"


namespace ss
{
	MonsterBar::MonsterBar()
		: mOwner(nullptr)
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
		mMeshRenderer->SetMaterial(Resources::Find<Material>(L"MonsterFrameMtrl")); 

	

		UI::Initialize();
	}
	void MonsterBar::Update()
	{
		Vector3 ArrowPos = mTransform->GetPosition();
		mTransform->SetPosition(ArrowPos);

		UI::Update();
	}
	void MonsterBar::LateUpdate()
	{
		UI::LateUpdate();
	}
	void MonsterBar::Render()
	{

		UI::Render();
	}
}