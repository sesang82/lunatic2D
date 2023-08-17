#include "ssCollisionManager.h"
#include "ssGameObject.h"
#include "ssScene.h"
#include "ssSceneManager.h"
#include "ssLayer.h"
#include "ssCollider2D.h"


namespace ss
{

	using namespace DirectX;

	std::bitset<LAYER_MAX> CollisionManager::mMatrix[LAYER_MAX] = {};
	std::map<UINT64, bool> CollisionManager::mCollisionMap = {};

	void CollisionManager::Initialize()
	{
	}

	void CollisionManager::Update()
	{
		for (UINT column = 0; column < (UINT)eLayerType::End; column++)
		{
			for (UINT row = 0; row < (UINT)eLayerType::End; row++)
			{
				if (mMatrix[column][row] == true)
				{
					LayerCollision((eLayerType)column, (eLayerType)row);
				}
			}
		}
	}

	void CollisionManager::LayerCollision(eLayerType left, eLayerType right)
	{
		Scene* activeScene = SceneManager::GetActiveScene();

		const std::vector<GameObject*>& lefts
			= activeScene->GetLayer(left).GetGameObjects();
		const std::vector<GameObject*>& rights
			= activeScene->GetLayer(right).GetGameObjects();

		for (GameObject* leftObj : lefts)
		{
			Collider2D* leftCol = leftObj->GetComponent<Collider2D>();
			if (leftCol == nullptr)
				continue;
			if (leftObj->GetState()
				!= GameObject::eState::Active)
				continue;

			for (GameObject* rightObj : rights)
			{
				Collider2D* rightCol = rightObj->GetComponent<Collider2D>();
				if (rightCol == nullptr)
					continue;
				if (leftObj == rightObj)
					continue;
				if (rightObj->GetState()
					!= GameObject::eState::Active)
					continue;

				ColliderCollision(leftCol, rightCol);
			}
		}
	}

	void CollisionManager::ColliderCollision(Collider2D* left, Collider2D* right)
	{
		// 두 충돌체의 ID bool값을 확인
		ColliderID id = {};
		id.left = left->GetColliderID();
		id.right = right->GetColliderID();

		// 충돌정보를 가져온다
		std::map<UINT64, bool>::iterator iter
			= mCollisionMap.find(id.id);

		if (iter == mCollisionMap.end())
		{
			mCollisionMap.insert(std::make_pair(id.id, false));
			iter = mCollisionMap.find(id.id);
		}

		if (Intersect(left, right))
		{
			// 충돌
			if (iter->second == false)
			{
				//최초 충돌
				left->OnCollisionEnter(right);
				right->OnCollisionEnter(left);

				left->OnCollisionEnter(left, right);
				right->OnCollisionEnter(right, left);
			
				// 시간초 지나면 충돌 끝 
				/*left->time = mTime;
				right->time = mTime;*/
			}
			else
			{
				// 충돌 중
				left->OnCollisionStay(right);
				right->OnCollisionStay(left);

				left->OnCollisionStay(left, right);
				right->OnCollisionStay(right, left);
			}
			iter->second = true;
		}
		else
		{
			// 충돌 X
			if (iter->second == true)
			{
				// 충돌하고 있다가 나갈떄
				left->OnCollisionExit(right);
				right->OnCollisionExit(left);

				left->OnCollisionExit(left, right);
				right->OnCollisionExit(right, left);
				
			}
			iter->second = false;
		}
	}

