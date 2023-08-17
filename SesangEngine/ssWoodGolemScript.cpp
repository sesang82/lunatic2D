#include "ssWoodGolemScript.h"

namespace ss
{
	WoodGolemScript::WoodGolemScript()
	{
		m_tMonsterInfo.m_fSpeed = 200.f;
		m_tMonsterInfo.m_fAttack = 10.f;
		m_tMonsterInfo.m_fDetectRange = 300.f;
	}
	WoodGolemScript::~WoodGolemScript()
	{
	}
	void WoodGolemScript::Initialize()
	{
	}
	void WoodGolemScript::Update()
	{
	}
	void WoodGolemScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void WoodGolemScript::OnCollisionStay(Collider2D* other)
	{
	}
	void WoodGolemScript::OnCollisionExit(Collider2D* other)
	{
	}
}