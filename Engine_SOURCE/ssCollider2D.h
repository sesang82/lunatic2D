#pragma once
#include "ssComponent.h"
#include "ssTransform.h"


namespace ss
{
	class Collider2D : public Component
	{
	public:
		Collider2D();
		~Collider2D();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	public:
		virtual void OnCollisionEnter(Collider2D* other);
		virtual void OnCollisionStay(Collider2D* other);
		virtual void OnCollisionExit(Collider2D* other);

		virtual void OnCollisionEnter(Collider2D* in, Collider2D* other);
		virtual void OnCollisionStay(Collider2D* in, Collider2D* other);
		virtual void OnCollisionExit(Collider2D* in, Collider2D* other);

	public:
		virtual void OnTriggerEnter(Collider2D* collider);
		virtual void OnTriggerStay(Collider2D* collider);
		virtual void OnTirggerExit(Collider2D* collider);

		void SetType(eColliderType type) { mType = type; }
		void SetSize(Vector2 size) { mSize = size; } // 충돌체 크기 임의로 조정
		void SetCenter(Vector2 size) { mCenter = size; } // 센터 값은 그냥 쓰지 말기 (오류)

		UINT GetColliderID() { return mColliderID; }

		eColliderType GetType() { return mType; }
		Vector3 GetPosition() { return mPosition; }
		Vector2 GetSize() { return mSize; }
		Vector2 GetCenter() { return mCenter; }
		Vector3 GetScale() { return mScale; }

		Matrix GetFinalMatrix() { return FinalMatrix; }

		int GetCount() { return mCount; }

	private:
		static UINT mColliderNumber;
		UINT mColliderID;

		eColliderType mType;
		Transform* mTransform;

		eColliderType	m_Type;


		Vector3 mScale;



		// 충돌체의 최종 위치를 따로 저장해두는 멤버 만들어둠 
		Vector3 mPosition;
		Vector2 mSize; // scale
		Vector2 mCenter;

		Matrix FinalMatrix;

		int mCount = 0; // 충돌횟수 
	};
}
