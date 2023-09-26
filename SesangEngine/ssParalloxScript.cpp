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

		//// mPlayer�� ����ִ� �ڵ� ���� AddComponent�� �ߴ��� �ش� ��ũ��Ʈ�� Initalize�� ���� ���ƹ�����
		//// mPlayer�� nullptr�� ���·� ���� ���� initalize�� �ƴ� update���� ���� 

		if (mFirst) // �÷��̾��� ó�� ��ġ 
		{
			mCameraPrePos = Cameratr->GetPosition();
			mFirst = true; 
		}
		
		
		// �÷��̾��� �ǽð� ��ġ 
		mCameraCurPos = Cameratr->GetPosition();
		


		// ������ ���
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

			// ��� ���̾� �� ��ġ ��� (��� ���̾�� �ӵ��� 
			// (���� �� ����� �ִ� 1��, ���� ������ �ּ� 0�� ������)) 
			Vector3 LayerNewPos = Layertr->GetPosition();

			LayerNewPos.x += (mCameraCurPos.x - mCameraPrePos.x) * mSpeed.x;
			LayerNewPos.y += (mCameraCurPos.y - mCameraPrePos.y) * mSpeed.y;

			// �� ��ġ�� ��� ���� �Ѿ�� �ʵ��� üũ
			LayerNewPos.x = (std::max)(mMinX, (std::min)(mMaxX, LayerNewPos.x));
			LayerNewPos.y = (std::max)(mMinY, (std::min)(mMaxY, LayerNewPos.y));
	
			Layertr->SetPosition(LayerNewPos);



			
		}

		//// ��� ���̾���� �ӵ� ����
		//float backgroundLayer1Speed = 0.5f; // ���� �� ���
		//float backgroundLayer2Speed = 0.7f;
		//float backgroundLayer3Speed = 1.0f; // ���� ����� ���



		// �ǽð� ��ġ ���� ��, ���� ��ġ �ǽð� ���� 
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
