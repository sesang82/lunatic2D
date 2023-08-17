#include "ssGoddessScript.h"

namespace ss
{
	GoddessScript::GoddessScript()
	{
		m_tMonsterInfo.m_fSpeed = 200.f;
		m_tMonsterInfo.m_fAttack = 10.f;
		m_tMonsterInfo.m_fDetectRange = 300.f;
	}
	GoddessScript::~GoddessScript()
	{
	}
	void GoddessScript::Initialize()
	{
	}
	void GoddessScript::Update()
	{
	}
	void GoddessScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void GoddessScript::OnCollisionStay(Collider2D* other)
	{
	}
	void GoddessScript::OnCollisionExit(Collider2D* other)
	{
	}
}