#pragma once
#include "ssScript.h"

namespace ss
{
	enum class eMonsterState
	{
		MOVE,
		TRACER,
		JUMP,
		FALL,
		LADING,
		STUN,
		HIT,
		GUARD,
		NEARATTACK,
		FARATTACK,
		DEAD,
		END
	};

	// hp나 그런거는 state 스크립트 갖다붙이기
	struct tMonsterInfo
	{
		float	m_fSpeed; // 움직이는 속도 

		float	m_fAttack; // 공격력
		float	m_fDefense; // 방어력

		float	m_fDetectRange; // 감지 범위 
		float	m_fAttackRange; // 공격 범위 
	};

	class MonsterScript :
		public Script
	{
    public:
        MonsterScript();
        virtual ~MonsterScript();

    public:
        virtual void Initialize() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(Collider2D* other) override;
        virtual void OnCollisionStay(Collider2D* other) override;
        virtual void OnCollisionExit(Collider2D* other) override;



	protected:
		tMonsterInfo m_tMonsterInfo;
		class FSM*			 mFSM;

		class Animator*			mAnimator;
		class Rigidbody2D*		mRigidbody;
		class Transform*		mTransform;
		class Collider2D*		mCollider;
		class MeshRenderer*		mMeshRenderer;
		class CharacterState* mCharacterState;
		
		class Player*			mPlayer;
		//class Monster*			mMonster;

		Vector3					mPrevDir;
		Vector3					mCurDir;

		eMonsterState			 mCurState;
		eMonsterState			 mPrevState;

		Vector2					mOringinColSize;
		Vector2					mOringinColCeter;

	private:
		GameObject* m_pTarget;

	public:
		tMonsterInfo GetMonsterInfo() const
		{
			return m_tMonsterInfo;
		}

		void SetMonsterInfo(const tMonsterInfo& _tMonsterInfo)
		{
			m_tMonsterInfo = _tMonsterInfo;
		}

		void SetTarget(GameObject* _pPlayer)
		{
			m_pTarget = _pPlayer;
		}

		void ChangeState(eMonsterState state) 
		{ 
			if (mCurState == state)
				return;

			mCurState = state; 
		}


		Vector2 GetOriginColSize() const
		{
			return mOringinColSize;
		}

		Vector2 GetOrigniColCenter() const
		{
			return mOringinColCeter;
		}



	};
}


