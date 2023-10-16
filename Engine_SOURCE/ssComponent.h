#pragma once
#include "ssEntity.h"

namespace ss
{
	using namespace ss::enums;
	using namespace ss::math;



	// Ŭ������ ����� ������ �տ� class�� �ٿ����ϴµ�, ���� ���� �� ��� ������ class�� �ٿ����� ���̴�.
	// ������ �̷��� Ŭ���� ���� �̷������� ���� ������ �صθ� class Ű���带 �����ϰ� ����� �� �� �ִ�.
	class GameObject;

	class Component : public Entity
	{
	public:
		Component(eComponentType type);
		virtual ~Component();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		GameObject* GetOwner() { return mOwner; }
		void SetOwner(GameObject* owner) { mOwner = owner; }

		eComponentType GetType() { return mType; }
		UINT GetComNumber() { return (UINT)mType; }

	private:
		// ���� ������Ʈ�� � ������Ʈ�� ������ �ִ��� Ȯ���ϰų�, Ư�� Ÿ���� ������Ʈ���� ������� ������ �����Ϸ��� ���
		const eComponentType mType;

		// Ȱ��� ��
		/* ������Ʈ�� �����ϰ� �ִ� ���� ������Ʈ�� �����ϴ� ����Դϴ�.
		������Ʈ�� �ڽ��� ���� ���� ������Ʈ�� ���¿� ���� ������ �����ϰų�,
		���� ������Ʈ�� �ٸ� ������Ʈ�� �����Ϸ��� ��쿡 mOwner�� ����� �� �ֽ��ϴ�.
		���� ���, Ư�� �̺�Ʈ�� �߻����� �� ���� ������Ʈ�� �ٸ� ������Ʈ�� ��ȣ�� �����ų�,
		���� ������Ʈ�� ��ġ�� ȸ�� ���� ���¸� �����Ϸ��� ��쿡 mOwner�� ���� ���� ������Ʈ�� ������ �� �ֽ��ϴ�.*/
		GameObject* mOwner;
	};
}
