#include "ssEnergyball.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssTime.h"
#include "ssEnergyballScript.h"
#include "ssAnimator.h"

namespace ss
{
	Energyball::Energyball()
	{
		Energyball::Initialize();


	}
	Energyball::~Energyball()
	{
	}
	void Energyball::Initialize()
	{
		Transform* tr = GetComponent<Transform>();
		MeshRenderer* mr = AddComponent<MeshRenderer>();
		Animator* anim = AddComponent<Animator>();


		tr->SetScale(Vector3(144.f, 135.f, 1.0f));
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimationMaterial"));

		Collider2D* col = AddComponent<Collider2D>();
		col->SetSize(Vector2(1.f, 1.f));
		col->SetName(L"EnergyballCol");


		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Boss2-Boss2_1_Energyball_B_Parrying_Energying_Effect");
		anim->Create(L"Energyball_B_Parrying_Energying", Image1, Vector2(0.f, 0.f), Vector2(144.f, 135.f), 6, Vector2(144.f, 135.f));


		anim->PlayAnimation(L"Energyball_B_Parrying_Energying", true); // trigger 완성하면 지우기 

		

		Bullet::Initialize();
	}
	void Energyball::Update()
	{
		Bullet::Update();
	}
	void Energyball::LateUpdate()
	{
		Bullet::LateUpdate();
	}
	void Energyball::Render()
	{
		Bullet::Render();
	}



	
}