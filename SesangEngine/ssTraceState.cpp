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
		// 상태 소유 오브젝트가 몬스터 종류가 아닌 경우
		Monster* pMon = (Monster*)GetOwnerObj();
		assert(pMon);

		MonsterScript* scriptMon = pMon->GetComponent<MonsterScript>();
		Transform* trMon = pMon->GetComponent<Transform>();

		// Player 를 알아낸다.
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
