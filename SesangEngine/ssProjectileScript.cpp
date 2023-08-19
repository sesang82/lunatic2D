#include "ssProjectileScript.h"
#include "ssState.h"
#include "ssGameState.h"

namespace ss
{
	ProjectileScript::ProjectileScript()
	{
	}
	ProjectileScript::~ProjectileScript()
	{
	}
	void ProjectileScript::Initialize()
	{
	}
	void ProjectileScript::Update()
	{
	}
	void ProjectileScript::OnCollisionEnter(Collider2D* other)
	{
		if (other->GetName() == L"Player")
		{
			mState = GameState::GetInst().GetState(L"Player");
			mState->SetCurrentHP(mState->GetCurrentHP() - 10);
		}
	}
	void ProjectileScript::OnCollisionStay(Collider2D* other)
	{
	}
	void ProjectileScript::OnCollisionExit(Collider2D* other)
	{
	}
}