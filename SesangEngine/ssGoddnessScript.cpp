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
		tr->SetScale(269.f, 276.f, 1.f);



		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Boss2-1_Idle");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Boss2-1_Stomp");
		std::shared_ptr<ss::graphics::Texture> Image3 = Resources::Find<ss::graphics::Texture>(L"Boss2-1_EnergyballStart");
		std::shared_ptr<ss::graphics::Texture> Image4 = Resources::Find<ss::graphics::Texture>(L"Boss2-1_EnergyballEnd");
		std::shared_ptr<ss::graphics::Texture> Image5 = Resources::Find<ss::graphics::Texture>(L"Boss2-1_Die");


		std::shared_ptr<ss::graphics::Texture> Image6 = Resources::Find<ss::graphics::Texture>(L"Boss2_2_MoveBack");
	
		// ==== 1������ ���� 
		/*mAnimator->Create(L"Boss_Goddness_Idle", Image1, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 2, Vector2(269.f, 308.f));
		mAnimator->Create(L"Boss_Goddness_Stomp", Image2, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 4, Vector2(269.f, 308.f));
		mAnimator->Create(L"Boss_Goddness_EnergyballStart", Image3, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 11, Vector2(269.f, 308.f));
		mAnimator->Create(L"Boss_Goddness_EnergyballEnd", Image4, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 4, Vector2(269.f, 308.f));
		mAnimator->Create(L"Boss_Goddness_Die", Image5, Vector2(0.f, 0.f), Vector2(269.f, 308.f), 22, Vector2(269.f, 308.f));*/


		// ==== 2������ ��
		mAnimator->Create(L"test", Image6, Vector2(0.f, 0.f), Vector2(269.f, 276.f), 6, Vector2(269.f, 276.f), Vector2::Zero);

		mAnimator->PlayAnimation(L"test", true);

		// ======
	// �ִϸ��̼� ���⿡ ���� ����	�� ����ش�.


		mDir = -mTransform->Right();



		// ������ �ʱ� ���´� �ο������� �ʴ´�. (������) 


		// ===== �⺻ �浹ü Hit �޴� �뵵 
		mCollider->SetName(L"Goddness_HitCol");
		mCollider->SetType(eColliderType::Rect);
		mCollider->SetSize(Vector2(0.6f, 0.36f));
		mCollider->SetCenter(Vector2(-4.f, -84.f));

		// ===== ������ٵ�



		////==== ���� ���� Ư�� �ε��� �浹ü 
		////�浹ü�� ���⼭ �ٷ� ���� ���� �ش� �ε��� �� �־��ٰ� ���� ������ �ϱ� 
		//mAttackColliderObj = object::Instantiate<AttackCollider>(eLayerType::Collision, L"BigWolf_AttackColObj");
		//mAttackColliderObj->Initialize();

		//mAttackColTr = mAttackColliderObj->GetComponent<Transform>();


		// trigger �ϼ��ϸ� �̰� ����� 
		//mMeshRenderer->SetMaterial(Resources::Find<Material>(L"tempMtrl"));

	}
	void GoddnessScript::Update()
	{
		MonsterScript::Update();

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