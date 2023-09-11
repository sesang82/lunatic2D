#include "ssParalloxScript.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssGameObject.h"
#include "ssAnimator.h"
#include "ssMonster.h"
#include "ssSceneManager.h"
#include "ssBackground.h"
#include "ssPlayer.h"



namespace ss
{
	ParalloxScript::ParalloxScript()
		: mFirst(false)
	{		
	}

	ParalloxScript::~ParalloxScript()
	{
	}

	void ParalloxScript::Initialize()
	{
		MeshRenderer* mr = GetOwner()->GetComponent<MeshRenderer>();
		Transform* tr = GetOwner()->GetComponent<Transform>();

		
		// 배경마다 크기가 다르므로 아래처럼 해준다.
		if (GetOwner()->GetName() == L"HowlingEffectObj")
		{
			tr->SetScale(Vector3(583.f, 123.f, 0.f));

		}
	}

	void ParalloxScript::Update()
	{
		Transform* Playertr = mPlayer->GetComponent<Transform>();

		// mPlayer를 담아주는 코드 전에 AddComponent를 했더니 해당 스크립트의 Initalize를 먼저 돌아버려서
		// mPlayer이 nullptr인 상태로 오기 땜에 initalize가 아닌 update에서 해줌 

		if (mFirst)
		{
			mPlayerPrePos = Playertr->GetPosition();
			mFirst = true; 
		}
		
		
		
		mPlayerCurPos = Playertr->GetPosition();

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
