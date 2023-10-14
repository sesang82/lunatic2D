#include "ssUIFrameScript.h"
#include "ssPlayer.h"
#include "ssPlayerScript.h"
#include "ssobject.h"
#include "ssGameObject.h"
#include "ssAnimator.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"

namespace ss
{


	UIFrameScript::UIFrameScript()
		:mbSword(false)
		,mbGauntlet(false)
		,mbPistol(false)
	{
	}

	UIFrameScript::~UIFrameScript()
	{
	}

	void UIFrameScript::Initialize()
	{
		/*if (nullptr != mPlayer)
		{
			PlayerScript* playerScript = mPlayer->GetComponent<PlayerScript>();

			bool sword = playerScript->GetSword();
			bool gauntlet = playerScript->GetGauntlet();
			bool pistol = playerScript->GetPistol();

		}*/

		mMr = GetOwner()->GetComponent<MeshRenderer>();



	
	}

	void UIFrameScript::Update()
	{
		if (nullptr != mPlayer)
		{
	
			if (GetOwner()->GetName() == L"Weapon_Slot1")
			{

				if (SceneManager::IsOnSword())
				{
					// 계속 update 도는 횟수 줄이기 위해서 지역변수니 일일이 다 써줌 
					mMr->SetMaterial(Resources::Find<Material>(L"w1Mtrl"));

				}

			}

			else if (GetOwner()->GetName() == L"Weapon_Slot2")
			{
				

				 if (SceneManager::IsOnPistol())
				{
					mMr->SetMaterial(Resources::Find<Material>(L"w2Mtrl"));

				}



			}

			else if (GetOwner()->GetName() == L"Weapon_Slot3")
			{
				
				if (SceneManager::IsOnGauntlet())
				{
					mMr->SetMaterial(Resources::Find<Material>(L"w3Mtrl"));
				}

			}


		}

	}

	void UIFrameScript::OnCollisionEnter(Collider2D* other)
	{
	}

	void UIFrameScript::OnCollisionStay(Collider2D* other)
	{
	}

	void UIFrameScript::OnCollisionExit(Collider2D* other)
	{
	}

}