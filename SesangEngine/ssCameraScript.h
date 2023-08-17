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
		GameObject* mTarget; // (얘로부터 tr 얻어와서 정보따면 될듯) 
	};
}
