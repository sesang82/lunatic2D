#include "ssPlayerGuardColScript.h"
#include "ssGameObject.h"
#include "ssPlayerScript.h"

namespace ss
{
	PlayerGuardColScript::PlayerGuardColScript()
	{
	}
	PlayerGuardColScript::~PlayerGuardColScript()
	{
	}
	void PlayerGuardColScript::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();
		mCollider = GetOwner()->GetComponent<Collider2D>();
	}
	void PlayerGuardColScript::Update()
	{
		Vector3 ColPos = mTransform->GetPosition();
		mTransform->SetPosition(ColPos);

	}
	void PlayerGuardColScript::OnCollisionEnter(Collider2D* other)
	{	
			if (other->GetName() == L"StoneEyeProjectileCol")
			{
				Transform* tr = other->GetOwner()->GetComponent<Transform>();
				Vector3 BulletCurScale = tr->GetScale();

				// 플레이어 위치를 이용하여 총알 회전을 다르게 준다. 
				// 플레이어가 오른쪽을 보고 있다면 몬스터는 왼쪽을 보고 있으므로......
				if (mPlayerScript->GetCurDir().x == 1.0f)
				{
					// 총알을 회전 시킨다. (오른쪽 총알 기준) 

					BulletCurScale.x = abs(BulletCurScale.x);

				}

				else if (mPlayerScript->GetCurDir().x == -1.0f)
				{
					// 총알을 회전 시킨다. (오른쪽 총알 기준) 
					BulletCurScale.x = -abs(BulletCurScale.x);
				}

				tr->SetScale(BulletCurScale);

			}
	
	}
	void PlayerGuardColScript::OnCollisionStay(Collider2D* other)
	{
	}
	void PlayerGuardColScript::OnCollisionExit(Collider2D* other)
	{
	}
}