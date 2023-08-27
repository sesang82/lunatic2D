#include "ssCameraScript.h"
#include "ssTransform.h"
#include "ssGameObject.h"
#include "ssTime.h"
#include "ssInput.h"
#include "ssPlayer.h"
#include "ssSceneManager.h"
#include "ssPlayer.h"
#include "ssCamera.h"
#include "ssRenderer.h"
#include "ssApplication.h"

extern ss::Application application;

namespace ss
{
	void CameraScript::Initialize()
	{
		mCameraHeight = 0;
		mCameraWidth = 0;
		mMapSize = Vector2(800.0f, 448.f); // ��ü ���� ���� ũ�� 
		someOffset = 75.f; //  ī�޶� ȭ���� ��� �Ǵ� �ϴܿ� ������ �ٰ����� �ʰ� �ϱ� ���� ��
	}


	void CameraScript::Update()
	{

		// ī�޶��� Ʈ������ ����
		Transform* Cam_tr = GetOwner()->GetComponent<Transform>();

		//// ī�޶� ���ߴ� ������ ����, ���� ũ���� ������ ���Ѵ�. 

		//mCameraHeight = renderer::mainCamera->GetSize();

		//mCameraWidth = mCameraHeight * 1600 / 896;



		// �÷��̾��� Ʈ������ ����
		if (mTarget)
		{
			Transform* Player_tr = mTarget->GetComponent<Transform>();
			Vector3 PlayerPos = Player_tr->GetPosition();
			Cam_tr->SetPosition(Vector3(PlayerPos.x, PlayerPos.y + 75.f, 990.f));
			// x���ٰ� +�� �����ָ� ó���� ������ �� �ش� ī�޶� �� ��ġ���� �����ϱ� �� 



			//Vector2 Center = Vector2(Vector2::Zero); // ī�޶� 0,0�� �����Ƿ� �̷��� ��


			//float lx = mMapSize.x - mCameraWidth; // ī�޶� ���η� �̵��� �� �ִ� ����
			//float clampX = std::clamp(PlayerPos.x, Center.x - lx, Center.x + lx);

			//float ly = mMapSize.y - mCameraWidth; // ī�޶� ���η� �̵��� �� �ִ� ����
			//float clampY = std::clamp(PlayerPos.y, Center.y - ly, Center.y + ly);
			//
			//Cam_tr->SetPosition(Vector3(clampX, clampY + 75.f, -10.f));
				
		}
	
		else
		{	
			Cam_tr->SetPosition(Vector3(0.f, 0.f, -10.f));
		}


		HWND hWnd = application.GetHwnd();
		RECT winRect = {};
		GetClientRect(hWnd, &winRect);


		float viewWidth = winRect.right - winRect.left;
		float viewHeight = winRect.bottom - winRect.top;

		float leftLimit = -viewWidth / 2.0f + someOffset;  // someOffset�� ���ϴ� �Ѱ谪�� ���� ����
		float rightLimit = viewWidth / 2.0f - someOffset;
		float topLimit = viewHeight / 2.0f - someOffset;
		float bottomLimit = -viewHeight / 2.0f + someOffset;

		Vector3 cameraPos = Cam_tr->GetPosition();

		// x ��ǥ ����
		if (cameraPos.x < leftLimit) cameraPos.x = leftLimit;
		if (cameraPos.x > rightLimit) cameraPos.x = rightLimit;

		// y ��ǥ ����
		if (cameraPos.y < bottomLimit) cameraPos.y = bottomLimit;
		if (cameraPos.y > topLimit) cameraPos.y = topLimit;

		Cam_tr->SetPosition(cameraPos);



	// ======================================================	
			
		//Vector3 pos = Cam_tr->GetPosition();
		//
		//if (Input::GetKey(eKeyCode::W))
		//{
		//	pos.y += 500.0f * Time::DeltaTime();
		//	Cam_tr->SetPosition(pos);
		//}
		//else if (Input::GetKey(eKeyCode::S))
		//{
		//	pos.y -= 500.0f * Time::DeltaTime();
		//	Cam_tr->SetPosition(pos);
		//}
		//else if (Input::GetKey(eKeyCode::A))
		//{
		//	pos.x -= 500.0f * Time::DeltaTime();
		//	Cam_tr->SetPosition(pos);
		//}
		//else if (Input::GetKey(eKeyCode::D))
		//{
		//	pos.x += 500.0f * Time::DeltaTime();
		//	Cam_tr->SetPosition(pos);
		//}
		//else if (Input::GetKey(eKeyCode::Q))
		//{
		//	pos.z -= 500.0f * Time::DeltaTime();
		//	Cam_tr->SetPosition(pos);
		//}
		//else if (Input::GetKey(eKeyCode::E))
		//{
		//	pos.z += 500.0f * Time::DeltaTime();
		//	Cam_tr->SetPosition(pos);
		//}

	}
}
