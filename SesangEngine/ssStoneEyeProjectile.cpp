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
		Bullet::Initialize(); // �ҷ����� �̸� �߰��ص� �� ���� ��ߵǱ� ������ �ҷ� �̴ϼȶ����� ���� ȣ���ϱ�

		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"StoneEyeBorrowMtrl")); 


		AddComponent<ProjectileScript>();
		 // �浹 ��ǰ�� Ư�� �ε������� add�ߴٰ� �����ϴ� ������� ���� 
		// ���� Attack Col�� ����ٰ� ���̴°� �ƴ�. ProjectileScript �ȿ��ٰ� �浹 �� ���ɵǴ����� ���� �� 



	}
	void StoneEyeProjectile::Update()
	{
		Bullet::Initialize();
	}
	void StoneEyeProjectile::LateUpdate()
	{
		Bullet::Initialize();
	}
	void StoneEyeProjectile::Render()
	{
		Bullet::Initialize();
	}
}
