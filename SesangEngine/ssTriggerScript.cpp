#include "ssTriggerScript.h"
#include "ssMonster.h"
#include "ssBigWolfScript.h"
#include "ssResources.h"
#include "ssMeshRenderer.h"
#include "ssAnimator.h"
#include "ssPlatform.h"
#include "ssCamera.h"
#include "ssCameraScript.h"
#include "ssobject.h"


namespace ss
{
	TriggerScript::TriggerScript()
	{
	}
	TriggerScript::~TriggerScript()
	{
	}
	void TriggerScript::Initialize()
	{
	}
	void TriggerScript::Update()
	{
	}
	void TriggerScript::OnCollisionEnter(Collider2D* other)
	{

		// Ÿ���� ���ͷ� �ű��.
		if (other->GetName() == L"colHit_player")
		{
			CameraScript* script = mCamera->GetComponent<CameraScript>();
			script->SetTarget(mMonster);

			Camera* camera = mCamera->GetComponent<Camera>();
			camera->SetTargetSize(3.5f);

			// ���͹ڽ��� �����.
			mLetterBoxUP = object::Instantiate<GameObject>(eLayerType::Etc, L"letterbox_UP");
		
			Transform* tr = mLetterBoxUP->GetComponent<Transform>();
			tr->SetPosition(Vector3(0.f, -405.f, 100.f));
			tr->SetScale(Vector3(1600.f, 896.f, 1.f));

			MeshRenderer* mr = mLetterBoxUP->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"LetterBoxUpMtrl"));



			mLetterBoxBottom = object::Instantiate<GameObject>(eLayerType::Etc, L"letterbox_Bottom");

			Transform* TR = mLetterBoxBottom->GetComponent<Transform>();
			TR->SetPosition(Vector3(0.f, -130.f, 100.f));
			TR->SetScale(Vector3(1600.f, 896.f, 1.f));

			MeshRenderer* MR = mLetterBoxBottom->AddComponent<MeshRenderer>();
			MR->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			MR->SetMaterial(Resources::Find<Material>(L"LetterBoxBottomMtrl"));


			// ���͹ڽ��� ��� �۱͸� �����.
			mBossName = object::Instantiate<GameObject>(eLayerType::Etc, L"Boss1Name");

			Transform* nameTR = mBossName->GetComponent<Transform>();
			nameTR->SetPosition(Vector3(0.f, -350.f, 100.f));
			nameTR->SetScale(Vector3(74.f, 48.f, 1.f));

			MeshRenderer* nameMR = mBossName->AddComponent<MeshRenderer>();
			nameMR->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			nameMR->SetMaterial(Resources::Find<Material>(L"Boss1NameMtrl"));


			// UIī�޶� (��� UI�� �ȳ����� ����) 
			Camera* UIcamera = mUICamera->GetComponent<Camera>();
			UIcamera->TurnLayerMask(eLayerType::UI, false);


			// ���� ĳ������ ���� ���� �ͼ� �����Ѵ�. 
			MeshRenderer* Bossmr = mMonster->GetComponent<MeshRenderer>();
			Bossmr->SetMaterial(Resources::Find<Material>(L"SpriteAnimationMaterial"));

			Animator* animator = mMonster->GetComponent<Animator>();
			animator->PlayAnimation(L"Boss_Wolf_SpawnL", false);

		
		
		}
	}

	void TriggerScript::OnCollisionStay(Collider2D* other)
	{
		if (other->GetName() == L"colHit_player")
		{
			Animator* animator = mMonster->GetComponent<Animator>();

			if (animator->GetCurActiveAnimation()->IsComplete())
			{
				int i = 0;
				BigWolfScript* script = mMonster->GetComponent<BigWolfScript>();
				script->ChangeState(eWolfBossState::IDLE);
			}
		}




	}
	void TriggerScript::OnCollisionExit(Collider2D* other)
	{
	
		GetOwner()->SetState(GameObject::eState::Dead);


	}
}