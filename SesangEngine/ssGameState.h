#pragma once
#include "ssCharacterState.h"

namespace ss
{
	

	class GameState
	{
		friend class CharacterState;

	private:
		GameState();
		~GameState();

	private:
		static GameState* m_Inst;

	public:
		static GameState& GetInst()
		{
			if (!m_Inst)
				m_Inst = new GameState;

			return *m_Inst;
		}

		static void DestroyInst()
		{
			if (m_Inst)
			{
				delete m_Inst;
				m_Inst = nullptr;
			}
		}


	private:
		std::map<std::wstring, CharacterState*> mCharacterStates;

	public:

	
		// appllication에서 추가해주면 됨 
		void AddState(const std::wstring& Name, int hp, int sp)
		{
			CharacterState* newState = new CharacterState();
			newState->SetCurrentHP(hp);
			newState->SetMaxHP(hp);
			newState->SetCurrentSP(sp);
			newState->SetMaxSP(sp);
			mCharacterStates[Name] = newState;
		}

		// 보스 2처럼 2페이즈 있는 보스들은 아래 활용하면 될듯 

		// 저장해둔 일부 녀석만 삭제하고 싶을 때
		void RemoveState(const std::wstring& Name)
		{
			CharacterState* stateToRemove = mCharacterStates[Name];
			delete stateToRemove;
			mCharacterStates.erase(Name);
		}


		CharacterState* GetState(const std::wstring& Name)
		{
			return mCharacterStates[Name];
		}





	};
}

