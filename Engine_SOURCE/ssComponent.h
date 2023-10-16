#pragma once
#include "ssEntity.h"

namespace ss
{
	using namespace ss::enums;
	using namespace ss::math;



	// 클래스를 멤버로 쓰려면 앞에 class를 붙여야하는데, 여러 개를 쓸 경우 일일이 class를 붙여야할 것이다.
	// 때문에 이렇게 클래스 위에 이런식으로 전방 선언을 해두면 class 키워드를 생략하고 멤버를 쓸 수 있다.
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
		// 게임 오브젝트가 어떤 컴포넌트를 가지고 있는지 확인하거나, 특정 타입의 컴포넌트만을 대상으로 연산을 수행하려는 경우
		const eComponentType mType;

		// 활용법 ↓
		/* 컴포넌트를 소유하고 있는 게임 오브젝트를 참조하는 멤버입니다.
		컴포넌트는 자신이 속한 게임 오브젝트의 상태에 따라 동작을 변경하거나,
		게임 오브젝트의 다른 컴포넌트에 접근하려는 경우에 mOwner를 사용할 수 있습니다.
		예를 들어, 특정 이벤트가 발생했을 때 게임 오브젝트의 다른 컴포넌트에 신호를 보내거나,
		게임 오브젝트의 위치나 회전 등의 상태를 변경하려는 경우에 mOwner를 통해 게임 오브젝트에 접근할 수 있습니다.*/
		GameObject* mOwner;
	};
}
