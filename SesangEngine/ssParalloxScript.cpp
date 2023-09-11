#include "ssParalloxScript.h"
#include "ssMeshRenderer.h"
#include "ssResources.h"
#include "ssGameObject.h"
#include "ssAnimator.h"
#include "ssMonster.h"
#include "ssSceneManager.h"
#include "ssBackground.h"



namespace ss
{
	ParalloxScript::ParalloxScript()
	{		
	}

	ParalloxScript::~ParalloxScript()
	{
	}

	void ParalloxScript::Initialize()
	{
		MeshRenderer* mr = GetOwner()->GetComponent<MeshRenderer>();
		Transform* tr = GetOwner()->GetComponent<Transform>();


		// ��渶�� ũ�Ⱑ �ٸ��Ƿ� �Ʒ�ó�� ���ش�.
		if (GetOwner()->GetName() == L"HowlingEffectObj")
		{
			tr->SetScale(Vector3(583.f, 123.f, 0.f));

		}
	}

	void ParalloxScript::Update()
	{
	}

	void ParalloxScript::OnCollisionEnter(Collider2D* other)
	{
	}

	void ParalloxScript::OnCollisionStay(Collider2D* other)
	{
	}

	void ParalloxScript::OnCollisionExit(Collider2D* other)
	{
	}
}
