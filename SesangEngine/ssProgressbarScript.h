#pragma once
#include <ssScript.h>

namespace ss
{
	class ProgressbarScript
		: public Script
	{
		// 일반적으로 이런 enum문은 public으로 제공된다구 함
	public:
		enum class eState
		{
			None,
			Damaged,
			Heal,
			End,
			// 다른 상태들 추가 가능
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

