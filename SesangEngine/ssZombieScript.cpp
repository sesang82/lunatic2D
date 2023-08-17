#include "ssZombieScript.h"

namespace ss
{
	ZombieScript::ZombieScript()
	{
		m_tMonsterInfo.m_fSpeed = 200.f;
		m_tMonsterInfo.m_fAttack = 10.f;
		m_tMonsterInfo.m_fDetectRange = 300.f;
	}
	ZombieScript::~ZombieScript()
	{
	}
	void ZombieScript::Initialize()
	{
	}
	void ZombieScript::Update()
	{
	}
	void ZombieScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void ZombieScript::OnCollisionStay(Collider2D* other)
	{
	}
	void ZombieScript::OnCollisionExit(Collider2D* other)
	{
	}
}