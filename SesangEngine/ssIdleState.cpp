#include "ssIdleState.h"
#include "ssMonster.h"
#include "ssMonsterScript.h"
#include "ssScene.h"
#include "ssSceneManager.h"
#include "ssPlayer.h"
#include "ssTransform.h"

namespace ss
{
	IdleState::IdleState()
	{
	}
	IdleState::~IdleState()
	{
	}
	void IdleState::LateUpdate()
	{
		// 상태 소유 오브젝트가 몬스터 종류가 아닌 경우
		Monster* pMon = dynamic_cast<Monster*>(GetOwnerObj());
		assert(pMon);


		// Player 를 알아낸다.
		Scene* pCurScene = SceneManager::GetActiveScene();
		Player* pPlayer = pCurScene->FindObjInActiveLayer<Player>(eLayerType::Player);

		// 몬스터스크립트를 가진 모든 자식 스크립트들에게도 해당되도록 부모로 갖고옴
		MonsterScript* scriptMon = pMon->GetComponent<MonsterScript>();
		Transform* trMon = pMon->GetComponent<Transform>();


		Transform* trPlayer = pPlayer->GetComponent<Transform>();

		assert(pPlayer);

		// Player 와 Monster 의 거리값을 계산
		float fDetectRange = scriptMon->GetMonsterInfo().m_fDetectRange;

		// Player 가 탐지범위 이내에 들어오면 추적상태로 전환
		Vector3 vDir = trMon->GetPosition() - trPlayer->GetPosition();
		if (vDir.Length() < fDetectRange)
		{
			ChangeState(L"Trace");
		}
	
	
	
	}
	void IdleState::Enter()
	{
	}
	void IdleState::Exit()
	{
	}
}