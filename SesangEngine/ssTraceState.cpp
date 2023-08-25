#include "ssTraceState.h"
#include "ssMonster.h"
#include "ssMonsterScript.h"
#include "ssScene.h"
#include "ssSceneManager.h"
#include "ssPlayer.h"
#include "ssTransform.h"
#include "ssTime.h"

namespace ss
{
	TraceState::TraceState()
	{
	}
	TraceState::~TraceState()
	{
	}
	void TraceState::LateUpdate()
	{
		// ���� ���� ������Ʈ�� ���� ������ �ƴ� ���
		Monster* pMon = (Monster*)GetOwnerObj();
		assert(pMon);

		MonsterScript* scriptMon = pMon->GetComponent<MonsterScript>();
		Transform* trMon = pMon->GetComponent<Transform>();

		// Player �� �˾Ƴ���.
		Scene* pCurScene = SceneManager::GetActiveScene();
		Player* pPlayer = pCurScene->FindObjInActiveLayer<Player>(eLayerType::Player);

		Transform* trPlayer = pPlayer->GetComponent<Transform>();



		Vector3 vMonPos = trMon->GetPosition();
		Vector3 vPlayerPos = trPlayer->GetPosition();

		Vector3 vMonToPlayer = vPlayerPos - vMonPos;
		vMonToPlayer.Normalize();

		float fSpeed = scriptMon->GetMonsterInfo().m_fSpeed;

		vMonPos += vMonToPlayer * Time::DeltaTime() * fSpeed;

		trMon->SetPosition(vMonPos);
	}
	void TraceState::Enter()
	{
	}
	void TraceState::Exit()
	{
	}
}
