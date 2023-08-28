#include "ssStoneEyeProjectile.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssProjectileScript.h"
#include "ssMonsterAttackColScript.h"
#include "ssTime.h"

namespace ss
{
	StoneEyeProjectile::StoneEyeProjectile()
	{
		StoneEyeProjectile::Initialize();
	}
	StoneEyeProjectile::~StoneEyeProjectile()
	{
	}
	void StoneEyeProjectile::Initialize()
	{

		Transform* tr = GetComponent<Transform>();
		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mFirstPos = tr->GetPosition();
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"StoneEyeBorrowMtrl")); 

		
		tr->SetScale(Vector3(25.f, 10.f, 0.f));
		mTime = 0.0f;

		Collider2D* col = AddComponent<Collider2D>();
		col->SetSize(Vector2(1.f, 1.f));
		col->SetName(L"StoneEyeProjectileCol");
		
	
		 // 충돌 부품은 특정 인덱스에서 add했다가 제거하는 방식으로 쓰기 
		// 몬스터 Attack Col은 여기다가 붙이는거 아님. ProjectileScript 안에다가 충돌 시 어케되는지를 쓰면 됨 
		AddComponent<ProjectileScript>();


		Bullet::Initialize(); // 불렛에서 미리 추가해둔 걸 갖다 써야되기 때문에 불렛 이니셜라이저 먼저 호출하기
		

	}
	void StoneEyeProjectile::Update()
	{
		// 거리에 따른 삭제 
		mTime += Time::DeltaTime();
		Transform* tr = GetComponent<Transform>();

		// 몬스터의 첫 위치로부터 거리를 재서 -이하면 삭제
		float distance = (mFirstPos - tr->GetPosition()).Length();

		if (distance <= 15.0f)
					SetState(eState::Dead);	



		// 시간이 지나면 삭제 
		//if(mTime >= 1.6f)
		//	SetState(eState::Dead);


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
