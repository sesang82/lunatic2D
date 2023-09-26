#include "ssGoddnessScript.h"
#include "ssResources.h"
#include "ssTexture.h"
#include "ssAnimator.h"
#include "ssCharacterState.h"
#include "ssGameObject.h"
#include "ssPlayer.h"
#include "ssRigidbody2D.h"
#include "ssTime.h"
#include "ssObject.h"
#include "ssRangeCollider.h"
#include "ssPlayerScript.h"
#include "ssAttackCollider.h"
#include "ssEffect.h"
#include "ssMeshRenderer.h"
#include "ssHitGroundScript.h"
#include "ssMonster.h"
#include "ssRenderer.h"
#include "ssGameState.h"
#include "ssEffectScript.h"

namespace ss
{
	GoddnessScript::GoddnessScript()
	{
		m_tMonsterInfo.m_fSpeed = 200.f;
		m_tMonsterInfo.m_fAttack = 10.f;
		m_tMonsterInfo.m_fDetectRange = 300.f;
	}
	GoddnessScript::~GoddnessScript()
	{
	}
	void GoddnessScript::Initialize()
	{
		MonsterScript::Initialize();




		mCharacterState->SetMaxHP(110.f);
		mCharacterState->SetCurrentHP(110.f);

		mMeshRenderer->SetMaterial(Resources::Find<Material>(L"BossAnimMtrl"));


		Transform* tr = GetOwner()->GetComponent<Transform>();
		tr->SetScale(272.f, 271.f, 1.f);



		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_Idle");

		mAnimator->Create(L"Boss_Wolf_IdleR", Image1, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 9, Vector2(272.f, 271.f));



		// ======
	// 애니메이션 방향에 관한 기준	을 잡아준다.


		mDir = -mTransform->Right();



		// 몬스터의 초기 상태는 부여해주지 않는다. (보스만) 


		// ===== 기본 충돌체 Hit 받는 용도 
		mCollider->SetName(L"Goddness_HitCol");
		mCollider->SetType(eColliderType::Rect);
		mCollider->SetSize(Vector2(0.6f, 0.36f));
		mCollider->SetCenter(Vector2(-4.f, -84.f));

		// ===== 리지드바디



		////==== 근접 공격 특정 인덱스 충돌체 
		////충돌체는 여기서 바로 넣지 말고 해당 인덱스 때 넣었다가 빼는 식으로 하기 
		//mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"BigWolf_AttackColObj");
		//mAttackColliderObj->Initialize();

		//mAttackColTr = mAttackColliderObj->GetComponent<Transform>();


		mMeshRenderer->SetMaterial(Resources::Find<Material>(L"tempMtrl"));

	}
	void GoddnessScript::Update()
	{
	}
	void GoddnessScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void GoddnessScript::OnCollisionStay(Collider2D* other)
	{
	}
	void GoddnessScript::OnCollisionExit(Collider2D* other)
	{
	}
	void GoddnessScript::Spawn()
	{
	}
	void GoddnessScript::Idle()
	{
	}
	void GoddnessScript::Hit()
	{
	}
	void GoddnessScript::Stomp()
	{
	}
	void GoddnessScript::Energyball_Start()
	{
	}
	void GoddnessScript::Energyball_End()
	{
	}
	void GoddnessScript::Dead()
	{
	}
	void GoddnessScript::Animation()
	{
	}
}