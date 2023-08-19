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

	
		// appllication���� �߰����ָ� �� 
		void AddState(const std::wstring& Name, int hp, int sp)
		{
			CharacterState* newState = new CharacterState();
			newState->SetCurrentHP(hp);
			newState->SetMaxHP(hp);
			newState->SetCurrentSP(sp);
			newState->SetMaxSP(sp);
			mCharacterStates[Name] = newState;
		}

		// ���� 2ó�� 2������ �ִ� �������� �Ʒ� Ȱ���ϸ� �ɵ� 

		// �����ص� �Ϻ� �༮�� �����ϰ� ���� ��
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

