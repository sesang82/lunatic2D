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


		// 충돌 부품은 특정 인덱스에서 add했다가 제거하는 방식으로 쓰기 
	   // 몬스터 Attack Col은 여기다가 붙이는거 아님. ProjectileScript 안에다가 충돌 시 어케되는지를 쓰면 됨 
		AddComponent<ArcherArrowScript>();


		Bullet::Initialize();


	}
	void ArcherArrow::Update()
	{
		// 거리에 따른 삭제 
		mTime += Time::DeltaTime();
		Transform* tr = GetComponent<Transform>();

		// 몬스터의 첫 위치로부터 거리를 재서 20이하면 삭제
		float distance = (mFirstPos - tr->GetPosition()).Length();

		if (distance <= 20.0f)
			SetState(eState::Dead);



		// 시간이 지나면 삭제 
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
