#include "ssProgressbar.h"
#include "ssRenderer.h"
#include "ssConstantBuffer.h"
#include "ssResources.h"
#include "ssMeshRenderer.h"
#include "ssCharacterState.h"
#include "ssSkeletonArcherScript.h"
#include "ssStoneEyeScript.h"
#include "ssSkeletonLizardScript.h"

namespace ss
{
	using namespace ss::graphics;

	Progressbar::Progressbar()
		: UI(eUIType::Progressbar)
		, mWidth(18) // 이미지 실제 크기 값 
		, mOffset(0.f)
		, mfHPratio(0.f)
	{
	}
	Progressbar::~Progressbar()
	{
	}
	void Progressbar::Initialize()
	{


		mTransform = GetComponent<Transform>();


		MeshRenderer* mr = AddComponent<MeshRenderer>();


		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));

		if (GetName() != L"Boss_Bar")
		{
			mr->SetMaterial(Resources::Find<Material>(L"MonsterHPBarMtrl"));
			mTransform->SetScale(Vector3(18.f, 3.f, 1.f)); // backsize랑 스케일값 동일하게 주기.
			mOwner = GetParent();
		}

		else
		{
			mr->SetMaterial(Resources::Find<Material>(L"BossHPBarMtrl"));
			mTransform->SetScale(Vector3(260.f, 6.f, 1.f)); // backsize랑 스케일값 동일하게 주기.
			mWidth = 260.f;
		}


		
	
		UI::Initialize();

	}
	void Progressbar::Update()
	{
	

			mState = mOwner->GetComponent<CharacterState>();
			
			if (nullptr != mState)
			{
			mfHPratio = mState->GetCurrentHP() / mState->GetMaxHP();
			}

		// 체력의 비율에 따라 체력바의 너비 조절
		float CurWidth = mWidth * mfHPratio;

		Vector3 pos = mTransform->GetPosition();


		if (mOwner->GetName() == L"B_WolfObj")
		{
			// 체력바 스케일 설정 
			mTransform->SetScale(Vector3(CurWidth, 6, 1.f));

			Vector3 Scale = mTransform->GetScale();

			// 체력바의 왼쪽 끝 위치를 계산 (x 위치에 왼쪽 끝으로 이동)
			Vector2 leftTop = mTransform->GetWorldLeftTop();

			leftTop.x = -130.f;
			pos.x = leftTop.x + (Scale.x * 0.5f);

			// 체력바의 위치 설정
			mTransform->SetPosition(pos);

		}

		else
		{
			// update를 돌 동안 값이 바뀔때만 하게 하려했으나, 오프셋 문제로 지움 
			if (mOwner->GetName() == L"StoneEye")
			{
				// 스톤아이처럼 오프셋을 하나로만 쓰려면, 애니메이션에다가 충돌체를 맞추지말고
				// 충돌체에다가 애니메이션을 맞춰야함. 
				pos.x = -36;

			}

			if (mOwner->GetName() == L"Archer")
			{
				SkeletonArcherScript* ArcherScript = mOwner->GetComponent<SkeletonArcherScript>();

				if (ArcherScript->GetDir() == Vector3(1.0f, 0.f, 0.f))
				{
					pos.x = -5; // offset 크기 조정 
				}
				else if (ArcherScript->GetDir() == Vector3(-1.0f, 0.f, 0.f))
				{
					pos.x = 5; // offset 크기 조정 
				}

			}

			if (mOwner->GetName() == L"Lizard")
			{
				SkeletonLizardScript* LizardScript = mOwner->GetComponent<SkeletonLizardScript>();
				pos.x = -5;

			}


			if (mOwner->GetName() == L"WoodGolemObj")
			{
				pos.x = -13;

			}

			if (mOwner->GetName() == L"WolfObj")
			{
				pos.x = 8;

			}

			// 체력바 오른쪽에서 왼쪽으로 깎이도록 하기 
			pos.x -= (1 - mfHPratio) * mWidth * 0.5;

			// 체력바 스케일 설정 
			mTransform->SetScale(Vector3(CurWidth, 3, 1));

		}

	
			



		UI::Update();

	}


	void Progressbar::LateUpdate()
	{



		UI::LateUpdate();
	}
	void Progressbar::Render()
	{
		UI::Render();
	}
	void Progressbar::BindConstantBuffer()
	{
	
	}
}
