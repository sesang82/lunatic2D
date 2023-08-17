#pragma once
#include <ssScript.h>

namespace ss
{

	class State;
	class MonsterScript;

	class FSM :
		public Script
	{

	public:
		FSM(MonsterScript* monsterScript);
		~FSM();

	public:
		virtual void Update() override;
		virtual void LateUpdate() override;

	private:
		State*						   mCurrentState;
		std::map<std::wstring, State*> mStates;

		class MonsterScript* m_MonsterScript;

	public:
		void AddState(const std::wstring& _strKey, State* _pState);

		State* FindState(const std::wstring& _strKey)
		{
			std::map<std::wstring, State*>::iterator iter =
				mStates.find(_strKey);

			// ���� �������� �ʾҴٸ� �ִٴ� ���̹Ƿ�
			if (iter != mStates.end())
				// State*�� ��ȯ
				return iter->second;

			return nullptr;
		}

		void ChangeState(const std::wstring& _strStateName);



	};

}

