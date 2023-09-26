#include "ssParalloxScript.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssGameObject.h"
#include "ssAnimator.h"
#include "ssMonster.h"
#include "ssSceneManager.h"
#include "ssBackground.h"
#include "ssPlayer.h"
#define NOMINMAX
#include <windows.h>
#include <algorithm>
#include "ssRenderer.h"
#include "ssCameraScript.h"





namespace ss
{
	ParalloxScript::ParalloxScript()
		: mFirst(false)
		, mSpeed(Vector2::Zero)
		, mTextureOffset(Vector2::Zero)
		, mMinX(0.f)
		, mMaxX(0.f)
		, mMinY(0.f)
		, mMaxY(0.f)

	{		
	}

	ParalloxScript::~ParalloxScript()
	{
	}

	void ParalloxScript::Initialize()
	{
		MeshRenderer* mr = GetOwner()->GetComponent<MeshRenderer>();
		Transform* tr = GetOwner()->GetComponent<Transform>();




	}

	void ParalloxScript::Update()
	{
		Transform* Cameratr = renderer::mainCamera->GetOwner()->GetComponent<Transform>();

		Transform* Layertr = GetOwner()->GetComponent<Transform>();

		//// mPlayer를 담아주는 코드 전에 AddComponent를 했더니 해당 스크립트의 Initalize를 먼저 돌아버려서
		//// mPlayer이 nullptr인 상태로 오기 땜에 initalize가 아닌 update에서 해줌 

		if (mFirst) // 플레이어의 처음 위치 
		{
			mCameraPrePos = Cameratr->GetPosition();
			mFirst = true; 
		}
		
		
		// 플레이어의 실시간 위치 
		mCameraCurPos = Cameratr->GetPosition();
		


		// 움직임 계산
		Vector3 characterMovement = mCameraCurPos - mCameraPrePos;


		if (GetOwner()->GetName() == L"BG_Moon"
			|| GetOwner()->GetName() == L"BG_Tree1"
			|| GetOwner()->GetName() == L"BG_Tree2"
			|| GetOwner()->GetName() == L"BG_Tree3"
			|| GetOwner()->GetName() == L"BG_Tree4"
			|| GetOwner()->GetName() == L"BG_BackGround_0"
			|| GetOwner()->GetName() == L"BG_BackGround_1"
			|| GetOwner()->GetName() == L"BG_BackGround_2"
			|| GetOwner()->GetName() == L"BG_BackGround_3"
			|| GetOwner()->GetName() == L"BG_BackGround_4")
		{

			// 배경 레이어 새 위치 계산 (배경 레이어들 속도는 
			// (가장 먼 배경은 최대 1로, 가장 가까운건 최소 0에 가깝게)) 
			Vector3 LayerNewPos = Layertr->GetPosition();

			LayerNewPos.x += (mCameraCurPos.x - mCameraPrePos.x) * mSpeed.x;
			LayerNewPos.y += (mCameraCurPos.y - mCameraPrePos.y) * mSpeed.y;

			// 새 위치가 경계 값을 넘어가지 않도록 체크
			LayerNewPos.x = (std::max)(mMinX, (std::min)(mMaxX, LayerNewPos.x));
			LayerNewPos.y = (std::max)(mMinY, (std::min)(mMaxY, LayerNewPos.y));
	
			Layertr->SetPosition(LayerNewPos);



			
		}

		//// 배경 레이어들의 속도 설정
		//float backgroundLayer1Speed = 0.5f; // 가장 먼 배경
		//float backgroundLayer2Speed = 0.7f;
		//float backgroundLayer3Speed = 1.0f; // 가장 가까운 배경



		// 실시간 위치 구한 후, 이전 위치 실시간 갱신 
		mCameraPrePos = mCameraCurPos;

	}

	void ParalloxScript::OnCollisionEnter(Collider2D* other)
	{
	}

	void ParalloxScript::OnCollisionStay(Collider2D* other)
	{
	}

	void ParalloxScript::OnCollisionExit(Collider2D* other)
	{
	}
}
