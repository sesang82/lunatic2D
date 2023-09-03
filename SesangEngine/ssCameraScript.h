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
		GameObject* mTarget; // (��κ��� tr ���ͼ� �������� �ɵ�) 

		// ī�޶� ���ߴ� ������ ����, ���� ũ��
		int mCameraHeight;
		int mCameraWidth;
		Vector2 mMapSize; // ī�޶� ���� ������ ũ�� (���� ������� ���ϴ� ũ��)
		float someOffset;

		bool mbFllowMonster;
		bool mbReturnToPlayer;

		CameraOffset cameraOffset;


	public:
		// ���ӿ�����Ʈ�� ��ӹ��� �ٸ� �༮���� �μ��� ���� �� ������ ���ٸ� �ش� Ŭ���� ��� ������ ��Ŭ��� �ϸ� �ȴ�. 
		void SetTarget(GameObject* obj) { mTarget = obj; }


	};
}
