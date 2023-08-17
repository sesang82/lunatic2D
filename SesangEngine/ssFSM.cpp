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

		// ���� ���°� �ִٸ� �������´�.
		if (nullptr != mCurrentState)
			mCurrentState->Exit();

		// ���� state�� ��ȯ�� �༮�� ���� �´�. 
		State* pNextState = FindState(_strStateName);
		assert(pNextState); // ���� ���¸� ã������ �ϸ� ����

		// ���� state�� ��ȯ�ϱ� ���� ���� ���·� �����Ѵ�. 
		mCurrentState = pNextState;
		// Enter�Լ��� ȣ���Ѵ�.
		mCurrentState->Enter();
	}
}