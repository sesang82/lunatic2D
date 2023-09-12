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
		Transform* Playertr = mPlayer->GetComponent<Transform>();

		Transform* Layertr = GetOwner()->GetComponent<Transform>();

		//// mPlayer�� ����ִ� �ڵ� ���� AddComponent�� �ߴ��� �ش� ��ũ��Ʈ�� Initalize�� ���� ���ƹ�����
		//// mPlayer�� nullptr�� ���·� ���� ���� initalize�� �ƴ� update���� ���� 

		if (mFirst) // �÷��̾��� ó�� ��ġ 
		{
			mPlayerPrePos = renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition();
			mFirst = true; 
		}
		
		
		// �÷��̾��� �ǽð� ��ġ 
	//	mPlayerCurPos = Playertr->GetPosition();
		

		mPlayerCurPos = renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition();


		// ������ ���
		//Vector3 characterMovement = mPlayerCurPos - mPlayerPrePos;


		if (GetOwner()->GetName() == L"BG_Moon"
			|| GetOwner()->GetName() == L"BG_Tree1"
			|| GetOwner()->GetName() == L"BG_Tree2"
			|| GetOwner()->GetName() == L"BG_Tree3"
			|| GetOwner()->GetName() == L"BG_Tree4")
		{

			// ��� ���̾� �� ��ġ ��� (��� ���̾�� �ӵ��� 
			// (���� �� ����� �ִ� 1��, ���� ������ �ּ� 0�� ������)) 
			Vector3 LayerNewPos = Layertr->GetPosition();

			LayerNewPos.x += (mPlayerCurPos.x - mPlayerPrePos.x) * mSpeed.x;
			LayerNewPos.y += (mPlayerCurPos.y - mPlayerPrePos.y) * mSpeed.y;

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
		mPlayerPrePos = mPlayerCurPos;

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
