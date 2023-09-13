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
#include "ssPlayer.h"
#include "ssBackground.h"

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
			mLetterBoxUP = object::Instantiate<Background>(eLayerType::Grid, L"letterbox_UP");
			mLetterBoxUP->Initialize();

			Transform* tr = mLetterBoxUP->GetComponent<Transform>();
			tr->SetPosition(Vector3(0.f, -405.f, 100.f));
			tr->SetScale(Vector3(1600.f, 896.f, 1.f));

			MeshRenderer* mr = mLetterBoxUP->GetComponent<MeshRenderer>();
			mr->SetMaterial(Resources::Find<Material>(L"LetterBoxUpMtrl"));



			mLetterBoxBottom = object::Instantiate<Background>(eLayerType::Grid, L"letterbox_Bottom");
			mLetterBoxBottom->Initialize();

			Transform* TR = mLetterBoxBottom->GetComponent<Transform>();
			TR->SetPosition(Vector3(0.f, -130.f, 100.f));
			TR->SetScale(Vector3(1600.f, 896.f, 1.f));

			MeshRenderer* MR = mLetterBoxBottom->GetComponent<MeshRenderer>();
			MR->SetMaterial(Resources::Find<Material>(L"LetterBoxBottomMtrl"));


			// ���͹ڽ��� ��� �۱͸� �����.
			mBossName = object::Instantiate<Background>(eLayerType::Grid, L"Boss1Name");
			mBossName->Initialize();

			Transform* nameTR = mBossName->GetComponent<Transform>();
			nameTR->SetPosition(Vector3(0.f, -350.f, 100.f));
			nameTR->SetScale(Vector3(74.f, 48.f, 1.f));

			MeshRenderer* nameMR = mBossName->GetComponent<MeshRenderer>();
			nameMR->SetMaterial(Resources::Find<Material>(L"Boss1NameMtrl"));


			// UIī�޶� (��� UI�� �ȳ����� ����) 
			Camera* UIcamera = mUICamera->GetComponent<Camera>();
			UIcamera->TurnLayerMask(eLayerType::UI, false);


			// ���� ĳ������ ���� ���� �ͼ� �����Ѵ�. 
			MeshRenderer* Bossmr = mMonster->GetComponent<MeshRenderer>();
			Bossmr->SetMaterial(Resources::Find<Material>(L"BossAnimMtrl"));

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

		Camera* UIcamera = mUICamera->GetComponent<Camera>();
		UIcamera->TurnLayerMask(eLayerType::UI);
	

		CameraScript* script = mCamera->GetComponent<CameraScript>();
		script->SetTarget(mPlayer);

		Camera* camera = mCamera->GetComponent<Camera>();
		camera->SetTargetSize(2.3f);
		camera->TurnLayerMask(eLayerType::Grid, false); // deadó���� �Ǵ°� �´µ� �ݿ��� �ȵǼ� grid�� �������� �Ⱥ��ߴ°ɷ� ��ȸ��

		GetOwner()->SetState(GameObject::eState::Dead);

		mLetterBoxUP->SetState(GameObject::eState::Dead);
		mLetterBoxBottom->SetState(GameObject::eState::Dead);
		mBossName->SetState(GameObject::eState::Dead);

	}
}