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
		// ���� ���� ������Ʈ�� ���� ������ �ƴ� ���
		Monster* pMon = dynamic_cast<Monster*>(GetOwnerObj());
		assert(pMon);


		// Player �� �˾Ƴ���.
		Scene* pCurScene = SceneManager::GetActiveScene();
		Player* pPlayer = pCurScene->FindObjInActiveLayer<Player>(eLayerType::Player);

		// ���ͽ�ũ��Ʈ�� ���� ��� �ڽ� ��ũ��Ʈ�鿡�Ե� �ش�ǵ��� �θ�� �����
		MonsterScript* scriptMon = pMon->GetComponent<MonsterScript>();
		Transform* trMon = pMon->GetComponent<Transform>();


		Transform* trPlayer = pPlayer->GetComponent<Transform>();

		assert(pPlayer);

		// Player �� Monster �� �Ÿ����� ���
		float fDetectRange = scriptMon->GetMonsterInfo().m_fDetectRange;

		// Player �� Ž������ �̳��� ������ �������·� ��ȯ
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