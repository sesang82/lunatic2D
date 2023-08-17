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
		// 음수로 빠질 때 걸리도록 해둠
		assert(0 <= mCount);

		Transform* tr = GetOwner()->GetComponent<Transform>();

		// 충돌체의 크기와 위치 값을 각각 가져온다.
		// 또한 가져온 값에 충돌체 멤버 값을 곱하거나 더해줘서
		// 멤버 만으로도 컨트롤할 수 있도록 해준다.
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

		/// matrix 생성
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
		mesh.ColCount = mCount; // 충돌 횟수를 전달함

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
	// count는 충돌 중일 때는 체크할 대상이 아니므로 상관없음 
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
		// count는 충돌 중일 때는 체크할 대상이 아니므로 상관없음 
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
