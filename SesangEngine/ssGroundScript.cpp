#include "ssGroundScript.h"
#include "ssGameObject.h"
#include "ssCollider2D.h"
#include "ssPlayer.h"
#include "ssRigidbody2D.h"

namespace ss
{
	GroundScript::GroundScript()
		: mTransform(nullptr)
	{
	}
	GroundScript::~GroundScript()
	{
	}
	void GroundScript::Initialize()
	{

		mTransform = GetOwner()->GetComponent<Transform>();
		mTransform->SetPosition(Vector3(0.f, -500.f, 450.f));
		mTransform->SetScale(Vector3(835.f, 100.f, 1.f));



		Script::Initialize();
	}
	void GroundScript::Update()
	{
		Script::Update();
	}
	void GroundScript::OnCollisionEnter(Collider2D* other)
	{
		Player* player = (Player*)other->GetOwner();

		if (nullptr == player)
			return;

		Rigidbody2D* rg = player->GetComponent<Rigidbody2D>();
		rg->SetGround(true);

		rg->SetVelocity(Vector2(0.0f, 0.0f));


		


	}
	void GroundScript::OnCollisionStay(Collider2D* other)
	{
	}
	void GroundScript::OnCollisionExit(Collider2D* other)
	{
	}
}