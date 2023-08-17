#pragma once
#include "..\Engine_SOURCE\ssScene.h"

namespace ss
{
    class Boss3Scene :
        public Scene
    {
	public:
		Boss3Scene();
		virtual ~Boss3Scene();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
    };
}

