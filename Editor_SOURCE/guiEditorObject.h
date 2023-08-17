#pragma once
#include "..\\Engine_SOURCE\\ssGameObject.h"


namespace gui
{
	class EditorObject : public ss::GameObject
	{
	public:
		EditorObject();
		virtual ~EditorObject();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
	};
}