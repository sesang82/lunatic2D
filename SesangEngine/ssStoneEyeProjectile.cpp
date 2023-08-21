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
		
	
		 // �浹 ��ǰ�� Ư�� �ε������� add�ߴٰ� �����ϴ� ������� ���� 
		// ���� Attack Col�� ����ٰ� ���̴°� �ƴ�. ProjectileScript �ȿ��ٰ� �浹 �� ���ɵǴ����� ���� �� 
		AddComponent<ProjectileScript>();


		Bullet::Initialize(); // �ҷ����� �̸� �߰��ص� �� ���� ��ߵǱ� ������ �ҷ� �̴ϼȶ����� ���� ȣ���ϱ�
		

	}
	void StoneEyeProjectile::Update()
	{
	/*	mTime += Time::DeltaTime();
		Transform* tr = GetComponent<Transform>();

		float distance = (mFirstPos - tr->GetPosition()).Length();

		if (distance >= 20.0f)
					SetState(eState::Dead);	*/

		//mTime += Time::DeltaTime();


		// �ð��� ������ ���� 
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
