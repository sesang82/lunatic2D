#include "ssPistolBulletScript.h"
#include "ssGameObject.h"
#include "ssCollider2D.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"


namespace ss
{
	PistolBulletScript::PistolBulletScript()
		: mReverse(false)
		, mSpeed(80.f)
		, mIsGuard(false)
		, mAnimator(nullptr)
		, mTransform(nullptr)
		, mCollider(nullptr)
		, mState(nullptr)
		, mPlayer(nullptr)
		, mMonster(nullptr)
		, mMr(nullptr)
	{
	}
	PistolBulletScript::~PistolBulletScript()
	{
	}
	void PistolBulletScript::Initialize()
	{
		//(StoneEye*)mOriginOwner;

		mState = GetOwner()->GetComponent<CharacterState>();
		mAnimator = GetOwner()->GetComponent<Animator>();
		mTransform = GetOwner()->GetComponent<Transform>();
		mCollider = GetOwner()->GetComponent<Collider2D>();
		mMr = GetOwner()->GetComponent<MeshRenderer>();



		if (GetOwner()->GetName() == L"Pistolbullet_Big_ObjR")
		{
			mMr->SetMaterial(Resources::Find<Material>(L"Pistol_BulletBigMtrl"));
			mTransform->SetScale(Vector3(12.f, 5.f, 0.f));;

			// �浹ü ũ�⵵ ���⼭ �����ֱ� 

		}


		else if 
			(GetOwner()->GetName() == L"Pistolbullet_Small_ObjL")
		{
			mMr->SetMaterial(Resources::Find<Material>(L"Pistol_BulletSmallMtrl"));
			mTransform->SetScale(Vector3(9.f, 3.f, 0.f));;

		}





	}
	void PistolBulletScript::Update()
	{


		//StoneEyeScript* monScript = mOriginOwner->GetComponent<StoneEyeScript>();
		//CharacterState* StoneEyeState = mOriginOwner->GetComponent<CharacterState>();

		//if (StoneEyeState->GetCurrentHP() > 0.f)
		//{
		//	Vector3 PlayerDir = monScript->GetCurDir();
		//	Vector3 ArrowPos = mTransform->GetPosition();

		//	if (mIsGuard)
		//	{
		//		// �浹�� �߻����� ���, ������ ����� �ݴ�� �����Դϴ�.
		//		ArrowPos.x += (PlayerDir.x == 1.0f ? -1 : 1) * mSpeed * Time::DeltaTime();
		//	}
		//	else
		//	{
		//		// �������� ������
		//		ArrowPos.x += (PlayerDir.x == 1.0f ? 1 : -1) * mSpeed * Time::DeltaTime();
		//	}

		//	mTransform->SetPosition(ArrowPos);
		//}

	}
	void PistolBulletScript::OnCollisionEnter(Collider2D* other)
	{

	


	}
	void PistolBulletScript::OnCollisionStay(Collider2D* other)
	{
	}
	void PistolBulletScript::OnCollisionExit(Collider2D* other)
	{
	}
}