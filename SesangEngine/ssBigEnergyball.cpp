#include "ssBigEnergyball.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssTime.h"
#include "ssSmallEnergyballScript.h"
#include "ssAnimator.h"
#include "ssobject.h"
#include "ssGoddnessScript.h"
#include "ssPlayer.h"
#include "ssBigEnergyballScript.h"

namespace ss
{
	int BigEnergyball::miSpawnedBallCount = 0;

	BigEnergyball::BigEnergyball()

	{
		BigEnergyball::Initialize();

	}
	BigEnergyball::~BigEnergyball()
	{
	}
	void BigEnergyball::Initialize()
	{
		mTransform = GetComponent<Transform>();
		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mAnimator = AddComponent<Animator>();


		mTransform->SetScale(Vector3(58.f, 54.f, 1.0f));
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimationMaterial"));

		Collider2D* col = AddComponent<Collider2D>();
		col->SetSize(Vector2(1.f, 1.f));
		col->SetName(L"EnergyballCol");


		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_B_noParrying_Spawn_Effect");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_B_noParrying_Energying_Effect");

		mAnimator->Create(L"Energyball_B_NoParrying_Spawn", Image1, Vector2(0.f, 0.f), Vector2(58.f, 54.f), 6, Vector2(58.f, 54.f), Vector2::Zero, 0.08f);
		mAnimator->Create(L"Energyball_B_NoParrying_Energying", Image2, Vector2(0.f, 0.f), Vector2(58.f, 54.f), 6, Vector2(58.f, 54.f), Vector2::Zero, 0.08f);


		mAnimator->PlayAnimation(L"Energyball_S_Parrying_Spawn", false); // trigger 완성하면 지우기 

		AddComponent<BigEnergyballScript>();


		Bullet::Initialize();
	}
	void BigEnergyball::Update()
	{
		Bullet::Update();
	}
	void BigEnergyball::LateUpdate()
	{
		Bullet::LateUpdate();
	}
	void BigEnergyball::Render()
	{
		Bullet::Render();
	}
	void BigEnergyball::CreateEnergyball()
	{
	}
}