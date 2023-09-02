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
		GameObject* mTarget; // (��κ��� tr ���ͼ� �������� �ɵ�) 

		// ī�޶� ���ߴ� ������ ����, ���� ũ��
		int mCameraHeight;
		int mCameraWidth;
		Vector2 mMapSize; // ī�޶� ���� ������ ũ�� (���� ������� ���ϴ� ũ��)
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
			mbFllowMonster = false;  // ���� ���� ����
		}

	};
}
