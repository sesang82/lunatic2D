#pragma once
#include "ssEntity.h"
#include "ssFSM.h"

namespace ss
{


	class State :
		public Entity
	{
		friend class FSM;

	public:
		State();
		~State();


	public:
		virtual void LateUpdate() = 0;
		virtual void Enter() = 0;
		virtual void Exit() = 0;

	private:
		FSM* mOwnerAI; // FSM컴포넌트가 state를 소유하고 있게함

	public:
		// State를 소유하고 있는 FSM 컴포넌트를 갖고 온다.
		FSM* GetOwnerAI() { return mOwnerAI; }

		// FSM 컴포넌트를 소유하고 있는 게임오브젝트를 갖고 온다.
		GameObject* GetOwnerObj() { return mOwnerAI->GetOwner(); }

		void ChangeState(const wchar_t* _strStateName);
	};

}

