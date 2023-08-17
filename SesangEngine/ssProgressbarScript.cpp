#include "ssProgressbarScript.h"
#include "ssTransform.h"
#include "ssApplication.h"
#include "ssGameObject.h"
#include "ssRenderer.h"
#include "ssMaterial.h"
#include "ssResources.h"
#include "ssMeshRenderer.h"
#include "ssConstantBuffer.h"

namespace ss
{
	ProgressbarScript::ProgressbarScript()
		: mMaxValue(100)
	{
	}
	ProgressbarScript::~ProgressbarScript()
	{
	}
	void ProgressbarScript::Initialize()
	{
	}

	void ProgressbarScript::Update()
	{

		// ���°��� �ο��ؼ� �˾Ƽ� ����� �� �ְ� �ϱ�
		// == �� if(state::Normal)�̶�� ���ǹ��� �ڵ� �ۼ��� ��ȿ���� ���� ���
		/*if (State::Normal)�� ���� ���¸� ���� ���ϴ� ���� State::Normal��
		�׻� true�� �򰡵Ǳ� ������ �ǹ̰� �����ϴ�. ������ if���� ������ ������ ����������
		�Ǻ��ϱ� ���� �Ҹ���(bool) ���� �ʿ��մϴ�. ���� mState�� ���¸� ���Ͽ� ������ ó���ؾ� �մϴ�.*/

		// ���¿� ���� �ٸ� ��� ó��
		if (mBarState == eState::None)
		{
			// Normal ���¿� ���� ��� ó��
		}
		else if (mBarState == eState::Damaged)
		{
			// Damaged ���¿� ���� ��� ó��
		}

		else
		{
			// Heal ���¿� ���� ��� ó�� 
		}


	}
	void ProgressbarScript::LateUpdate()
	{
	}
	void ProgressbarScript::Render()
	{
	}
	void ProgressbarScript::BindConstantBuffer()
	{
		//// ��� ���ۿ� ����, ��, ���� ����� ��� �����ش�. 
		//renderer::AmountCB atCB = {};
		//atCB.mCurrentValue = mCurrentValue;
		//atCB.mMaxValue = mMaxValue;
		////atCB.mBarStates = mBarStates;

		//// ��� ���� �߿� Amount��� ���۸� ���� �´�. 
		//ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Progressbar];
		//cb->SetData(&atCB); // ��� ���ۿ� �־��ش�.

		//cb->Bind(eShaderStage::PS); // ��� ���۴� ��� ���̴� �ܰ��̵� ���ε��� �� �ִٴ°� �����̴�. 

	}
}
