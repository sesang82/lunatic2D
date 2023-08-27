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
		void SetTarget(GameObject* obj) { mTarget = obj; }


	public:
		GameObject* mTarget; // (��κ��� tr ���ͼ� �������� �ɵ�) 

		// ī�޶� ���ߴ� ������ ����, ���� ũ��
		int mCameraHeight;
		int mCameraWidth;
		Vector2 mMapSize; // ī�޶� ���� ������ ũ�� (���� ������� ���ϴ� ũ��)
		float someOffset;



	};
}
