#include "ssQueenWolfScript.h"

namespace ss
{
	QueenWolfScript::QueenWolfScript()
	{
		m_tMonsterInfo.m_fSpeed = 200.f;
		m_tMonsterInfo.m_fAttack = 10.f;
		m_tMonsterInfo.m_fDetectRange = 300.f;
	}
	QueenWolfScript::~QueenWolfScript()
	{
	}
	void QueenWolfScript::Initialize()
	{

	}
	void QueenWolfScript::Update()
	{
	}
	void QueenWolfScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void QueenWolfScript::OnCollisionStay(Collider2D* other)
	{
	}
	void QueenWolfScript::OnCollisionExit(Collider2D* other)
	{
	}
}