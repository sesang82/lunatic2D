#include "ssWallScript.h"
#include "ssGameObject.h"
#include "ssRigidbody2D.h"
#include "ssPlayer.h"
#include "ssInput.h"
#include "ssPlayerScript.h"


namespace ss
{
	WallScript::WallScript()
	{
	}
	WallScript::~WallScript()
	{
	}
	void WallScript::Initialize()
	{


	}
	void WallScript::Update()
	{
	}
	void WallScript::OnCollisionEnter(Collider2D* other)
	{

		
	}
	void WallScript::OnCollisionStay(Collider2D* other)
	{
		if (other->GetName() == L"colHit_player" && GetOwner()->GetName() == L"Wall_R_Obj")
		{
			Rigidbody2D* playerRigd = mPlayer->GetComponent<Rigidbody2D>();
			
			bool TurnOverload = mPlayer->GetComponent<PlayerScript>()->IsTurnOverload();
			bool IsDash = mPlayer->GetComponent<PlayerScript>()->IsDash();
			bool IsJump = mPlayer->GetComponent<PlayerScript>()->IsJump();


			if (Input::GetKey(eKeyCode::RIGHT) || Input::GetKeyUp(eKeyCode::RIGHT) || IsDash || IsJump)
			{
				playerRigd->SetVelocity(Vector2(0.f, playerRigd->GetVelocity().y));

				if (IsJump)
				{
					playerRigd->SetGravity(Vector2(0.f, 1500.f));
				}

			}

			else if (Input::GetKey(eKeyCode::LEFT))
			{
				if (!TurnOverload)
				{
					playerRigd->SetVelocity(Vector2(-300.f, 0.f));
				}

				else
				{
					playerRigd->SetVelocity(Vector2(-400.f, 0.f));
				}
			}

		}

		else if (other->GetName() == L"colHit_player" && GetOwner()->GetName() == L"Wall_L_Obj")
		{
			Rigidbody2D* playerRigd = mPlayer->GetComponent<Rigidbody2D>();
			bool TurnOverload = mPlayer->GetComponent<PlayerScript>()->IsTurnOverload();
			bool IsDash = mPlayer->GetComponent<PlayerScript>()->IsDash();
			bool IsJump = mPlayer->GetComponent<PlayerScript>()->IsJump();

			bool IsGround = playerRigd->IsGround();


			if (Input::GetKey(eKeyCode::LEFT) || Input::GetKeyUp(eKeyCode::LEFT) || IsDash || IsJump)
			{
				playerRigd->SetVelocity(Vector2(0.f, playerRigd->GetVelocity().y));

				if (IsJump)
				{
					playerRigd->SetGravity(Vector2(0.f, 1500.f));

					if (IsGround)
					{
						mPlayer->GetComponent<PlayerScript>()->SetJump(false);
					}
	
				}


			}

			else if (Input::GetKey(eKeyCode::RIGHT))
			{
				
				if (!IsJump)
				{
					playerRigd->SetGravity(Vector2::Zero);
				}

				if (!TurnOverload || !IsJump)
				{
					playerRigd->SetVelocity(Vector2(300.f, 0.f));
				}

				else if (TurnOverload || !IsJump)
				{
					playerRigd->SetVelocity(Vector2(400.f, 0.f));
				}
			}


			

		}




		else if (other->GetName() == L"colHit_player" && GetOwner()->GetName() == L"Wall_Up_Obj")
		{
			Rigidbody2D* playerRigd = mPlayer->GetComponent<Rigidbody2D>();
			playerRigd->SetGravity(Vector2(0.f, 1500.f));

		}
	}
	void WallScript::OnCollisionExit(Collider2D* other)
	{
		if (other->GetName() == L"colHit_player" && GetOwner()->GetName() == L"Wall_R_Obj")
		{
			Rigidbody2D* playerRigd = mPlayer->GetComponent<Rigidbody2D>();
			playerRigd->SetVelocity(Vector2(300.f, 0.f));
		}

		else if (other->GetName() == L"colHit_player" && GetOwner()->GetName() == L"Wall_L_Obj")
		{
			Rigidbody2D* playerRigd = mPlayer->GetComponent<Rigidbody2D>();
			playerRigd->SetVelocity(Vector2::Zero);


		}

		else if (other->GetName() == L"colHit_player" && GetOwner()->GetName() == L"Wall_Up_Obj")
		{
			Rigidbody2D* playerRigd = mPlayer->GetComponent<Rigidbody2D>();
			playerRigd->SetGravity(Vector2(0.f, 1500.f));

		}
	}
}