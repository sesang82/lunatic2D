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
		mMapSize = Vector2(800.0f, 448.f); // 전체 맵의 절반 크기 
		someOffset = 75.f; //  카메라가 화면의 상단 또는 하단에 가까이 다가가지 않게 하기 위한 값
	}


	void CameraScript::Update()
	{

		// 카메라의 트랜스폼 얻어옴
		Transform* Cam_tr = GetOwner()->GetComponent<Transform>();

		//// 카메라가 비추는 영역의 가로, 세로 크기의 절반을 구한다. 

		//mCameraHeight = renderer::mainCamera->GetSize();

		//mCameraWidth = mCameraHeight * 1600 / 896;



		// 플레이어의 트랜스폼 얻어옴
		if (mTarget)
		{
			Transform* Player_tr = mTarget->GetComponent<Transform>();
			Vector3 PlayerPos = Player_tr->GetPosition();
			Cam_tr->SetPosition(Vector3(PlayerPos.x, PlayerPos.y + 75.f, 990.f));
			// x에다가 +로 더해주면 처음에 시작할 때 해당 카메라가 그 위치에서 시작하긴 함 



			//Vector2 Center = Vector2(Vector2::Zero); // 카메라를 0,0에 뒀으므로 이렇게 씀


			//float lx = mMapSize.x - mCameraWidth; // 카메라가 가로로 이동할 수 있는 영역
			//float clampX = std::clamp(PlayerPos.x, Center.x - lx, Center.x + lx);

			//float ly = mMapSize.y - mCameraWidth; // 카메라가 세로로 이동할 수 있는 영역
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

		float leftLimit = -viewWidth / 2.0f + someOffset;  // someOffset은 원하는 한계값에 따라 조절
		float rightLimit = viewWidth / 2.0f - someOffset;
		float topLimit = viewHeight / 2.0f - someOffset;
		float bottomLimit = -viewHeight / 2.0f + someOffset;

		Vector3 cameraPos = Cam_tr->GetPosition();

		// x 좌표 제한
		if (cameraPos.x < leftLimit) cameraPos.x = leftLimit;
		if (cameraPos.x > rightLimit) cameraPos.x = rightLimit;

		// y 좌표 제한
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
