#include "ssFSM.h"
#include "ssState.h"
#include "ssMonsterScript.h"

namespace ss
{
	FSM::FSM(MonsterScript* monsterScript)
		: m_MonsterScript(monsterScript)
		, mCurrentState(nullptr)
	{
	}

	FSM::~FSM()
	{
		std::map<std::wstring, State*>::iterator iter = mStates.begin();

		for (; iter != mStates.end(); ++iter)
		{
			if(nullptr != iter->second)
				delete iter->second;
		}

		mStates.clear();
	}

	void FSM::Update()
	{
	}

	void FSM::LateUpdate()
	{
		if (nullptr == mCurrentState)
			return;

		mCurrentState->LateUpdate();
	}


	void FSM::AddState(const std::wstring& _strKey, State* _pState)
	{
		mStates.insert(make_pair(_strKey, _pState));
		_pState->mOwnerAI = this;


	}

	void FSM::ChangeState(const std::wstring& _strStateName)
	{

		// 현재 상태가 있다면 빠져나온다.
		if (nullptr != mCurrentState)
			mCurrentState->Exit();

		// 다음 state로 전환될 녀석을 갖고 온다. 
		State* pNextState = FindState(_strStateName);
		assert(pNextState); // 없는 상태를 찾으려고 하면 에러

		// 다음 state로 전환하기 위해 현재 상태로 지정한다. 
		mCurrentState = pNextState;
		// Enter함수를 호출한다.
		mCurrentState->Enter();
	}
}