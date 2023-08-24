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
		AddComponent<MeshRenderer>();
		AddComponent<Animator>();
	}

	Effect::~Effect()
	{
	}

	void Effect::Initialize()
	{
		Transform* tr = GetComponent<Transform>();
		tr->SetScale(1.f, 1.f, 1.f);

		MeshRenderer* mr = GetComponent<MeshRenderer>();
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimationMaterial"));

		Animator* ani = GetComponent<Animator>();

		std::shared_ptr<graphics::Texture> Stun = Resources::Find<graphics::Texture>(L"StunEffect");

		// === 
		// 		// �� �浹ü ũ��� �޶����°� �ִٸ� fsm���� ���� �������ֱ� 
		// === 
		// �÷��̾� �ִϸ��̼��� �� Ư���ϹǷ�, ���� ���� ������ �� �ٸ��� �ֵ� ��������	 �����ϰ� �ֱ�. (������ ���� Ʋ���� ��) 
		// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
		//  LT, 1������ �縦 ������, �ڸ� ����, ������(y���� -���� Ŭ���� �ö�), ����ð�, ������

		// === 
		// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
		//  LT, 1������ �縦 ������, �ڸ� ����, �������, ������(y���� -���� Ŭ���� �ö�)
		//ani->Create(L"StunEffectAnim", Stun, Vector2(0.f, 0.f), Vector2(27.f, 15.f), 5, Vector2(27.f, 15.f), Vector2(0.f, -19.f));


		GameObject::Initialize();
	}

	void Effect::Update()
	{
	
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