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

		void SetRotation(const Vector3& rotationInDegrees)
		{
			mRelativeRot.x = XMConvertToRadians(rotationInDegrees.x);
			mRelativeRot.y = XMConvertToRadians(rotationInDegrees.y);
			mRelativeRot.z = XMConvertToRadians(rotationInDegrees.z);
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

	



	};
}