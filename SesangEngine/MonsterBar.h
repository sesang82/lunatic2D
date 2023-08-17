#pragma once
#include "ssUI.h"

namespace ss
{
	// 프로그래스바 UI (Normal 몬스터 버전) 
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

