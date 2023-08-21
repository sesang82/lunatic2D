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

		if (!mReverse) // ������ �̹����� �ӹ����ִ°� 0,0,0�ε� pos�� ���� ���ָ� �翬�� ��ġ ���� ��� 0 ���ð��� ... 
		{
			ArrowPos.x += fabs(ArrowPos.x * Time::DeltaTime());
		}

		else // �������� ���� 
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
		// ���ݿ� �浹ü�� ���⼭ ���ֱ� 
	}
	void ProjectileScript::OnCollisionExit(Collider2D* other)
	{
	}
}