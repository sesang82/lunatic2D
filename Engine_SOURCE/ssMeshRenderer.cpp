#include "ssMeshRenderer.h"
#include "ssGameObject.h"
#include "ssTransform.h"
#include "ssRenderer.h"
#include "ssAnimator.h"

namespace ss
{
	MeshRenderer::MeshRenderer()
		: Component(eComponentType::MeshRenderer)
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

		mMesh->Render();

		mMaterial->Clear();
	}
}