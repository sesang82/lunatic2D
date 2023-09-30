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
		float mPosOffset; // �÷��̾��� �⺻ ��ġ�� ī�޶� �󸶳� ������    // ī�޶� �÷��̾ ����ٴϴµ� �󸶳� ������ �������� ���� ��
		float someOffset;

		bool mbFllowMonster;
		bool mbReturnToPlayer;

		CameraOffset cameraOffset;
		
		// shake ���� 
		bool isShaking = false;
		float shakeDuration = 0.0f;
		float shakeTimer = 0.0f;
		float shakeMagnitude = 0.0f;
		Vector3 originalPosition;

	public:
		// ���ӿ�����Ʈ�� ��ӹ��� �ٸ� �༮���� �μ��� ���� �� ������ ���ٸ� �ش� Ŭ���� ��� ������ ��Ŭ��� �ϸ� �ȴ�. 
		void SetTarget(GameObject* obj) { mTarget = obj; }

		void StartShake(float duration, float magnitude);
		void SetOffset(float left, float right) 
		{
			cameraOffset.leftOffset = left, cameraOffset.rightOffset = right;
		}


	};
}
