#pragma once
#include "..\Engine_SOURCE\ssScene.h"

namespace ss
{
    class Boss4Scene :
        public Scene
    {
	public:
		Boss4Scene();
		virtual ~Boss4Scene();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
    };
}

