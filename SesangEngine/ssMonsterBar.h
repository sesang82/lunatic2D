#pragma once
#include "ssUI.h"

namespace ss
{
	// ���α׷����� UI (Normal ���� ����) 
    class MonsterBar :
        public UI
    {
	public:
		MonsterBar();
		virtual ~MonsterBar();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
    };

}

