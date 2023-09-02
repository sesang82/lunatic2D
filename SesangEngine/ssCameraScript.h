#pragma once
//#include "yaScript.h"
#include <ssScript.h>

namespace ss
{
	class player;

	class CameraScript : public Script
	{
	public:
		virtual void Initialize() override;
		virtual void Update() override;


	public:
		GameObject* mTarget; // (얘로부터 tr 얻어와서 정보따면 될듯) 

		// 카메라가 비추는 영역의 가로, 세로 크기
		int mCameraHeight;
		int mCameraWidth;
		Vector2 mMapSize; // 카메라가 비출 영역의 크기 (내가 마음대로 정하는 크기)
		float someOffset;

		bool mbFllowMonster;
		bool mbReturnToPlayer;


	public:
		void SetTarget(GameObject* obj) { mTarget = obj; }

		void ChangeTarget(GameObject* newTarget, bool followMonster = false)
		{
			mTarget = newTarget;
			mbFllowMonster = followMonster;
		}

		void ReturnToPlayer(GameObject* player)
		{
			mbReturnToPlayer = true;
			mbFllowMonster = false;  // 몬스터 추적 중지
		}

	};
}
