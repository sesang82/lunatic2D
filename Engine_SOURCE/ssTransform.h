#pragma once
#include "ssComponent.h"

namespace ss
{
	// ���� ����� ���� �ִ�. ����İ� ��������� ī�޶� ������Ʈ�� ���� ����
	class Transform : public Component
	{
	public:
		Transform();
		~Transform();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	private:
		bool	mFlip; // �¿� ����

		// === ���� ����� ����� ���� ���
		Vector3 mRelativePos;
		Vector3 mRelativeRot;
		Vector3 mRelativeScale;

		Matrix mWorld; // ��������� ����� ���� ���
		Matrix mParentMat; // �θ��� ���

		Vector3 mWorldPos;
		Vector3 mWorldRot;
		Vector3 mWorldScale;

		bool m_bIgnParentScale; // �θ� ������ ����

		// ==== ���� ���� (�¿�� ȸ���� Up�� ����� ������, R�� F�� ��� �ٲ�Ƿ�
	// ������ �˱� ���� ���� ���͸� ����� ���´�.)
		Vector3 mRelativeUp;
		Vector3 mRelativeForward;
		Vector3 mRelativeRight;  // ������ -�� ���̸� �� ��

		Vector3 mWorldUp;
		Vector3 mWorldForward;
		Vector3 mWorldRight;

		// === ������Ʈ�� �θ� �ڽ� ���� ������ ����
		Transform* mParent; // �θ�� ������ ������Ʈ�� Ʈ�������� ���� �ְ� �� 

		Vector3 mStoreOwnerPos; // ����� ������ ��ġ�� �����ϱ� ����

	public:
		void SetFlip() { mFlip = !mFlip; }
		bool GetFlip() { return mFlip; }

		void BindConstantBuffer();  // GPU�� ������ ��� ���ۿ� ������ �־��ִ� �뵵

		// == �θ� �ڽ� ���� 
		void SetParent(Transform* parent) { mParent = parent; }
		Transform* GetParent() { return mParent; }

		// �θ� ������ ����
		void SetIgnoreParentScale(bool ignore) { m_bIgnParentScale = ignore; }

		// ==��ġ, ȸ��, ũ�� (���)
		  // near�� far���� ��ġ���� ������ ��. z���� �ȼ������� 100�� ����!
		void SetPosition(Vector3 position) { mRelativePos = position; }
		void SetPosition(float x, float y, float z) { mRelativePos = Vector3(x, y, z); }

		void SetRotation(Vector3 rot)
		{
			if (rot.z <= 1.0f && rot.z >= 0.0f)
			{
				mRelativeRot = rot;

			}
			else
			{
				const float PI = 3.141592;
				mRelativeRot.z = rot.z * PI / 180;
			}
		}
		void SetRotation(float x, float y, float z) { mRelativeRot = Vector3(x, y, z); }

		// z���� 1�̿��߸� �������� �ȴ�(���� ũ���)
		void SetScale(Vector3 scale) { mRelativeScale = scale; }
		void SetScale(float x, float y, float z) { mRelativeScale = Vector3(x, y, z); }

		Vector3 GetPosition() { return mRelativePos; }
		Vector3 GetRotation() { return mRelativeRot; }
		Vector3 GetScale() { return mRelativeScale; }

		const Matrix& GetWorldMatrix() const { return mWorld; }
		const Matrix& GetParentMatrix() const { return mParentMat; }

		const Vector3& GetWorldPosition() const { return mWorldPos; }
		const Vector3& GetWorldRotation() const { return mWorldRot; }
		const Vector3& GetWorldScale() const { return mWorldScale; }


		// === ���� ���� 
		const Vector3& Forward() { return mRelativeForward; }
		const Vector3& Right() { return mRelativeRight; }
		const Vector3& Up() { return mRelativeUp; }
		const Vector3& WorldForward() { return mWorldForward; }
		const Vector3& WorldRight() { return mWorldRight; }
		const Vector3& WorldUp() { return mWorldUp; }

	
	public:
		void StoreOwnerPos(Vector3 _StorePos) { mStoreOwnerPos = _StorePos;  }
		Vector3 GetStoreOwnerPos() { return mStoreOwnerPos;  }


	public:
		Vector2 GetWorldCenterUp() const { return Vector2(mWorldPos.x, mWorldPos.y + mWorldScale.y * 0.5f); }
		Vector2 GetWorldCenterBottom() const { return Vector2(mWorldPos.x, mWorldPos.y - mWorldScale.y * 0.5f); }
		
		Vector2 GetWorldLeftTop() const { return Vector2(mWorldPos.x - mWorldScale.x * 0.5f, mWorldPos.y + mWorldScale.y * 0.5f); }
		Vector2 GetWorldLeftCenter() const { return Vector2(mWorldPos.x - mWorldScale.x * 0.5f, mWorldPos.y); }
		Vector2 GetWorldLeftBottom() const { return Vector2(mWorldPos.x - mWorldScale.x * 0.5f, mWorldPos.y - mWorldScale.y * 0.5f); }
		
		Vector2 GetWorldRightTop() const { return Vector2(mWorldPos.x + mWorldScale.x * 0.5f, mWorldPos.y + mWorldScale.y * 0.5f); }
		Vector2 GetWorldRightCenter() const { return Vector2(mWorldPos.x + mWorldScale.x * 0.5f, mWorldPos.y); }
		Vector2 GetWorldRightBottom() const { return Vector2(mWorldPos.x + mWorldScale.x * 0.5f, mWorldPos.y - mWorldScale.y * 0.5f); }
		
		Vector4 GetWorldLTRB() const
		{
			return Vector4(mWorldPos.x - mWorldScale.x * 0.5f, mWorldPos.y + mWorldScale.y * 0.5f
				, mWorldPos.x + mWorldScale.x * 0.5f, mWorldPos.y - mWorldScale.y * 0.5f);
		}


	};
}