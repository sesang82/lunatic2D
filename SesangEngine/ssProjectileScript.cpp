#include "ssProjectileScript.h"
#include "ssState.h"
#include "ssGameState.h"
#include "ssGameObject.h"
#include "ssTime.h"
#include "ssStoneEye.h"
#include "ssStoneEyeScript.h"


namespace ss
{
	ProjectileScript::ProjectileScript()
		: mReverse(false)
		, mSpeed(80.f)
	{
	}
	ProjectileScript::~ProjectileScript()
	{
		
	}
	void ProjectileScript::Initialize()
	{
		//(StoneEye*)mOriginOwner;

		mState = GetOwner()->GetComponent<CharacterState>();
		mAnimator = GetOwner()->GetComponent<Animator>();
		mTransform = GetOwner()->GetComponent<Transform>();
		mCollider = GetOwner()->GetComponent<Collider2D>();


	}
	void ProjectileScript::Update()
	{
	
		// 현재 위치 값 갖고 옴 
		(StoneEye*)mOriginOwner;

		Vector3 PlayerDir = mOriginOwner->GetComponent<StoneEyeScript>()->GetCurDir();

		Vector3 ArrowPos = mTransform->GetPosition();
		//Vector3 curpos = Vector3::Zero;

		if (PlayerDir.x == 1.0f) // 원점에 이미지가 머물러있는건 0,0,0인데 pos값 따로 안주면 당연히 위치 값이 계속 0 나올것임 ... 
		{
			ArrowPos.x += mSpeed * Time::DeltaTime();
		}

		else if (PlayerDir.x == -1.0f)
		{
	
			ArrowPos.x += -mSpeed * Time::DeltaTime();
		}

		mTransform->SetPosition(ArrowPos);
	}
	void ProjectileScript::OnCollisionEnter(Collider2D* other)
	{
		if (other->GetOwner()->GetName() == L"Player")
		{
			mState = GameState::GetInst().GetState(L"Player");
			mState->SetCurrentHP(mState->GetCurrentHP() - 10);
		
			GetOwner()->SetState(ss::GameObject::eState::Dead);
		}
	}
	void ProjectileScript::OnCollisionStay(Collider2D* other)
	{
		// 공격용 충돌체를 여기서 없애기 
	}
	void ProjectileScript::OnCollisionExit(Collider2D* other)
	{
	}
}