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

		mTransform->SetScale(Vector3(73.f, 61.f, 1.f)); // backsize�� �����ϰ� �����ϰ� �ֱ�. 

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

		// �÷��̾� �ִϸ��̼��� �� Ư���ϹǷ�, ���� ���� ������ �� �ٸ��� �ֵ� ��������	 �����ϰ� �ֱ�. (������ ���� Ʋ���� ��) 
		// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
		//  LT, 1������ �縦 ������, �ڸ� ����, ������, , ����ð� ������
		// === 
		// �÷��̾� �ִϸ��̼��� �� Ư���ϹǷ�, ���� ���� ������ �� �ٸ��� �ֵ� ��������	 �����ϰ� �ֱ�. (������ ���� Ʋ���� ��) 
		// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
		//  LT, 1������ �縦 ������, �ڸ� ����, �������, ������
		mAnimator->Create(L"Archer_Idle", Image1, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 4, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_Run", Image2, Vector2(0.f, 0.f), Vector2(73.f, 61.f),8, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_Hit", Image3, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 1, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_NearAttack", Image4, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 13, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_FarAttack", Image5, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 12, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_Stun", Image6, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 5, Vector2(73.f, 61.f));
		mAnimator->Create(L"Archer_Die", Image7, Vector2(0.f, 0.f), Vector2(73.f, 61.f), 17, Vector2(73.f, 61.f));

		// �ϴ� Idle ���´� �������� ����� 
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