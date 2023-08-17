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
		// ���°��� �ο��ؼ� �˾Ƽ� ����� �� �ְ� �ϱ�
		// == �� if(state::Normal)�̶�� ���ǹ��� �ڵ� �ۼ��� ��ȿ���� ���� ���
		/*if (State::Normal)�� ���� ���¸� ���� ���ϴ� ���� State::Normal��
		�׻� true�� �򰡵Ǳ� ������ �ǹ̰� �����ϴ�. ������ if���� ������ ������ ����������
		�Ǻ��ϱ� ���� �Ҹ���(bool) ���� �ʿ��մϴ�. ���� mState�� ���¸� ���Ͽ� ������ ó���ؾ� �մϴ�.*/

		// ���¿� ���� �ٸ� ��� ó��
		if (mBarState == eBarState::None)
		{
			// Normal ���¿� ���� ��� ó��
		}
		else if (mBarState == eBarState::Damaged)
		{
			// Damaged ���¿� ���� ��� ó��
		}

		else
		{
			// Heal ���¿� ���� ��� ó�� 
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
		//// ��� ���ۿ� ����, ��, ���� ����� ��� �����ش�. 
		//renderer::AmountCB atCB = {};
		//atCB.a_CurrentValue = mCurrentValue;
		//atCB.a_MaxValue = mMaxValue;

		//// ��� ���� �߿� Amount��� ���۸� ���� �´�. 
		//ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Amount];
		//cb->SetData(&atCB); // ��� ���ۿ� �־��ش�.

		//cb->Bind(eShaderStage::PS); // ��� ���۴� ��� ���̴� �ܰ��̵� ���ε��� �� �ִٴ°� �����̴�. 
	}
}
