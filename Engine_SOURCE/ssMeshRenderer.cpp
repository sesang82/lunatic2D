#include "ssMeshRenderer.h"
#include "ssGameObject.h"
#include "ssTransform.h"
#include "ssRenderer.h"
#include "ssAnimator.h"
#include "ssAnimation.h"
#include "../SesangEngine/ssMonsterScript.h"
#include "../SesangEngine/ssPlayerScript.h"
#include "ssTime.h"

namespace ss
{
	MeshRenderer::MeshRenderer()
		: Component(eComponentType::MeshRenderer)
		, Time(0.f)
	{
	}
	MeshRenderer::~MeshRenderer()
	{
	}
	void MeshRenderer::Initialize()
	{
	}
	void MeshRenderer::Update()
	{
		

	}
	void MeshRenderer::LateUpdate()
	{
	}

	// 출력할지 말지를 담당하는 컴포넌트이므로, Transform 컴포넌트를 이용하여 위치를 설정한 후 그리게 한다. 
	void MeshRenderer::Render()
	{
		// 게임오브젝트는 기본적으로 트랜스폼을 갖고 있게 했기 때문에 아래처럼 바로 꺼내올 수 있다.
		Transform* tr = GetOwner()->GetComponent<Transform>();
		tr->BindConstantBuffer();


		// === 물체 1개당 렌더링할 역할 
		mMesh->BindBuffer();
		mMaterial->Binds();

		Animator* animator = GetOwner()->GetComponent<Animator>();

		if (animator)
		{
			animator->Binds();
		}


		PlayerScript* Playerscript = GetOwner()->GetComponent<PlayerScript>();

		if (Playerscript)
		{
			Playerscript->BindConstantBuffer();
		}

		MonsterScript* monscript = GetOwner()->GetComponent<MonsterScript>();

		if (monscript)
		{
			monscript->BindConstantBuffer();
		}

	

		mMesh->Render();

		mMaterial->Clear();
	}
}