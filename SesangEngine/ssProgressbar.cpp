#include "ssProgressbar.h"
#include "ssRenderer.h"
#include "ssConstantBuffer.h"
#include "ssResources.h"
#include "ssMeshRenderer.h"
#include "ssCharacterState.h"

namespace ss
{
	using namespace ss::graphics;

	Progressbar::Progressbar()
		: UI(eUIType::Progressbar)
		, mWidth(18) // 이미지 실제 크기 값 
		, mOffset(0.f)
	{
	}
	Progressbar::~Progressbar()
	{
	}
	void Progressbar::Initialize()
	{


		mTransform = GetComponent<Transform>();
		mTransform->SetScale(Vector3(18.f, 3.f, 1.f)); // backsize랑 스케일값 동일하게 주기. 


		MeshRenderer* mr = AddComponent<MeshRenderer>();
		mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"MonsterHPBarMtrl")); 


		mOwner = GetParent();
		mState = mOwner->GetComponent<CharacterState>();

		UI::Initialize();

	}
	void Progressbar::Update()
	{
	
		float HPratio = mState->GetCurrentHP() / mState->GetMaxHP();

		float test2 = mState->GetCurrentHP();

		// 체력의 비율에 따라 체력바의 너비 조절
		float CurWidth = mWidth * HPratio;


		//// 바뀔때만 연산을 하도록 설정 (Update이므로 성능에 영향 갈 수 있으므로)
		if (mState->Getchanged())
		{
			float test = mState->GetCurrentHP();


			Vector3 pos = mTransform->GetPosition();
			pos.x = mOffset; // offset 크기 조정 

			// 체력바 오른쪽에서 왼쪽으로 깎이도록 하기 
			pos.x -= (1 - HPratio) * mWidth * 0.5;

			// 체력바의 위치 설정
			mTransform->SetPosition(Vector3(pos));

			// 체력바 스케일 설정 
			mTransform->SetScale(Vector3(CurWidth, 3, 1));



			mState->SetChanged(false);
		

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
