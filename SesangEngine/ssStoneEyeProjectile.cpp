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
		Bullet::Initialize(); // 불렛에서 미리 추가해둔 걸 갖다 써야되기 때문에 불렛 이니셜라이저 먼저 호출하기

		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"StoneEyeBorrowMtrl")); 


		AddComponent<ProjectileScript>();
		 // 충돌 부품은 특정 인덱스에서 add했다가 제거하는 방식으로 쓰기 
		// 몬스터 Attack Col은 여기다가 붙이는거 아님. ProjectileScript 안에다가 충돌 시 어케되는지를 쓰면 됨 



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
