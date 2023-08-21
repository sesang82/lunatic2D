#include "ssProjectileScript.h"
#include "ssState.h"
#include "ssGameState.h"
#include "ssGameObject.h"
#include "ssTime.h"

namespace ss
{
	ProjectileScript::ProjectileScript()
		: mReverse(false)
		, mMonsterPos(Vector3::Zero)
	{
	}
	ProjectileScript::~ProjectileScript()
	{
		
	}
	void ProjectileScript::Initialize()
	{
		//mMonster = (StoneEye*)GetOwner();

		mState = GetOwner()->GetComponent<CharacterState>();
		mAnimator = GetOwner()->GetComponent<Animator>();
		mTransform = GetOwner()->GetComponent<Transform>();
		mCollider = GetOwner()->GetComponent<Collider2D>();

	}
	void ProjectileScript::Update()
	{
	


		if (!mReverse) // ���������� ���� 
		{
			mMonsterPos.x += mTransform->Forward().x * 200.f * Time::DeltaTime();
		}

		else // �������� ���� 
		{

		}
		mTransform->SetPosition(mMonsterPos);
	}
	void ProjectileScript::OnCollisionEnter(Collider2D* other)
	{
		if (other->GetName() == L"Player")
		{
			mState = GameState::GetInst().GetState(L"Player");
			mState->SetCurrentHP(mState->GetCurrentHP() - 10);
		}
	}
	void ProjectileScript::OnCollisionStay(Collider2D* other)
	{
		// ���ݿ� �浹ü�� ���⼭ ���ֱ� 
	}
	void ProjectileScript::OnCollisionExit(Collider2D* other)
	{
	}
}