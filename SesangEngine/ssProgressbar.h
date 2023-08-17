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
			// 다른 상태들 추가 가능
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

		// (유지관리 용이를 위해 감소 / 증가 함수도 따로 빼둠)
		// 감소하는 함수
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

