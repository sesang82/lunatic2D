#include "ssWallScript.h"
#include "ssGameObject.h"
#include "ssRigidbody2D.h"
#include "ssPlayer.h"
#include "ssInput.h"
#include "ssPlayerScript.h"
#include "ssAnimator.h"
#include "ssBigEnergyball.h"


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

		if (GetOwner()->GetName() == L"Wall_R_Obj")
		{
			if (other->GetOwner()->GetName() == L"S_EnergyballObjs"
				|| other->GetOwner()->GetName() == L"Parrying_S_EnergyballObj")
			{

				Transform* tr = other->GetOwner()->GetComponent<Transform>();

				Vector3 rotation = tr->GetDegreeRot();
				rotation.z = 90.0f;
				tr->SetRotation(rotation);

				other->GetOwner()->GetComponent<Animator>()->PlayAnimation(L"Energyball_S_Parrying_End", false);

			}


			//else if (other->GetOwner()->GetName() == L"Sword_LeftToLight")
			//{
			//	other->GetOwner()->SetState(GameObject::eState::Dead);
			//}



		}

		else if (GetOwner()->GetName() == L"Wall_L_Obj")
		{
			if (other->GetOwner()->GetName() == L"S_EnergyballObjs"
				|| other->GetOwner()->GetName() == L"Parrying_S_EnergyballObj")
			{

				Transform* tr = other->GetOwner()->GetComponent<Transform>();

				Vector3 rotation = tr->GetDegreeRot();
				rotation.z = -90.0f;
				tr->SetRotation(rotation);


				other->GetOwner()->GetComponent<Animator>()->PlayAnimation(L"Energyball_S_Parrying_End", false);

			}

			//else if (other->GetOwner()->GetName() == L"Sword_RightToLeft")
			//{
			//	other->GetOwner()->SetState(GameObject::eState::Dead);
			//}


		}

		else if (GetOwner()->GetName() == L"Wall_Up_Obj")
		{
			if (other->GetName() == L"colHit_player")
			{
				Rigidbody2D* playerRigd = mPlayer->GetComponent<Rigidbody2D>();
				playerRigd->SetVelocity(Vector2(playerRigd->GetVelocity().x, 0.f));

			}

			else if (other->GetOwner()->GetName() == L"S_EnergyballObjs"
				|| other->GetOwner()->GetName() == L"Parrying_S_EnergyballObj")
			{

				Transform* tr = other->GetOwner()->GetComponent<Transform>();

				Vector3 rotation = tr->GetDegreeRot();
				rotation.z = 180.0f;
				tr->SetRotation(rotation);


				other->GetOwner()->GetComponent<Animator>()->PlayAnimation(L"Energyball_S_Parrying_End", false);

			}

			else if (other->GetOwner()->GetName() == L"BigEnergyballs")
			{
				Transform* tr = other->GetOwner()->GetComponent<Transform>();

				Vector3 rotation = tr->GetDegreeRot();
				rotation.z = 180.0f;
				tr->SetRotation(rotation);


				BigEnergyball* big = (BigEnergyball*)other->GetOwner();

				big->SetHit(true);


			}


		/*	else if (other->GetOwner()->GetName() == L"Sword_DownToUp")
			{
				other->GetOwner()->SetState(GameObject::eState::Dead);
			}*/


		}


		


		





		
	}
	void WallScript::OnCollisionStay(Collider2D* other)
	{

		if (GetOwner()->GetName() == L"Wall_R_Obj")
		{
			if (other->GetName() == L"colHit_player")
			{
				Rigidbody2D* playerRigd = mPlayer->GetComponent<Rigidbody2D>();

				bool TurnOverload = mPlayer->GetComponent<PlayerScript>()->IsTurnOverload();
				bool IsDash = mPlayer->GetComponent<PlayerScript>()->IsDash();
				bool IsJump = mPlayer->GetComponent<PlayerScript>()->IsJump();

				bool IsGround = playerRigd->IsGround();


				if (Input::GetKey(eKeyCode::RIGHT) || Input::GetKeyUp(eKeyCode::RIGHT) || IsDash || IsJump)
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

				else if (Input::GetKey(eKeyCode::LEFT))
				{

					if (!IsJump)
					{
						playerRigd->SetGravity(Vector2::Zero);
					}


					if (!TurnOverload || !IsJump)
					{
						playerRigd->SetVelocity(Vector2(-300.f, 0.f));
					}

					else if (TurnOverload || !IsJump)
					{
						playerRigd->SetVelocity(Vector2(-400.f, 0.f));
					}
				}

			}






		}


		if (GetOwner()->GetName() == L"Wall_L_Obj")
		{
			if (other->GetName() == L"colHit_player")
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