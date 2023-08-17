#include "ssWolfScript.h"

namespace ss
{
	WolfScript::WolfScript()
	{
		m_tMonsterInfo.m_fSpeed = 200.f;
		m_tMonsterInfo.m_fAttack = 10.f;
		m_tMonsterInfo.m_fDetectRange = 300.f;
	}
	WolfScript::~WolfScript()
	{
	}
	void WolfScript::Initialize()
	{
	}
	void WolfScript::Update()
	{
	}
	void WolfScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void WolfScript::OnCollisionStay(Collider2D* other)
	{
	}
	void WolfScript::OnCollisionExit(Collider2D* other)
	{
	}
}