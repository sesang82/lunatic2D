#include "ssMonsterAttackColScript.h"
#include "ssGameObject.h"
#include "ssPlayerScript.h"
#include "ssPlayer.h"


namespace ss
{
	MonsterAttackColScript::MonsterAttackColScript()
	{
	}
	MonsterAttackColScript::~MonsterAttackColScript()
	{
	}
	void MonsterAttackColScript::Initialize()
	{
		mState = GetOwner()->GetComponent<CharacterState>(); // 보스용으로 쓰기 
		mAnimator = GetOwner()->GetComponent<Animator>();
		mTransform = GetOwner()->GetComponent<Transform>();
		mCollider = GetOwner()->GetComponent<Collider2D>();

	}
	void MonsterAttackColScript::Update()
	{
	}

	void MonsterAttackColScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void MonsterAttackColScript::OnCollisionStay(Collider2D* other)
	{
	}
	void MonsterAttackColScript::OnCollisionExit(Collider2D* other)
	{
	}
}
