#include "ssAttackCollider.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssCharacterState.h"
#include "ssAnimator.h"

namespace ss
{
	AttackCollider::AttackCollider()
	{
	}
	AttackCollider::~AttackCollider()
	{
	}
	void AttackCollider::Initialize()
	{
		Transform* tr = GetComponent<Transform>();
	
		
		CharacterState* cs = AddComponent<CharacterState>();
		Animator* anim = AddComponent<Animator>();

		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"DebugMaterial"));


		tr->SetScale(Vector3(1.f, 1.f, 1.f));




		GameObject::Initialize();
	}
	void AttackCollider::Update()
	{


			GameObject::Update();

	}
	void AttackCollider::LateUpdate()
	{
		

			GameObject::LateUpdate();
		
	}
	void AttackCollider::Render()
	{

			GameObject::Render();
		
	}
}