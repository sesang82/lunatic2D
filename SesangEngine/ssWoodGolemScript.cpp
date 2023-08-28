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

		mTransform->SetScale(Vector3(111.f, 83.f, 1.f)); // backsize�� �����ϰ� �����ϰ� �ֱ�. 


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


		// �÷��̾� �ִϸ��̼��� �� Ư���ϹǷ�, ���� ���� ������ �� �ٸ��� �ֵ� ��������	 �����ϰ� �ֱ�. (������ ���� Ʋ���� ��) 
		// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
		//  LT, 1������ �縦 ������, �ڸ� ����, ������, , ����ð� ������
		// === 
		// �÷��̾� �ִϸ��̼��� �� Ư���ϹǷ�, ���� ���� ������ �� �ٸ��� �ֵ� ��������	 �����ϰ� �ֱ�. (������ ���� Ʋ���� ��) 
		// �ִϸ��̼� ���߿� �÷��̾� �� �ϼ��Ǹ� 
		//  LT, 1������ �縦 ������, �ڸ� ����(1���� �����ؼ� ����), �������, ������
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