#pragma once
#include <ssScript.h>

namespace ss
{
	class ProgressbarScript
		: public Script
	{
		// �Ϲ������� �̷� enum���� public���� �����ȴٱ� ��
	public:
		enum class eState
		{
			None,
			Damaged,
			Heal,
			End,
			// �ٸ� ���µ� �߰� ����
		};

	public:
		ProgressbarScript();
		~ProgressbarScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	public:
		void BindConstantBuffer();

		//void SetCamera(Camera* camera) { mCamera = camera; }

	private:
		float mCurrentValue;
		float mMaxValue;


		eState mBarState;


		UINT mBarStates[(UINT)eState::End];

	public:
		int GetCurrentValue() { return mCurrentValue; }
		int GetMaxValue() { return mMaxValue; }

		void SetSate(eState state) { mBarState = state; }
		eState GetState() { return mBarState; }

		// (�������� ���̸� ���� ���� / ���� �Լ��� ���� ����)
		// �����ϴ� �Լ�
		int SetDecrease(int _Damage)
		{
			if (0 < mCurrentValue)
				mCurrentValue -= _Damage;

			return mCurrentValue;
		}

		int SetIncrease(int _Increase)
		{
			if (mCurrentValue > mMaxValue)
				mCurrentValue += _Increase;

			return mCurrentValue;
		}




	};
}

