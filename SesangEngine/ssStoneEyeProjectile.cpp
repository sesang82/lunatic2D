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
		
	
		 // 충돌 부품은 특정 인덱스에서 add했다가 제거하는 방식으로 쓰기 
		// 몬스터 Attack Col은 여기다가 붙이는거 아님. ProjectileScript 안에다가 충돌 시 어케되는지를 쓰면 됨 
		AddComponent<ProjectileScript>();


		Bullet::Initialize(); // 불렛에서 미리 추가해둔 걸 갖다 써야되기 때문에 불렛 이니셜라이저 먼저 호출하기
		

	}
	void StoneEyeProjectile::Update()
	{
	/*	mTime += Time::DeltaTime();
		Transform* tr = GetComponent<Transform>();

		float distance = (mFirstPos - tr->GetPosition()).Length();

		if (distance >= 20.0f)
					SetState(eState::Dead);	*/

		//mTime += Time::DeltaTime();


		// 시간이 지나면 삭제 
		mTime += Time::DeltaTime();

		if(mTime >= 0.3f)
			SetState(eState::Dead);


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
