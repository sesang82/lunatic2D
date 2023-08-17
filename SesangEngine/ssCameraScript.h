#pragma once
//#include "yaScript.h"
#include <ssScript.h>

namespace ss
{
	class player;

	class CameraScript : public Script
	{
	public:
		virtual void Update() override;

	public:
		void SetTarget(GameObject* obj) { mTarget = obj; }


	public:
		GameObject* mTarget; // (��κ��� tr ���ͼ� �������� �ɵ�) 
	};
}
