#include "ssProgressbar.h"
#include "ssRenderer.h"
#include "ssConstantBuffer.h"
#include "ssResources.h"
#include "ssMeshRenderer.h"
#include "ssCharacterState.h"
#include "ssSkeletonArcherScript.h"
#include "ssStoneEyeScript.h"
#include "ssSkeletonLizardScript.h"
#include "ssBigWolfScript.h"
#include "ssGameState.h"

namespace ss
{
	using namespace ss::graphics;

	Progressbar::Progressbar()
		: mWidth(0.f) // 이미지 실제 크기 값 
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

		if (GetName() == L"Boss_Bar")
		{
			mr->SetMaterial(Resources::Find<Material>(L"BossHPBarMtrl"));
			mTransform->SetScale(Vector3(260.f, 6.f, 1.f));
			mWidth = 260.f;
		}


		else if (GetName() == L"overloadBar")
		{
			mr->SetMaterial(Resources::Find<Material>(L"PlayerOverloadBarMtrl"));
			mTransform->SetScale(Vector3(48.f, 4.f, 1.f));
			mWidth = 48.f;
		}

		else // 일반 몬스터 hp바 
		{
			mr->SetMaterial(Resources::Find<Material>(L"MonsterHPBarMtrl"));
			mTransform->SetScale(Vector3(18.f, 3.f, 1.f));
			mOwner = GetParent();
			mWidth = 18.f;
		}


		
	
		UI::Initialize();

	}
	void Progressbar::Update()
	{
	

			mState = mOwner->GetComponent<CharacterState>();
			
			if (nullptr != mState) // 몬스터가 dead 상태일 때 업데이트가 더는 돌지 않아서 오류나기 때문에 이처럼 함 
			{
			  mfHPratio = mState->GetCurrentHP() / mState->GetMaxHP();
			}

		// 체력의 비율에 따라 체력바의 너비 조절
		float CurHPWidth = mWidth * mfHPratio;

		Vector3 pos = mTransform->GetPosition();


		if (mOwner->GetName() == L"B_WolfObj")
		{

			// 체력바 스케일 설정 
			mTransform->SetScale(Vector3(CurHPWidth, 6, 1.f));

			Vector3 Scale = mTransform->GetScale();

			// 체력바의 왼쪽 끝 위치를 계산 (x 위치에 왼쪽 끝으로 이동)
			Vector2 leftTop = mTransform->GetWorldLeftTop();

			leftTop.x = -130.f;
			pos.x = leftTop.x + (Scale.x * 0.5f);

			// 체력바의 위치 설정
			mTransform->SetPosition(pos);

		}


		else if (GetName() == L"overloadBar")
		{
			mState = GameState::GetInst().GetState(L"Player");

			float CurOverloadratio = mState->GetCurrentOverload() / mState->GetMaxOverload();

			// 체력의 비율에 따라 체력바의 너비 조절
			float CurOverloadWidth = mWidth * CurOverloadratio;


			// 체력바 스케일 설정 
			mTransform->SetScale(Vector3(CurOverloadWidth, 4, 1.f));

			Vector3 Scale = mTransform->GetScale();

			// 체력바의 왼쪽 끝 위치를 계산 (x 위치에 왼쪽 끝으로 이동)
			Vector2 leftTop = mTransform->GetWorldLeftTop();

			leftTop.x = -24.f;
			pos.x = leftTop.x + (Scale.x * 0.5f);

			// 체력바의 위치 설정
			mTransform->SetPosition(pos);


		}


		else
		{


			// 체력바 스케일 설정 
			mTransform->SetScale(Vector3(CurHPWidth, 3, 1));

			Vector3 Scale = mTransform->GetScale();

			// 체력바의 왼쪽 끝 위치를 계산 (x 위치에 왼쪽 끝으로 이동)
			Vector2 leftTop = mTransform->GetWorldLeftTop();

			
			// update를 돌 동안 값이 바뀔때만 하게 하려했으나, 오프셋 문제로 지움 
			if (mOwner->GetName() == L"StoneEye")
			{
				// 스톤아이처럼 오프셋을 하나로만 쓰려면, 애니메이션에다가 충돌체를 맞추지말고
				// 충돌체에다가 애니메이션을 맞춰야함. 
				leftTop.x = -45.f;

			}

			else if (mOwner->GetName() == L"Archer")
			{
				leftTop.x = -15.f;

			}

			else if (mOwner->GetName() == L"Lizard")
			{
				leftTop.x = -25.f;

			}


			else if (mOwner->GetName() == L"WoodGolemObj")
			{
				leftTop.x = -20.f;

			}

			else if (mOwner->GetName() == L"WolfObj")
			{
				leftTop.x = -3.f;

			}

			pos.x = leftTop.x + (Scale.x * 0.5f);

			// 체력바의 위치 설정
			mTransform->SetPosition(pos);

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
