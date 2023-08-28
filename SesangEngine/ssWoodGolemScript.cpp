#include "ssWoodGolemScript.h"
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


namespace ss
{
	WoodGolemScript::WoodGolemScript()
	{
		m_tMonsterInfo.m_fSpeed = 50.f;
		m_tMonsterInfo.m_fNearAttackRange = 25.f;
		m_tMonsterInfo.m_fDetectRange = 180.f;
		m_tMonsterInfo.m_fCoolDown = 0.1f;
	}
	WoodGolemScript::~WoodGolemScript()
	{
	}
	void WoodGolemScript::Initialize()
	{
		MonsterScript::Initialize();

		mTransform->SetScale(Vector3(111.f, 83.f, 1.f)); // backsize랑 스케일값 동일하게 주기. 


		//
		mCharacterState->SetCurrentHP(110.f);
		mCharacterState->SetMaxHP(110.f);

		mFirstPos = GetOwner()->GetComponent<Transform>()->GetPosition();

		//
		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Lizard_Idle");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Lizard_Run");
		std::shared_ptr<ss::graphics::Texture> Image3 = Resources::Find<ss::graphics::Texture>(L"Lizard_NearAttack");
		std::shared_ptr<ss::graphics::Texture> Image4 = Resources::Find<ss::graphics::Texture>(L"Lizard_Hit");
		std::shared_ptr<ss::graphics::Texture> Image5 = Resources::Find<ss::graphics::Texture>(L"Lizard_Stun");
		std::shared_ptr<ss::graphics::Texture> Image6 = Resources::Find<ss::graphics::Texture>(L"Lizard_Die");


		// 플레이어 애니메이션은 좀 특수하므로, 무기 별로 오프셋 값 다르게 주되 백사이즈는	 동일하게 주기. (사이즈 값은 틀려도 됨) 
		// 애니메이션 나중에 플레이어 다 완성되면 
		//  LT, 1프레임 사를 사이즈, 자를 갯수, 오프셋, , 재생시간 뒤집기
		// === 
		// 플레이어 애니메이션은 좀 특수하므로, 무기 별로 오프셋 값 다르게 주되 백사이즈는	 동일하게 주기. (사이즈 값은 틀려도 됨) 
		// 애니메이션 나중에 플레이어 다 완성되면 
		//  LT, 1프레임 사를 사이즈, 자를 갯수(1부터 시작해서 세기), 백사이즈, 오프셋
		mAnimator->Create(L"Lizard_IdleR", Image1, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 7, Vector2(96.f, 48.f));
		mAnimator->Create(L"Lizard_IdleL", Image1, Vector2(0.f, 0.f), Vector2(96.f, 48.f), 7, Vector2(96.f, 48.f), Vector2(0.f, 0.f), 0.1f, true);

	}
	void WoodGolemScript::Update()
	{
	}
	void WoodGolemScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void WoodGolemScript::OnCollisionStay(Collider2D* other)
	{
	}
	void WoodGolemScript::OnCollisionExit(Collider2D* other)
	{
	}
	void WoodGolemScript::Move()
	{


	}
}