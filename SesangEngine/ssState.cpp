#include "ssState.h"

namespace ss
{
	State::State()
		: mOwnerAI(nullptr)
	{
	}
	State::~State()
	{
	}
	void State::ChangeState(const wchar_t* _strStateName)
	{
		mOwnerAI->ChangeState(_strStateName);
	}
}
