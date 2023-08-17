#pragma once
//#include "yaScene.h"
#include "..\Engine_SOURCE\ssScene.h"

namespace ss
{
	class PlayScene : public Scene
	{
	public:
		PlayScene();
		virtual ~PlayScene();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	private:

	};
}
