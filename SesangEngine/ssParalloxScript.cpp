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





namespace ss
{
	ParalloxScript::ParalloxScript()
		: mFirst(false)
		, mSpeed(false)
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

		
		// ��渶�� ũ�Ⱑ �ٸ��Ƿ� �Ʒ�ó�� ���ش�.
		if (GetOwner()->GetName() == L"HowlingEffectObj")
		{
			tr->SetScale(Vector3(583.f, 123.f, 0.f));

		}
	}

	void ParalloxScript::Update()
	{
		Transform* Playertr = mPlayer->GetComponent<Transform>();

		Transform* Layertr = GetOwner()->GetComponent<Transform>();

		// mPlayer�� ����ִ� �ڵ� ���� AddComponent�� �ߴ��� �ش� ��ũ��Ʈ�� Initalize�� ���� ���ƹ�����
		// mPlayer�� nullptr�� ���·� ���� ���� initalize�� �ƴ� update���� ���� 

		if (mFirst) // �÷��̾��� ó�� ��ġ 
		{
			mPlayerPrePos = Playertr->GetPosition();
			mFirst = true; 
		}
		
		
		// �÷��̾��� �ǽð� ��ġ 
		mPlayerCurPos = Playertr->GetPosition();
		
		// ������ ���
		Vector3 characterMovement = mPlayerCurPos - mPlayerPrePos;


		if (GetOwner()->GetName() == L"BG_Moon"
			|| GetOwner()->GetName() == L"BG_Tree1"
			|| GetOwner()->GetName() == L"BG_Tree2"
			|| GetOwner()->GetName() == L"BG_Tree3"
			|| GetOwner()->GetName() == L"BG_Tree4")
		{

			// ��� ���̾� �� ��ġ ��� (��� ���̾�� �ӵ��� 
			// (���� �� ����� �ִ� 1��, ���� ������ �ּ� 0�� ������)) 
			Vector3 LayerNewPos = Layertr->GetPosition() + (characterMovement * mSpeed);


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
