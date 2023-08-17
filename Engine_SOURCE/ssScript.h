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

		// �������� �浹�� ���õ� ������ �ۼ��ϴ� ����
		virtual void OnCollisionEnter(Collider2D* other) {};
		virtual void OnCollisionStay(Collider2D* other) {};
		virtual void OnCollisionExit(Collider2D* other) {};


		// �Լ� �����ε��� �˾Ƽ� �� 
		virtual void OnCollisionEnter(Collider2D* in, Collider2D* other) {};
		virtual void OnCollisionStay(Collider2D* in, Collider2D* other) {};
		virtual void OnCollisionExit(Collider2D* in, Collider2D* other) {};

		// �浹ü�� �ε����� �� �̺�Ʈ�� �߻���Ű�� ������ �ۼ��ϴ� ����(Ex. �浹ü�� �ε�ġ�� ���� �ʿ� ����) 
		virtual void OnTriggerEnter(Collider2D* collider) {};
		virtual void OnTriggerStay(Collider2D* collider) {};
		virtual void OnTriggerExit(Collider2D* collider) {};

	private:

	};
}
