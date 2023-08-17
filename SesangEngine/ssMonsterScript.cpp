#include "ssMonsterScript.h"
#include "ssTime.h"
#include "ssFSM.h"
#include "ssGameObject.h"
#include "ssRigidbody2D.h"
#include "ssScene.h"
#include "ssSceneManager.h"
#include "ssPlayer.h"
#include "ssMonster.h"


namespace ss
{
	MonsterScript::MonsterScript()
		: m_tMonsterInfo{}
		, mFSM(nullptr)
		, mAnimator(nullptr)
		, mRigidbody(nullptr)
		, mTransform(nullptr)
		, mCollider(nullptr)
		, mMeshRenderer(nullptr)
		, mCharacterState(nullptr)
		, mPlayer(nullptr)
		, mOringinColSize(Vector2::Zero)
		, mOringinColCeter(Vector2::Zero)
	{
		

		//// AI 설정
		//GetAI()->AddState(L"Idle", new CIdleState);
		//GetAI()->AddState(L"Trace", new CTraceState);

		//GetAI()->ChangeState(L"Idle");

	}
	MonsterScript::~MonsterScript()
	{
		if (nullptr != mFSM)
			delete mFSM;
	}
	void MonsterScript::Initialize()
	{

		Scene* pCurScene = SceneManager::GetActiveScene();

		if (pCurScene->FindObjInActiveLayer<Player>())
		{
			mPlayer = pCurScene->FindObjInActiveLayer<Player>();
			assert(mPlayer);
		}

	/*	mMonster = dynamic_cast<Monster*>(GetOwner());
		assert(mMonster);*/

		// 일단 만들어놨는데 활용은 안할 계획 
		mFSM = new FSM(this);


		if (nullptr != GetOwner()->GetComponent<CharacterState>())
		{
			mCharacterState = GetOwner()->GetComponent<CharacterState>();
			assert(mCharacterState);
		}

	}
	void MonsterScript::Update()
	{

		/*if (IsValid(m_pTarget))
{
	Vector3 vTargetPos = m_pTarget->GetPosition();
	m_pTarget->SetName(L"Player");
	Vector3 vPos = GetPosition();

	Vec3 vDir = vTargetPos - vPos;
	vDir.Normalize();

	vPos.x += vDir.x * 100.f * DT;
	vPos.y += vDir.y * 100.f * DT;

	SetPosition(vPos);
}*/
	}

	// 충돌체 작용 내용은 상속 되지 않음. 따로따로 적어줘야됨 
	void MonsterScript::OnCollisionEnter(Collider2D* other)
	{




	}
	void MonsterScript::OnCollisionStay(Collider2D* other)
	{
	}
	void MonsterScript::OnCollisionExit(Collider2D* other)
	{
	}
}