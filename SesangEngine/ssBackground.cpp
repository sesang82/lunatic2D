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
		mr->SetMaterial(Resources::Find<Material>(L"Fire_turnon_Mtrl"));

		anim = AddComponent<Animator>();

		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Fire_turnonstart");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Fire_turnon");
		std::shared_ptr<ss::graphics::Texture> Image3 = Resources::Find<ss::graphics::Texture>(L"Fire_Base");

		// ==== 1페이즈 석상 
		anim->Create(L"fire_turnOnStart", Image1, Vector2(0.f, 0.f), Vector2(48.f, 64.f), 7, Vector2(48.f, 64.f));
		anim->Create(L"fire_turnOnLoop", Image2, Vector2(0.f, 0.f), Vector2(48.f, 64.f), 8, Vector2(48.f, 64.f));
		anim->Create(L"fire_turnoff", Image3, Vector2(0.f, 0.f), Vector2(48.f, 64.f), 1, Vector2(48.f, 64.f));

	

		if (GetName() == L"Boss2_Fire")
		{
			anim->PlayAnimation(L"fire_turnOnLoop", true);
		}



		GameObject::Initialize();
	}
	void Background::Update()
	{

		

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