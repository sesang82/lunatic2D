#pragma once
#include "..\\Engine_SOURCE\\ssGameObject.h"


namespace gui
{
	class DebugObject : public ss::GameObject
	{
	public:
		DebugObject();
		~DebugObject();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	private:
	};
}
