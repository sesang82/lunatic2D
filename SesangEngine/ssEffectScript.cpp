#include "ssEffectScript.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssGameObject.h"
#include "ssAnimator.h"
#include "ssMonster.h"
#include "ssSceneManager.h"
#include "ssWoodGolemScript.h"
#include "ssWolfScript.h"
#include "ssZombieScript.h"
#include "ssBigWolfScript.h"


namespace ss
{
	EffectScript::EffectScript()
	{

	}
	EffectScript::~EffectScript()
	{
	}
	void EffectScript::Initialize()
	{
		MeshRenderer* mr = GetOwner()->GetComponent<MeshRenderer>();
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Animator* anim = GetOwner()->GetComponent<Animator>();

		std::shared_ptr<ss::graphics::Texture> Image1 = Resources::Find<ss::graphics::Texture>(L"Boss_Wolf_HowlingEffect");

		anim->Create(L"Boss_Wolf_HowlingEffect", Image1, Vector2(0.f, 0.f), Vector2(583.f, 123.f), 6, Vector2(583.f, 123.f));
		//mAnimator->Create(L"Boss_Wolf_HowlingEffectL", Image10, Vector2(0.f, 0.f), Vector2(272.f, 271.f), 5, Vector2(272.f, 271.f), Vector2(-16.f, 0.f), 0.1f, true);
	


		// 일단 이렇게 한 이유는 hit ground의 크기가 다를 수도 있어서 일단 이렇게 해놨는데 나중에 그럴 필요없으면 리팩토링 하기
		if (GetOwner()->GetName() == L"HowlingEffectObj")
		{
			tr->SetScale(Vector3(583.f, 123.f, 0.f));
			anim->PlayAnimation(L"Boss_Wolf_HowlingEffect", true);
			
		}
	}


	void EffectScript::Update()
	{

		

		//if (SceneManager::GetActiveScene()->GetName() == L"Stage1Scene")
		if (GetOwner()->GetName() == L"HowlingEffectObj")
		{
			BigWolfScript* bosswolfScript = mOwnerObj->GetComponent<BigWolfScript>();
			eWolfBossState state = bosswolfScript->GetCurWolfState();

			if (bosswolfScript->GetCurWolfState() == eWolfBossState::HOWLING_END)
			{
				GetOwner()->SetState(GameObject::eState::Dead); // 하울링 이펙트를 제거한다.
			}

		}
	}
	void EffectScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void EffectScript::OnCollisionStay(Collider2D* other)
	{
	}
	void EffectScript::OnCollisionExit(Collider2D* other)
	{
	}
}