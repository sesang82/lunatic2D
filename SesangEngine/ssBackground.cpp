#include "ssBackground.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssMeshRenderer.h"
#include "ssAnimator.h"
#include "ssParalloxScript.h"
#include "ssMonster.h"
#include "ssGoddnessScript.h"

namespace ss
{
	Background::Background()
		: mBoss(nullptr)
	{
		mr = AddComponent<MeshRenderer>();
	

	}
	Background::~Background()
	{
	}
	void Background::Initialize()
	{
		// 게임 오브젝트 객체에 연결된 컴포넌트를 인식시켜줘야하기 때문에 아래처럼 해야한다. 
		mr = GetComponent<MeshRenderer>();

		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"torchAnimMtrl"));

		anim = AddComponent<Animator>();

		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Fire_turnonstart");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Fire_turnon");
		std::shared_ptr<ss::graphics::Texture> Image3 = Resources::Find<ss::graphics::Texture>(L"Fire_Base");
		
		std::shared_ptr<ss::graphics::Texture> Image4 = Resources::Find<ss::graphics::Texture>(L"Boss2_GroundFloatform");
		std::shared_ptr<Texture> Image5 = Resources::Find<Texture>(L"Moon1Statue");

		// ==== 1페이즈 석상 
		anim->Create(L"fire_turnOnStart", Image1, Vector2(0.f, 0.f), Vector2(48.f, 64.f), 7, Vector2(48.f, 64.f));
		anim->Create(L"fire_turnOnLoop", Image2, Vector2(0.f, 0.f), Vector2(48.f, 64.f), 8, Vector2(48.f, 64.f));
		anim->Create(L"fire_turnoff", Image3, Vector2(0.f, 0.f), Vector2(48.f, 64.f), 1, Vector2(48.f, 64.f));

		anim->Create(L"boss2_GroundplatfromStart", Image4, Vector2(0.f, 0.f), Vector2(100.f, 24.f), 12, Vector2(100.f, 24.f), Vector2::Zero, 0.12f);
		anim->Create(L"Statue_Left_Anim", Image5, Vector2(0.f, 0.f), Vector2(212.f, 124.f), 6, Vector2(212.f, 124.f), Vector2::Zero);
		anim->Create(L"Statue_Right_Anim", Image5, Vector2(0.f, 0.f), Vector2(212.f, 124.f), 6, Vector2(212.f, 124.f), Vector2::Zero, 0.1f, true);

		if (GetName() == L"Boss2_Fire")
		{
			anim->PlayAnimation(L"fire_turnOnLoop", true);
		}


		else if (GetName() == L"PlatformLTObj"
			|| GetName() == L"PlatformLBObj"
			|| GetName() == L"PlatformRTObj"
			|| GetName() == L"PlatformRBObj")
		{

			anim->PlayAnimation(L"boss2_GroundplatfromStart", false);
			GetComponent<Transform>()->SetScale(Vector3(100.f, 24.f, 1.f));

		}

		else if (GetName() == L"Left_Statue")
		{
			anim->PlayAnimation(L"Statue_Left_Anim", true);
		}

		else if (GetName() == L"Right_Statue")
		{
			anim->PlayAnimation(L"Statue_Right_Anim", true);
		}

		GameObject::Initialize();
	}
	void Background::Update()
	{
		if (GetName() == L"PlatformLTObj"
			|| GetName() == L"PlatformLBObj"
			|| GetName() == L"PlatformRTObj"
			|| GetName() == L"PlatformRBObj")
		{

			if (anim->GetCurActiveAnimation()->IsComplete())
			{
				mr->SetMaterial(Resources::Find<Material>(L"Boss2_GroundPlatformImage_Mtrl"));
			}
		}
		

		GameObject::Update();


	}
	void Background::LateUpdate()
	{

	
		GameObject::LateUpdate();
	}
	void Background::Render()
	{
		GameObject::Render();
	}
	
}