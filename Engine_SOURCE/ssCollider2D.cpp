#include "ssCollider2D.h"
#include "ssGameObject.h"
#include "ssRenderer.h"

namespace ss
{
	UINT Collider2D::mColliderNumber = 0;

	Collider2D::Collider2D()
		: Component(eComponentType::Collider2D)
		, mTransform(nullptr)
		, mSize(Vector2::One)
		, mCenter(Vector2::Zero)
		, mType(eColliderType::Rect)
	{
		SetName(L"Collier2D");
		mColliderNumber++;
		mColliderID = mColliderNumber;
	}

	Collider2D::~Collider2D()
	{
	}
	void Collider2D::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();
	}

	void Collider2D::Update()
	{
	
	}
	void Collider2D::LateUpdate()
	{
		// ������ ���� �� �ɸ����� �ص�
		assert(0 <= mCount);

		Transform* tr = GetOwner()->GetComponent<Transform>();

		// �浹ü�� ũ��� ��ġ ���� ���� �����´�.
		// ���� ������ ���� �浹ü ��� ���� ���ϰų� �����༭
		// ��� �����ε� ��Ʈ���� �� �ֵ��� ���ش�.
		mScale = tr->GetWorldScale();
		mScale *= Vector3(mSize.x, mSize.y, 1.0f);

		Vector3 rotation = tr->GetWorldRotation();


		GameObject* Parent = GetOwner()->GetParent();
		Vector3 pos = tr->GetWorldPosition();

		if (nullptr != Parent)
		{
			Transform* parentTr = Parent->GetComponent<Transform>();
			rotation += parentTr->GetRotation();
			pos += parentTr->GetPosition();
			//mScale *= parentTr->GetScale();
	
		}


		Vector3 ColPos = pos + Vector3(mCenter.x, mCenter.y, 0.0f);
		mPosition = ColPos;

		/// matrix ����
		Matrix scaleMatrix = Matrix::CreateScale(mScale);
		Matrix rotationMatrix;
		rotationMatrix = Matrix::CreateRotationX(rotation.x);
		rotationMatrix *= Matrix::CreateRotationX(rotation.y);
		rotationMatrix *= Matrix::CreateRotationX(rotation.z);

		Matrix positionMatrix;
		positionMatrix.Translation(Vector3(ColPos.x, ColPos.y, ColPos.z));

		Matrix worldMatrix = scaleMatrix * rotationMatrix * positionMatrix;



		graphics::DebugMesh mesh = {};
		mesh.position = mPosition;
		mesh.scale = mScale;
		mesh.rotation = rotation;
		mesh.type = m_Type;
		mesh.ColCount = mCount; // �浹 Ƚ���� ������

		renderer::PushDebugMeshAttribute(mesh);


	}
	void Collider2D::Render()
	{
	}

	void Collider2D::OnCollisionEnter(Collider2D* other)
	{


		++mCount;

		const std::vector<Script*> scripts
			= GetOwner()->GetComponents<Script>();

		for (Script* script : scripts)
		{
			script->OnCollisionEnter(other);
		}
	}
	void Collider2D::OnCollisionStay(Collider2D* other)
	{
	// count�� �浹 ���� ���� üũ�� ����� �ƴϹǷ� ������� 
		const std::vector<Script*> scripts
			= GetOwner()->GetComponents<Script>();

		for (Script* script : scripts)
		{
			script->OnCollisionStay(other);
		}
	}
	void Collider2D::OnCollisionExit(Collider2D* other)
	{
		--mCount;

		const std::vector<Script*> scripts
			= GetOwner()->GetComponents<Script>();

		for (Script* script : scripts)
		{
			script->OnCollisionExit(other);
		}
	}

	void Collider2D::OnCollisionEnter(Collider2D* in, Collider2D* other)
	{
		++mCount;

		const std::vector<Script*> scripts
			= GetOwner()->GetComponents<Script>();

		for (Script* script : scripts)
		{
			script->OnCollisionEnter(in);
			script->OnCollisionEnter(other);
		}
	}

	void Collider2D::OnCollisionStay(Collider2D* in, Collider2D* other)
	{
		// count�� �浹 ���� ���� üũ�� ����� �ƴϹǷ� ������� 
		const std::vector<Script*> scripts
			= GetOwner()->GetComponents<Script>();

		for (Script* script : scripts)
		{
			script->OnCollisionStay(in);
			script->OnCollisionStay(other);
		}
	}

	void Collider2D::OnCollisionExit(Collider2D* in, Collider2D* other)
	{
		--mCount;

		const std::vector<Script*> scripts
			= GetOwner()->GetComponents<Script>();

		for (Script* script : scripts)
		{
			script->OnCollisionExit(in);
			script->OnCollisionExit(other);
		}
	}

	void Collider2D::OnTriggerEnter(Collider2D* collider)
	{
		const std::vector<Script*> scripts
			= GetOwner()->GetScripts();

		for (Script* script : scripts)
		{
			script->OnTriggerEnter(collider);
		}
	}

	void Collider2D::OnTriggerStay(Collider2D* collider)
	{

		const std::vector<Script*> scripts
			= GetOwner()->GetScripts();

		for (Script* script : scripts)
		{
			script->OnTriggerStay(collider);
		}
	}

	void Collider2D::OnTirggerExit(Collider2D* collider)
	{

		const std::vector<Script*> scripts
			= GetOwner()->GetScripts();

		for (Script* script : scripts)
		{
			script->OnTriggerExit(collider);
		}
	}

}
