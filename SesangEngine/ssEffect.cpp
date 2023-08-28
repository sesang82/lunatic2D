#include "ssEffect.h"
#include "ssResources.h"
#include "ssMesh.h"
#include "ssMeshRenderer.h"
#include "ssAnimator.h"
#include "ssStoneEyeScript.h"

namespace ss
{

	Effect::Effect()
	{
		Effect::Initialize(); // 생성자에서 안해주면 렌더링 가려짐 
	}

	Effect::~Effect()
	{
	}

	void Effect::Initialize()
	{
		Transform* tr = GetComponent<Transform>();
		tr->SetScale(1.f, 1.f, 1.f);
		tr->SetPosition(0.f, 0.f, 350.f);

		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimationMaterial"));

		Animator* ani = AddComponent<Animator>();


		// === 
		// 		// 각 충돌체 크기는 달라지는게 있다면 fsm에서 각각 조정해주기 
		// === 
		// 플레이어 애니메이션은 좀 특수하므로, 무기 별로 오프셋 값 다르게 주되 백사이즈는	 동일하게 주기. (사이즈 값은 틀려도 됨) 
		// 애니메이션 나중에 플레이어 다 완성되면 
		//  LT, 1프레임 사를 사이즈, 자를 갯수, 오프셋(y값은 -값이 클수록 올라감), 재생시간, 뒤집기

		// === 
		// 애니메이션 나중에 플레이어 다 완성되면 
		//  LT, 1프레임 사를 사이즈, 자를 갯수, 백사이즈, 오프셋(y값은 -값이 클수록 올라감)
		//ani->Create(L"StunEffectAnim", Stun, Vector2(0.f, 0.f), Vector2(27.f, 15.f), 5, Vector2(27.f, 15.f), Vector2(0.f, -19.f));


		GameObject::Initialize();
	}

	void Effect::Update()
	{
	
		GameObject::Update();
	}	

	void Effect::LateUpdate()
	{
		GameObject::LateUpdate();	
	}

	void Effect::Render()
	{
		GameObject::Render();
	}

}