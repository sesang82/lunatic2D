#include "ssPlayerPistolBullet.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssTime.h"
#include "ssPistolBulletScript.h"

namespace ss
{
	PlayerPistolBullet::PlayerPistolBullet()
	{
		PlayerPistolBullet::Initialize();

	}
	PlayerPistolBullet::~PlayerPistolBullet()
	{
	}
	void PlayerPistolBullet::Initialize()
	{
		Transform* tr = GetComponent<Transform>();
		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mFirstPos = tr->GetPosition();
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimationMaterial"));


		mTime = 0.0f;

		Collider2D* col = AddComponent<Collider2D>();
		col->SetSize(Vector2(1.f, 1.f));
		col->SetName(L"PlayerPistolBulletObj");
   


		Bullet::Initialize();
	}


	void PlayerPistolBullet::Update()
	{

		mTime += Time::DeltaTime();


		//Transform* tr = GetComponent<Transform>();

		//// 몬스터의 첫 위치로부터 거리를 재서 20이하면 삭제
		//float distance = (mFirstPos - tr->GetPosition()).Length();

		//if (distance <= 20.0f)
		//	SetState(eState::Dead);



		// 시간이 지나면 삭제 
		if(mTime >= 1.6f)
			SetState(eState::Dead);


		Bullet::Update();

	}
	void PlayerPistolBullet::LateUpdate()
	{
		Bullet::LateUpdate();
	}
	void PlayerPistolBullet::Render()
	{
		Bullet::Render();
	}
}