#include "ssArcherArrow.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssProjectileScript.h"
#include "ssMonsterAttackColScript.h"
#include "ssTime.h"
#include "ssArcherArrowScript.h"

namespace ss
{
	ArcherArrow::ArcherArrow()
	{
		ArcherArrow::Initialize();
	}
	ArcherArrow::~ArcherArrow()
	{
	}
	void ArcherArrow::Initialize()
	{

		Transform* tr = GetComponent<Transform>();
		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mFirstPos = tr->GetPosition();
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"Archer_ArrowMtrl"));


		tr->SetScale(Vector3(28.f, 7.f, 0.f));
		mTime = 0.0f;

		Collider2D* col = AddComponent<Collider2D>();
		col->SetSize(Vector2(1.f, 1.f));
		col->SetName(L"ArcherArrowCol");


		// �浹 ��ǰ�� Ư�� �ε������� add�ߴٰ� �����ϴ� ������� ���� 
	   // ���� Attack Col�� ����ٰ� ���̴°� �ƴ�. ProjectileScript �ȿ��ٰ� �浹 �� ���ɵǴ����� ���� �� 
		AddComponent<ArcherArrowScript>();


		Bullet::Initialize();


	}
	void ArcherArrow::Update()
	{
		// �Ÿ��� ���� ���� 
		mTime += Time::DeltaTime();
		Transform* tr = GetComponent<Transform>();

		// ������ ù ��ġ�κ��� �Ÿ��� �缭 20���ϸ� ����
		float distance = (mFirstPos - tr->GetPosition()).Length();

		if (distance <= 20.0f)
			SetState(eState::Dead);



		// �ð��� ������ ���� 
		//if(mTime >= 1.6f)
		//	SetState(eState::Dead);


		Bullet::Update();
	}
	void ArcherArrow::LateUpdate()
	{


		Bullet::LateUpdate();
	}
	void ArcherArrow::Render()
	{
		Bullet::Render();
	}
}
