#include "ssSkeletonArcherScript.h"
#include "ssFSM.h"
#include "ssIdleState.h"
#include "ssTraceState.h"

namespace ss
{
	SkeletonArcherScript::SkeletonArcherScript()
	{
		m_tMonsterInfo.m_fSpeed = 200.f;
		m_tMonsterInfo.m_fAttack = 10.f;
		m_tMonsterInfo.m_fDetectRange = 300.f;
	}
	SkeletonArcherScript::~SkeletonArcherScript()
	{
	}
	void SkeletonArcherScript::Initialize()
	{
		mFSM = new FSM(this);
		mFSM->AddState(L"Idle", new IdleState());
		mFSM->AddState(L"Trace", new TraceState());
		//mFSM->AddState(L"Attack", new AttackState());
		mFSM->ChangeState(L"Idle");
	}
	void SkeletonArcherScript::Update()
	{
	}
	void SkeletonArcherScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void SkeletonArcherScript::OnCollisionStay(Collider2D* other)
	{
	}
	void SkeletonArcherScript::OnCollisionExit(Collider2D* other)
	{
	}
}