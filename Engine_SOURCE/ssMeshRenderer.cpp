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

	// ������� ������ ����ϴ� ������Ʈ�̹Ƿ�, Transform ������Ʈ�� �̿��Ͽ� ��ġ�� ������ �� �׸��� �Ѵ�. 
	void MeshRenderer::Render()
	{
		// ���ӿ�����Ʈ�� �⺻������ Ʈ�������� ���� �ְ� �߱� ������ �Ʒ�ó�� �ٷ� ������ �� �ִ�.
		Transform* tr = GetOwner()->GetComponent<Transform>();
		tr->BindConstantBuffer();


		// === ��ü 1���� �������� ���� 
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