#include "ssStoneEyeProjectile.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssProjectileScript.h"
#include "ssMonsterAttackColScript.h"

namespace ss
{
	StoneEyeProjectile::StoneEyeProjectile()
	{
	}
	StoneEyeProjectile::~StoneEyeProjectile()
	{
	}
	void StoneEyeProjectile::Initialize()
	{

		Transform* tr = GetComponent<Transform>();
		MeshRenderer* mr = AddComponent<MeshRenderer>();

		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"StoneEyeBorrowMtrl")); 


		tr->SetScale(Vector3(25.f, 10.f, 0.f));
		
	
		 // �浹 ��ǰ�� Ư�� �ε������� add�ߴٰ� �����ϴ� ������� ���� 
		// ���� Attack Col�� ����ٰ� ���̴°� �ƴ�. ProjectileScript �ȿ��ٰ� �浹 �� ���ɵǴ����� ���� �� 
		AddComponent<ProjectileScript>();


		Bullet::Initialize(); // �ҷ����� �̸� �߰��ص� �� ���� ��ߵǱ� ������ �ҷ� �̴ϼȶ����� ���� ȣ���ϱ�

	}
	void StoneEyeProjectile::Update()
	{
		Bullet::Update();
	}
	void StoneEyeProjectile::LateUpdate()
	{
		Bullet::LateUpdate();
	}
	void StoneEyeProjectile::Render()
	{
		Bullet::Render();
	}
}
