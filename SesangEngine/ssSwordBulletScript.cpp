#include "ssSwordBulletScript.h"
#include "ssGameObject.h"
#include "ssPlayerScript.h"
#include "ssGameState.h"
#include "ssGoddnessScript.h"


namespace ss
{
	SwordBulletScript::SwordBulletScript()
	{
	}
	SwordBulletScript::~SwordBulletScript()
	{
	}
	void SwordBulletScript::Initialize()
	{
	}
	void SwordBulletScript::Update()
	{
	}
	void SwordBulletScript::OnCollisionEnter(Collider2D* other)
	{

		if (other->GetName() == L"col_Floor" && GetOwner()->GetName() == L"Sword_UpToDown")
		{


			GetOwner()->SetState(GameObject::eState::Dead);


		}

		else if (other->GetName() == L"colHit_player")
		{
			PlayerScript* script = other->GetOwner()->GetComponent<PlayerScript>();
			bool bDash = script->IsDash();

			// 대쉬 중엔 공격 무력화 
			if (!bDash)
			{
				mState = GameState::GetInst().GetState(L"Player");
				mState->SetCurrentHP(mState->GetCurrentHP() - 10);

				// 공격 당했을 시 HIT 상태로 변경 
				script->ChangeState(ePlayerState::HIT);

			}
		}

	
	}
	void SwordBulletScript::OnCollisionStay(Collider2D* other)
	{

		
	}
	void SwordBulletScript::OnCollisionExit(Collider2D* other)
	{
	}
}
