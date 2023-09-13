#pragma once
#include "ssScript.h"

namespace ss
{

	enum class eMonsterState
	{
		SPAWN,
		IDLE,
		MOVE,
		TRACER,
		JUMP,
		FALL,
		LADING,
		STUN,
		HIT,
		HIT_AFTER,	
		GUARD,
		NEARATTACK,
		NEARATTACK_AFTER,
		FARATTACK,
		DEAD,
		END
	};

	enum class eWolfBossState
	{
		SPAWN,
		IDLE,
		APPEAR,
		DISAPPEAR,
		BREATH_START,
		BREATHING,
		BREATH_END,
		HOWLING_START,
		HOWLING,
		HOWLING_END,
		DASH,
		STOM_START,
		STOMING,
		STOM_END,
		HIT,
		DEAD,
		END
	};



	// hp�� �׷��Ŵ� state ��ũ��Ʈ ���ٺ��̱�
	struct tMonsterInfo
	{
		float	m_fSpeed = 0;// �����̴� �ӵ� 

		float	m_fAttack = 0; // ���ݷ�
		float	m_fDefense = 0; // ����

		float	m_fDetectRange = 0; // ���� ����(���� ����) 
		float	m_fNearAttackRange = 0; // ���� ���� ����
		float	m_fFarAttackRange = 0; // ���Ÿ� ���� ����

		float   m_fCoolDown = 0; // ���� ��Ÿ��
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
		eWolfBossState m_eWolfBossState;


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

		eWolfBossState			 mCurWolfBossState;
		eWolfBossState			 mPrevWolfBossState;

		Vector2					mOringinColSize;
		Vector2					mOringinColCeter;

		float					m_fTime; // �ð� ������


		float                   mHitTime;
		bool                    mHit; // hit�������� 

	private:
		GameObject* m_pTarget;


	public:
		virtual void BindConstantBuffer();

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

	public:
		virtual void ChangeState(eWolfBossState state)
		{
			if (mCurWolfBossState == state)
				return;

			mCurWolfBossState = state;
		}

		virtual eWolfBossState GetPrevWolfState() { return mPrevWolfBossState; }
		virtual eWolfBossState GetCurWolfState() { return mCurWolfBossState; }


		virtual void SetHit(bool hit) { mHit = hit; }

	};
}


