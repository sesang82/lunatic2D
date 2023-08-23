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

				// �÷��̾� ��ġ�� �̿��Ͽ� �Ѿ� ȸ���� �ٸ��� �ش�. 
				// �÷��̾ �������� ���� �ִٸ� ���ʹ� ������ ���� �����Ƿ�......
				if (mPlayerScript->GetCurDir().x == 1.0f)
				{
					// �Ѿ��� ȸ�� ��Ų��. (������ �Ѿ� ����) 

					BulletCurScale.x = abs(BulletCurScale.x);

				}

				else if (mPlayerScript->GetCurDir().x == -1.0f)
				{
					// �Ѿ��� ȸ�� ��Ų��. (������ �Ѿ� ����) 
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