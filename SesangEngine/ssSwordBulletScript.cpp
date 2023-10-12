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

			// �뽬 �߿� ���� ����ȭ 
			if (!bDash)
			{
				mState = GameState::GetInst().GetState(L"Player");
				mState->SetCurrentHP(mState->GetCurrentHP() - 10);

				// ���� ������ �� HIT ���·� ���� 
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
