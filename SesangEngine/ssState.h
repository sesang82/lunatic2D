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
		FSM* mOwnerAI; // FSM������Ʈ�� state�� �����ϰ� �ְ���

	public:
		// State�� �����ϰ� �ִ� FSM ������Ʈ�� ���� �´�.
		FSM* GetOwnerAI() { return mOwnerAI; }

		// FSM ������Ʈ�� �����ϰ� �ִ� ���ӿ�����Ʈ�� ���� �´�.
		GameObject* GetOwnerObj() { return mOwnerAI->GetOwner(); }

		void ChangeState(const wchar_t* _strStateName);
	};

}

