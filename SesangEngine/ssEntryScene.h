#pragma once
#include "ssBaseScene.h"

namespace ss
{
	class EntryScene :
		public BaseScene
	{
	public:
		EntryScene();
		virtual ~EntryScene();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;


	};
}

