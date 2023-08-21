#include "ssRangeCollider.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssCharacterState.h"
#include "ssAnimator.h"

ss::RangeCollider::RangeCollider()
{
}

ss::RangeCollider::~RangeCollider()
{
}

void ss::RangeCollider::Initialize()
{
	Transform* tr = GetComponent<Transform>();
	tr->SetScale(Vector3(1.f, 1.f, 0.f)); // 0만 안되면 되서 최소한으로 둠 



	MeshRenderer* mr = AddComponent<MeshRenderer>();
	mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
	mr->SetMaterial(Resources::Find<Material>(L"DebugMaterial"));

	Collider2D* col = AddComponent<Collider2D>();

	




	GameObject::Initialize();
}

void ss::RangeCollider::Update()
{
	GameObject::Update();
}

void ss::RangeCollider::LateUpdate()
{
	GameObject::LateUpdate();
}

void ss::RangeCollider::Render()
{
	GameObject::Render();
}
