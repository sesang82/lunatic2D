#pragma once
#include "ssComponent.h"
#include "ssCollider2D.h"




namespace ss
{
	class Script : public Component
	{
	public:
		Script();
		virtual ~Script();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		// 물리적인 충돌과 관련된 로직을 작성하는 공간
		virtual void OnCollisionEnter(Collider2D* other) {};
		virtual void OnCollisionStay(Collider2D* other) {};
		virtual void OnCollisionExit(Collider2D* other) {};


		// 함수 오버로딩은 알아서 됨 
		virtual void OnCollisionEnter(Collider2D* in, Collider2D* other) {};
		virtual void OnCollisionStay(Collider2D* in, Collider2D* other) {};
		virtual void OnCollisionExit(Collider2D* in, Collider2D* other) {};

		// 충돌체와 부딪쳤을 때 이벤트를 발생시키는 로직을 작성하는 공간(Ex. 충돌체에 부딪치면 다음 맵에 가기) 
		virtual void OnTriggerEnter(Collider2D* collider) {};
		virtual void OnTriggerStay(Collider2D* collider) {};
		virtual void OnTriggerExit(Collider2D* collider) {};

	private:

	};
}