	bool CollisionManager::Intersect(Collider2D* left, Collider2D* right)
	{
		if (eColliderType::Rect == left->GetType() && eColliderType::Rect == right->GetType())
		{
			// Rect vs Rect 
		// 0 --- 1
		// |     |
		// 3 --- 2
			Vector3 arrLocalPos[4] =
			{
			   Vector3{-0.5f, 0.5f, 0.0f}
			   ,Vector3{0.5f, 0.5f, 0.0f}
			   ,Vector3{0.5f, -0.5f, 0.0f}
			   ,Vector3{-0.5f, -0.5f, 0.0f}
			};

			Transform* leftTr = left->GetOwner()->GetComponent<Transform>();
			Transform* rightTr = right->GetOwner()->GetComponent<Transform>();

			Vector2 leftCenter = left->GetCenter();
			Vector2 rightCenter = right->GetCenter();

			Transform Final_LeftTr = *leftTr;
			Transform Final_RightTr = *rightTr;

			Vector3 FinalLeftPos;
			Vector3 FinalRightPos;

			FinalLeftPos.x = leftTr->GetPosition().x + leftCenter.x;
			FinalLeftPos.y = leftTr->GetPosition().y + leftCenter.y;
			FinalLeftPos.z = leftTr->GetPosition().z;

			FinalRightPos.x = rightTr->GetPosition().x + rightCenter.x;
			FinalRightPos.y = rightTr->GetPosition().y + rightCenter.y;
			FinalRightPos.z = rightTr->GetPosition().z;

			Final_LeftTr.SetPosition(FinalLeftPos);
			Final_RightTr.SetPosition(FinalRightPos);

			Matrix leftMatrix = Final_LeftTr.GetWorldMatrix();
			Matrix rightMatrix = Final_RightTr.GetWorldMatrix();

			Vector3 Axis[4] = {};

			Vector3 leftScale = Vector3(left->GetSize().x, left->GetSize().y, 1.0f);
			Matrix finalLeft = Matrix::CreateScale(leftScale);
			finalLeft *= leftMatrix;

			Vector3 rightScale = Vector3(right->GetSize().x, right->GetSize().y, 1.0f);
			Matrix finalRight = Matrix::CreateScale(rightScale);
			finalRight *= rightMatrix;

			Axis[0] = Vector3::Transform(arrLocalPos[1], finalLeft);
			Axis[1] = Vector3::Transform(arrLocalPos[3], finalLeft);
			Axis[2] = Vector3::Transform(arrLocalPos[1], finalRight);
			Axis[3] = Vector3::Transform(arrLocalPos[3], finalRight);

			Axis[0] -= Vector3::Transform(arrLocalPos[0], finalLeft);
			Axis[1] -= Vector3::Transform(arrLocalPos[0], finalLeft);
			Axis[2] -= Vector3::Transform(arrLocalPos[0], finalRight);
			Axis[3] -= Vector3::Transform(arrLocalPos[0], finalRight);

			for (size_t i = 0; i < 4; i++)
				Axis[i].z = 0.0f;

			Vector3 vc = Final_LeftTr.GetPosition() - Final_RightTr.GetPosition();
			vc.z = 0.0f;

			Vector3 centerDir = vc;
			for (size_t i = 0; i < 4; i++)
			{
				Vector3 vA = Axis[i];

				float projDistance = 0.0f;
				for (size_t j = 0; j < 4; j++)
				{
					projDistance += fabsf(Axis[j].Dot(vA) / 2.0f);
				}

				if (projDistance < fabsf(centerDir.Dot(vA)))
					return false;
			}

			return true;


		}


		else if (eColliderType::Circle == left->GetType() && eColliderType::Circle == right->GetType())
		{
			Vector3 CenterToLeft = left->GetPosition();
			Vector3 CenterToRight = right->GetPosition();

			float distance = Vector3::Distance(CenterToLeft, CenterToRight);

			float radius_L = (left->GetOwner()->GetComponent<Transform>()->GetScale().x * left->GetSize().x) / 2.f;
			float radius_R = (right->GetOwner()->GetComponent<Transform>()->GetScale().x * right->GetSize().x) / 2.f;

			if (radius_L + radius_R >= distance)
				return true;
		}

		return false;
	}

	void CollisionManager::SetLayer(eLayerType left, eLayerType right, bool enable)
	{
		UINT row = -1;
		UINT col = -1;

		UINT iLeft = (UINT)left;
		UINT iRight = (UINT)right;

		if (iLeft <= iRight)
		{
			row = iLeft;
			col = iRight;
		}
		else
		{
			row = iRight;
			col = iLeft;
		}

		mMatrix[col][row] = enable;
	}

	void CollisionManager::Clear()
	{
		mMatrix->reset();
		mCollisionMap.clear();
	}
}
