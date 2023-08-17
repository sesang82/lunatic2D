#pragma once
#include "ssUI.h"

namespace ss
{
	class Progressbar :
		public UI
	{
		enum class eBarState
		{
			None,
			Damaged,
			Heal,
			// �ٸ� ���µ� �߰� ����
		};

	public:
		Progressbar();
		virtual ~Progressbar();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void BindConstantBuffer();

	private:
		int mCurrentValue;
		int mMaxValue;

		eBarState mBarState;

	public:
		int SetCurrentValue() { return mCurrentValue; }
		int SetMaxValue() { return mMaxValue; }

	public:
		eBarState GetBarState() { return mBarState; }

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

