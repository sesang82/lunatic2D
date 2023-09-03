#pragma once
//#include "yaScript.h"
#include <ssScript.h>

namespace ss
{

	struct CameraOffset 
	{
		float leftOffset;
		float rightOffset;
	};


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

		CameraOffset cameraOffset;


	public:
		// 게임오브젝트를 상속받은 다른 녀석들을 인수로 넣을 떄 오류가 난다면 해당 클래스 헤더 파일을 인클루드 하면 된다. 
		void SetTarget(GameObject* obj) { mTarget = obj; }


	};
}
