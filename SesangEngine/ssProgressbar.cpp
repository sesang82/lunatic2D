#include "ssProgressbar.h"
#include "ssRenderer.h"
#include "ssConstantBuffer.h"

namespace ss
{
	using namespace ss::graphics;

	Progressbar::Progressbar()
		: UI(eUIType::Progressbar)
		, mMaxValue(100)
	{
	}
	Progressbar::~Progressbar()
	{
	}
	void Progressbar::Initialize()
	{
	}
	void Progressbar::Update()
	{
		// 상태값을 부여해서 알아서 제어될 수 있게 하기
		// == ↓ if(state::Normal)이라는 조건문이 코드 작성시 유효하지 않은 경우
		/*if (State::Normal)과 같이 상태를 직접 비교하는 것은 State::Normal이
		항상 true로 평가되기 때문에 의미가 없습니다. 실제로 if문은 조건이 참인지 거짓인지를
		판별하기 위해 불리언(bool) 값이 필요합니다. 따라서 mState와 상태를 비교하여 동작을 처리해야 합니다.*/

		// 상태에 따른 다른 모션 처리
		if (mBarState == eBarState::None)
		{
			// Normal 상태에 대한 모션 처리
		}
		else if (mBarState == eBarState::Damaged)
		{
			// Damaged 상태에 대한 모션 처리
		}

		else
		{
			// Heal 상태에 대한 모션 처리 
		}


	}


	void Progressbar::LateUpdate()
	{
	}
	void Progressbar::Render()
	{
	}
	void Progressbar::BindConstantBuffer()
	{
		//// 상수 버퍼에 월드, 뷰, 투영 행렬을 담아 보내준다. 
		//renderer::AmountCB atCB = {};
		//atCB.a_CurrentValue = mCurrentValue;
		//atCB.a_MaxValue = mMaxValue;

		//// 상수 버퍼 중에 Amount상수 버퍼를 갖고 온다. 
		//ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Amount];
		//cb->SetData(&atCB); // 상수 버퍼에 넣어준다.

		//cb->Bind(eShaderStage::PS); // 상수 버퍼는 어느 쉐이더 단계이든 바인딩할 수 있다는게 장점이다. 
	}
}
