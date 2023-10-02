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

	enum class eBoss2_Phase1
	{
		INTRO,
		IDLE,
		STOMP_READY,
		STOMP_ING,
		STOMP_END,
		ENERGYBALL_READY,
		ENERGYBALL_ING,
		ENERGYBALL_END,
		WIND,
		DIE,
	};

	enum class eBoss2_Phase2
	{
		INTRO,
		INTRO_END,
		IDLE,
		MOVE_FRONT,
		MOVE_BACK,
		DIAGONAL_ATTACK_DIAGONAL,
		DIAGONAL_ATTACK_VERTICAL,
		ENERGYBALL_START,
		ENERGYBALL_ING,
		ENERGYBALL_END,
		COUNTER_START,
		COUNTER_ING,
		COUNTER_END,
		COUNTER_HIT,
		GROUNDSPEAR,
		SHIELDBEAM_START,
		SHIELDBEAM_ING,
		SHIELDBEAM_END,
		SUMMONSPEAR_START,
		SUMMONSPEAR_ING,
		SUMMONSPEAR_END,
		HIT,
		DIE,
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
		friend class GolemNearRangeScript;



	public:
		MonsterScript();
		virtual ~MonsterScript();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		virtual void OnCollisionEnter(Collider2D* other) override;
		virtual void OnCollisionStay(Collider2D* other) override;
		virtual void OnCollisionExit(Collider2D* other) override;



	protected:
		tMonsterInfo m_tMonsterInfo;
		eWolfBossState m_eWolfBossState;


		class FSM* mFSM;

		class Animator* mAnimator;
		class Rigidbody2D* mRigidbody;
		class Transform* mTransform;
		class Collider2D* mCollider;
		class MeshRenderer* mMeshRenderer;
		class CharacterState* mCharacterState;

		class Player* mPlayer;
		class GameObject* mBossHPFrame;
		class GameObject* mBossHPBar;
		class GameObject* mPlayerUIFrame;
		//class Monster*			mMonster;

		class GameObject* mMainCamera;
		class GameObject* mUICamera;

		Vector3					mPrevDir;
		Vector3					mCurDir;

		eMonsterState			 mCurState;
		eMonsterState			 mPrevState;

		eWolfBossState			 mCurWolfBossState;
		eWolfBossState			 mPrevWolfBossState;

		eBoss2_Phase1           mCurBoss2_Phase1_State;
		eBoss2_Phase1           mPrevBoss2_Phase1_State;

		eBoss2_Phase2           mCurBoss2_Phase2_State;
		eBoss2_Phase2           mPrevBoss2_Phase2_State;

		Vector2					mOringinColSize;
		Vector2					mOringinColCeter;

		float					m_fTime; // 시간 측정용


		float                   mHitTime;
		bool                    mHit; // hit상태인지 

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


		virtual void ChangeState(eBoss2_Phase1 state)
		{
			mCurBoss2_Phase1_State = state;
		}

		virtual void ChangeState(eBoss2_Phase2 state)
		{
			mCurBoss2_Phase2_State = state;
		}


		virtual eBoss2_Phase1 GetPrevBoss2_Phase1_State() { return mPrevBoss2_Phase1_State; }
		virtual eBoss2_Phase1 GetCurBoss2_Phase1_State() { return mCurBoss2_Phase1_State; }

		virtual eBoss2_Phase2 GetPrevBoss2_Phase2_State() { return mPrevBoss2_Phase2_State; }
		virtual eBoss2_Phase2 GetCurBoss2_Phase2_State() { return mCurBoss2_Phase2_State; }



		virtual void SetHit(bool hit) { mHit = hit; }
		bool GetHit() { return mHit; }


		void SetUI(GameObject* ui) { mBossHPFrame = ui; }
		GameObject* GetUI() { return mBossHPFrame; }
		
		
		void SetUICamera(GameObject* ui) { mUICamera = ui; }
		GameObject* GetUICamera() { return mUICamera; }

		void SetMainCamera(GameObject* ui) { mMainCamera = ui; }
		GameObject* GetMainCamera() { return mMainCamera; }

	};
}