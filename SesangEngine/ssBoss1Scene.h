#pragma once
#include "..\Engine_SOURCE\ssScene.h"

namespace ss
{
	class Boss1Scene :
		public Scene
	{
	public:
		Boss1Scene();
		virtual ~Boss1Scene();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;
	};


}

