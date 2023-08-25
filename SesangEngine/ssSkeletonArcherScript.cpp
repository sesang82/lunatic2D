#include "ssSkeletonArcherScript.h"
#include "ssFSM.h"
#include "ssIdleState.h"
#include "ssTraceState.h"
#include "ssResources.h"
#include "ssTexture.h"
#include "ssAnimator.h"
#include "ssCharacterState.h"

namespace ss
{
	SkeletonArcherScript::SkeletonArcherScript()
	{
		m_tMonsterInfo.m_fSpeed = 200.f;
		m_tMonsterInfo.m_fAttack = 10.f;
		m_tMonsterInfo.m_fDetectRange = 300.f;
	}
	SkeletonArcherScript::~SkeletonArcherScript()
	{
	}
	void SkeletonArcherScript::Initialize()
	{
		MonsterScript::Initialize();

		mTransform->SetScale(Vector3(73.f, 61.f, 1.f)); // backsize랑 스케일값 동일하게 주기. 

		mCollider->SetName(L"colHit_player");
		mCollider->SetType(eColliderType::Rect);
		mCollider->SetSize(Vector2(0.4f, 0.9f));
		mCollider->SetCenter(Vector2(-6.f, -0.f));



		//
		mCharacterState->SetCurrentHP(110.f);
		mCharacterState->SetMaxHP(110.f);



		//
		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Archer_Idle");
		std::shared_ptr<ss::graphics::Texture> Image2 = Resources::Find<ss::graphics::Texture>(L"Archer_Run");
		std::shared_ptr<ss::graphics::Texture> Image3 = Resources::Find<ss::graphics::Texture>(L"Archer_Hit");
		std::shared_ptr<ss::graphics::Texture> Image4 = Resources::Find<ss::graphics::Texture>(L"Archer_NearAttack");
		std::shared_ptr<ss::graphics::Texture> Image5 = Resources::Find<ss::graphics::Texture>(L"Archer_FarAttack");
		std::shared_ptr<ss::graphics::Texture> Image6 = Resources::Find<ss::graphics::Texture>(L"Archer_Stun");
		std::shared_ptr<ss::graphics::Texture> Image7 = Resources::Find<ss::graphics::Texture>(L"Archer_Die");

		// 플레이어 애니메이션은 좀 특수하므로, 무기 별로 오프셋 값 다르게 주되 백사이즈는	 동일하게 주기. (사이즈 값은 틀려도 됨) 
		// 애니메이션 나중에 플레이어 다 완성되면 
		//  LT, 1프레임 사를 사이즈, 자를 갯수, 오프셋, , 재생시간 뒤집기
		// === 
		// 플레이어 애니메이션은 좀 특수하므로, 무기 별로 오프셋 값 다르게 주되 백사이즈는	 동일하게 주기. (사이즈 값은 틀려도 됨) 
		// 애니메이션 나중에 플레이어 다 완성되면 
		//  LT, 1프레임 사를 사이즈, 자를 갯수, 백사이즈, 오프셋
		mAnimator->Create(L"Archer_Idle", Image1, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 4, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_Run", Image2, Vector2(0.f, 0.f), Vector2(73.f, 61.f),8, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_Hit", Image3, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 1, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_NearAttack", Image4, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 13, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_FarAttack", Image5, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 12, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_Stun", Image6, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 5, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_Die", Image7, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 17, Vector2(73.f, 61.f));

		// 일단 Idle 상태는 나중으로 만들기 
		mAnimator->PlayAnimation(L"Archer_Run", true);






	}
	void SkeletonArcherScript::Update()
	{

	}
	void SkeletonArcherScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void SkeletonArcherScript::OnCollisionStay(Collider2D* other)
	{
	}
	void SkeletonArcherScript::OnCollisionExit(Collider2D* other)
	{
	}
}