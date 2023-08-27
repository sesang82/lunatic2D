#pragma once
#include "ssScript.h"

namespace ss
{

	enum class eMonsterState
	{
		IDLE,
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
		float	m_fSpeed = 0;// 움직이는 속도 

		float	m_fAttack = 0; // 공격력
		float	m_fDefense = 0; // 방어력

		float	m_fDetectRange = 0; // 감지 범위(추적 범위) 
		float	m_fNearAttackRange = 0; // 근접 공격 범위
		float	m_fFarAttackRange = 0; // 원거리 공격 범위

		float   m_fCoolDown = 0; // 공격 쿨타임
	};



	class MonsterScript :
		public Script
	{
		friend class StoneNearRangeScript;
		friend class ArcherNearRangeScript;

	
	
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
		class CharacterState*	mCharacterState;
		
		class Player*			mPlayer;
		//class Monster*			mMonster;

		Vector3					mPrevDir;
		Vector3					mCurDir;

		eMonsterState			 mCurState;
		eMonsterState			 mPrevState;

		Vector2					mOringinColSize;
		Vector2					mOringinColCeter;

		float					m_fTime; // 시간 측정용

	private:
		GameObject* m_pTarget;

	public:
		virtual tMonsterInfo GetMonsterInfo() const
		{
			return m_tMonsterInfo;
		}

		virtual void SetMonsterInfo(const tMonsterInfo& _tMonsterInfo)
		{
			m_tMonsterInfo = _tMonsterInfo;
		}


		virtual void SetTarget(GameObject* _pPlayer)
		{
			m_pTarget = _pPlayer;
		}

		virtual void ChangeState(eMonsterState state) 
		{ 
			if (mCurState == state)
				return;

			mCurState = state; 
		}

		virtual eMonsterState GetPrevState() { return mPrevState; }





	};
}


