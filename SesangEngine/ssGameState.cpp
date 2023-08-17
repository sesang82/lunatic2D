#include "ssGameState.h"

namespace ss
{

	GameState* GameState::m_Inst = nullptr;

	GameState::GameState()
	{
	}
	GameState::~GameState()
	{
		// �޸� �� �ذ� �Ϸ� 
		std::map<std::wstring, CharacterState*>::iterator iter =
			mCharacterStates.begin();

		for (; iter != mCharacterStates.end(); ++iter)
		{

			if(nullptr != iter->second)
			delete iter->second;
		}

		mCharacterStates.clear();
	}





}