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
		mbFllowMonster = false;
		mbReturnToPlayer = false;

		// 카메라의 트랜스폼 얻어옴
		Camera* cameraComp = GetOwner()->GetComponent<Camera>();

		// else if를 하면 if문과 else if문 중 하나만 실행되기 때문에 if문으로 하기 
		if (SceneManager::GetSceneByName(L"LibraryScene"))
		{
			cameraOffset.leftOffset = 785.0f;
			cameraOffset.rightOffset = 785.0f; // 숫자가 높을수록 오른쪽 제한 값이 왼쪽 안으로 들어옴 
			someOffset = 75.f; //  카메라가 화면의 상단 또는 하단에 가까이 다가가지 않게 하기 위한 값
			mPosOffset = 100.f;
		}

		else if (SceneManager::GetSceneByName(L"EntranceScene"))
		{
			cameraOffset.leftOffset = 545.0f;			
			cameraOffset.rightOffset = 584.0f; // 숫자가 높을수록 오른쪽 제한 값이 왼쪽 안으로 들어옴 
			someOffset = 660.f; //  카메라가 화면의 상단 또는 하단에 가까이 다가가지 않게 하기 위한 값
			mPosOffset = 80.f;
		}


		if (SceneManager::GetSceneByName(L"EntryScene"))
		{
			cameraOffset.leftOffset = 668.0f;
			cameraOffset.rightOffset = 668.0f; // 숫자가 높을수록 오른쪽 제한 값이 왼쪽 안으로 들어옴 
			someOffset = 30.f; //  카메라가 화면의 상단 또는 하단에 가까이 다가가지 않게 하기 위한 값
			mPosOffset = 100.f;
		}


		if (SceneManager::GetSceneByName(L"Stage1Scene"))
		{
			cameraOffset.leftOffset = 670.0f;
			cameraOffset.rightOffset = 840.0f; // 숫자가 높을수록 오른쪽 제한 값이 왼쪽 안으로 들어옴 
			someOffset = 180.f; //  카메라가 화면의 상단 또는 하단에 가까이 다가가지 않게 하기 위한 값
			mPosOffset = 0.f;
		}


		else if (SceneManager::GetSceneByName(L"Boss1Scene"))
		{
			cameraOffset.leftOffset = 668.0f;
			cameraOffset.rightOffset = 668.0f; // 숫자가 높을수록 오른쪽 제한 값이 왼쪽 안으로 들어옴 
			mPosOffset = 100.f;
		}

		 float what = cameraOffset.leftOffset;
		 float what2 = cameraOffset.rightOffset;

	
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
			Transform* Target_tr = mTarget->GetComponent<Transform>();
			Vector3 TargetPos = Target_tr->GetPosition();

			// x에다가 +로 더해주면 처음에 시작할 때 해당 카메라가 그 위치에서 시작하긴 함 



			//Vector2 Center = Vector2(Vector2::Zero); // 카메라를 0,0에 뒀으므로 이렇게 씀


			//float lx = mMapSize.x - mCameraWidth; // 카메라가 가로로 이동할 수 있는 영역
			//float clampX = std::clamp(PlayerPos.x, Center.x - lx, Center.x + lx);

			//float ly = mMapSize.y - mCameraWidth; // 카메라가 세로로 이동할 수 있는 영역
			//float clampY = std::clamp(PlayerPos.y, Center.y - ly, Center.y + ly);
			//
			//Cam_tr->SetPosition(Vector3(clampX, clampY + 75.f, -10.f));

			// Lerp를 사용하여 부드럽게 타겟에게 접근

			if (mTarget->GetName() == L"Player")
			{
				Vector3 PlayerPos = Vector3::Lerp(Cam_tr->GetPosition(), Vector3(TargetPos.x, TargetPos.y + mPosOffset, 990.f), 0.003f);
				Cam_tr->SetPosition(PlayerPos);
			}

			else
			{
				Vector3 newPos = Vector3::Lerp(Cam_tr->GetPosition(), Vector3(TargetPos.x, TargetPos.y -90.f, 990.f), 0.004f);
				Cam_tr->SetPosition(newPos);
			}

				
		}
	
		else
		{	
			Cam_tr->SetPosition(Vector3(0.f, -200.f, -10.f));
		}


		HWND hWnd = application.GetHwnd();
		RECT winRect = {};
		GetClientRect(hWnd, &winRect);


		float viewWidth = winRect.right - winRect.left;
		float viewHeight = winRect.bottom - winRect.top;

		float leftLimit = -viewWidth / 2.0f + cameraOffset.leftOffset;  // someOffset은 원하는 한계값에 따라 조절
		float rightLimit = viewWidth / 2.0f - cameraOffset.rightOffset;
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
