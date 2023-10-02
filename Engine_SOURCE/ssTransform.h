#pragma once
#include "ssComponent.h"

namespace ss
{
	// 월드 행렬을 갖고 있다. 뷰행렬과 투영행렬은 카메라 컴포넌트가 갖고 있음
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
		bool	mFlip; // 좌우 반전

		// === 월드 행렬을 만들기 위한 재료
		Vector3 mRelativePos;
		Vector3 mRelativeRot;
		Vector3 mRelativeScale;

		Matrix mWorld; // 월드행렬을 만들기 위한 행렬
		Matrix mParentMat; // 부모의 행렬

		Vector3 mWorldPos;
		Vector3 mWorldRot;
		Vector3 mWorldScale;

		bool m_bIgnParentScale; // 부모 스케일 무시

		// ==== 방향 벡터 (좌우로 회전시 Up은 상관이 없지만, R과 F는 계속 바뀌므로
	// 방향을 알기 위해 방향 벡터를 만들어 놓는다.)
		Vector3 mRelativeUp;
		Vector3 mRelativeForward;
		Vector3 mRelativeRight;  // 왼쪽은 -를 붙이면 될 것

		Vector3 mWorldUp;
		Vector3 mWorldForward;
		Vector3 mWorldRight;

		// === 오브젝트간 부모 자식 관계 설정을 위함
		Transform* mParent; // 부모로 설정된 오브젝트의 트랜스폼을 갖고 있게 함 

		Vector3 mStoreOwnerPos; // 스토어 오너의 위치를 저장하기 위함

	public:
		void SetFlip() { mFlip = !mFlip; }
		bool GetFlip() { return mFlip; }

		void BindConstantBuffer();  // GPU에 생성한 상수 버퍼에 데이터 넣어주는 용도

		// == 부모 자식 관계 
		void SetParent(Transform* parent) { mParent = parent; }
		Transform* GetParent() { return mParent; }

		// 부모 스케일 무시
		void SetIgnoreParentScale(bool ignore) { m_bIgnParentScale = ignore; }

		// ==위치, 회전, 크기 (행렬)
		  // near와 far값에 걸치지만 않으면 됨. z값은 픽셀단위니 100씩 하자!
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

		// z값은 1이여야만 렌더링이 된다(원래 크기로)
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


		// === 방향 벡터 
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