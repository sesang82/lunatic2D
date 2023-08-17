#pragma once
#include "ssBaseScene.h"

namespace ss
{
    class LibraryScene :
        public BaseScene
    {
	public:
		LibraryScene();
		virtual ~LibraryScene();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;

	private:
		Camera* mCamera;
    };
}
