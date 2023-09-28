#include "ssEnergyball.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssTime.h"
#include "ssEnergyballScript.h"
#include "ssAnimator.h"
#include "ssobject.h"
#include "ssGoddnessScript.h"

namespace ss
{
	Energyball::Energyball()
		: mAnimator(nullptr)
		, mTransform(nullptr)
		, mbTest(false)
	{
		Energyball::Initialize();


	}
	Energyball::~Energyball()
	{
	}
	void Energyball::Initialize()
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


		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_S_Parrying_Spawn_Effect");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Boss2_1_Energyball_S_Parrying_AfterSpawn_Effect");

		mAnimator->Create(L"Energyball_S_Parrying_Spawn", Image1, Vector2(0.f, 0.f), Vector2(58.f, 54.f), 6, Vector2(58.f, 54.f));
		mAnimator->Create(L"Energyball_S_Parrying_Energying", Image2, Vector2(0.f, 0.f), Vector2(58.f, 54.f), 6, Vector2(58.f, 54.f));


		mAnimator->PlayAnimation(L"Energyball_S_Parrying_Spawn", false); // trigger 완성하면 지우기 

		

		Bullet::Initialize();
	}
	void Energyball::Update()
	{
		
		if (mAnimator->GetCurActiveAnimation()->IsComplete())
		{
			mAnimator->PlayAnimation(L"Energyball_S_Parrying_Energying", true);

			
			//GoddnessScript* boss = mOwner->GetComponent<GoddnessScript>();

			//Vector3 bossPos = mOwner->GetComponent<Transform>()->GetPosition();



			if (!mbTest)
			{
				object::Instantiate<Energyball>(Vector3(mTransform->GetPosition().x, mTransform->GetPosition().y - 30.f, 300.f), eLayerType::Collision, L"Parrying_S_EnergyballObj2");
				mbTest = true;
			}



		}

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