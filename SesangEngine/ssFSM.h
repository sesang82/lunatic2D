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

			// 끝에 도달하지 않았다면 있다는 뜻이므로
			if (iter != mStates.end())
				// State*를 반환
				return iter->second;

			return nullptr;
		}

		void ChangeState(const std::wstring& _strStateName);



	};

}

