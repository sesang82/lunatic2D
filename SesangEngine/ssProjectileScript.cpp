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
	
		//Transform* stoneTR = mOriginOwner->GetComponent<Transform>();

		//Vector3 Monsterpos = stoneTR->GetPosition();


		Vector3 ArrowPos = mTransform->GetPosition();
		//Vector3 curpos = Vector3::Zero;

		if (!mReverse) // 원점에 이미지가 머물러있는건 0,0,0인데 pos값 따로 안주면 당연히 위치 값이 계속 0 나올것임 ... 
		{
			ArrowPos.x += fabs(ArrowPos.x * Time::DeltaTime());
		}

		else // 왼쪽으로 나감 
		{
			ArrowPos.x += -1.0f *(fabs(ArrowPos.x * Time::DeltaTime()));
		}

		mTransform->SetPosition(ArrowPos);
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
		// 공격용 충돌체를 여기서 없애기 
	}
	void ProjectileScript::OnCollisionExit(Collider2D* other)
	{
	}
}