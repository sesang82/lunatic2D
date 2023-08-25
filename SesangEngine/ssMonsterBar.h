#pragma once
#include "ssProgressbar.h"

namespace ss
{
	// ���α׷����� UI (Normal ���� ����) 
    class MonsterBar :
        public Progressbar
    {
	public:
		MonsterBar();
		virtual ~MonsterBar();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;


	private:
		GameObject* mOwner;

	public:
		void SetOwner(GameObject* owner) { mOwner = owner; }
    };

}

