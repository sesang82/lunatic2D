#include "ssPlayerGuardColScript.h"
#include "ssGameObject.h"

namespace ss
{
	PlayerGuardColScript::PlayerGuardColScript()
	{
	}
	PlayerGuardColScript::~PlayerGuardColScript()
	{
	}
	void PlayerGuardColScript::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();
		mCollider = GetOwner()->GetComponent<Collider2D>();
	}
	void PlayerGuardColScript::Update()
	{
		Vector3 ColPos = mTransform->GetPosition();
		mTransform->SetPosition(ColPos);

	}
	void PlayerGuardColScript::OnCollisionEnter(Collider2D* other)
	{
		if (other->GetOwner()->GetLayerType() == eLayerType::Mon_Bullet)
		{
			if (other->GetOwner()->GetName() == L"StoneEyeProjectileCol")
			{
				// 총알을 회전 시킨다. 
				Transform* tr = other->GetOwner()->GetComponent<Transform>();
				tr->SetRotation(90.f, 0.f, 1.f);

			}
		}
	}
	void PlayerGuardColScript::OnCollisionStay(Collider2D* other)
	{
	}
	void PlayerGuardColScript::OnCollisionExit(Collider2D* other)
	{
	}
}