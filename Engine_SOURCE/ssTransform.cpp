#include "ssTransform.h"
#include "ssRenderer.h"
#include "ssConstantBuffer.h"
#include "ssCamera.h"
#include "ssGameObject.h"

namespace ss
{
	using namespace ss::graphics;
	Transform::Transform()
		: Component(eComponentType::Transform)
		, mRelativePos(Vector3::Zero)
		, mRelativeRot(Vector3::Zero)
		, mRelativeScale(Vector3::One)
		, mParent(nullptr)
	
		, mWorldPos(Vector3::Zero)
		, mWorldRot(Vector3::Zero)
		, mWorldScale(Vector3::One)
		, mRelativeUp(Vector3::Up)
		, mRelativeForward(Vector3::Forward)
		, mRelativeRight(Vector3::Right)
		, mFlip(false)
		//, mWorldForward(Vector3::Forward)
		//, mWorldRight(Vector3::Right)
		//, mWorldUp(Vector3::Up)
	{
	}

	Transform::~Transform()
	{
	}

	void Transform::Initialize()
	{

	}

	void Transform::Update()
	{
		// �̵� ȸ�� ũ�� ����
	}

	void Transform::LateUpdate()
	{
		// �������� ���� ��ġ���� ������Ʈ.
		// mWorld�� ��������� ���� �뵵�� ����ϱ� ���� ������ķ� �����
		mWorld = Matrix::Identity;


		// ���� ����� �ݵ�� [ũ��*ȸ��*�̵�] �� ������ ��������Ѵ�.
		// �̵��� ���� ���ϰ� ȸ���� �ϸ� ���� ������ ��ġ�� obj�� ���ְ� �ȴ�. 

		// ====== <���� ��ǥ�� ����>���� ũ�� ȸ�� �̵� ����
		// ũ�� ����� �����
		Matrix scale = Matrix::CreateScale(mRelativeScale);
		mWorldScale = mRelativeScale;

		// ȸ�� ����� �����
		Matrix rotation;
		rotation = Matrix::CreateRotationX(mRelativeRot.x);
		rotation *= Matrix::CreateRotationY(mRelativeRot.y);
		rotation *= Matrix::CreateRotationZ(mRelativeRot.z);
		mWorldRot = mRelativeRot;
		
		// �̵� ����� �����
		Vector3 pos = mRelativePos;

		Matrix position;
		position.Translation(pos);
		mWorldPos = pos;

		// === 
		// == ��ü�� ���� ��ǥ�踦 <���� ��ǥ��>�� ��ȯ�Ѵ�.
		// �� ����� ����ϸ� ��ü�� ���� ��ǥ(��: ��ü�� �߽��� (0,0,0)�� ��ǥ)��
		// ���� ��ǥ(��, ���� ���迡���� ���� ��ġ)�� ��ȯ�� �� �ֽ��ϴ�.
		mWorld = scale * rotation * position;


		// === ���⺤�Ϳ� ȸ������� �����ش�.
		// ��ü�� ���� ���� ����(up, forward, right)�� ���� ��ǥ��� ��ȯ���ִ� ��.
		// �̷��� �ϸ� ��ü�� ȸ���� �� �� ���� ���͵� ������ ȸ���ϰ� �˴ϴ�.
		mWorldForward = mRelativeUp = Vector3::TransformNormal(Vector3::Up, rotation);
		mWorldRight = mRelativeForward = Vector3::TransformNormal(Vector3::Forward, rotation);
		mWorldUp = mRelativeRight = Vector3::TransformNormal(Vector3::Right, rotation);


		// == �� �θ� ������Ʈ�� �ڽ� ������Ʈ�� ��� ����
		/*
		�θ� ������Ʈ�� �̵���Ų�ٸ� ���� ��ǥ�踦 �������� �̵��ϰ� �� ���̴�
		(�츮 ������ pos�� ���� ��ǥ�� �������� ��� ����)
		�ڽ� ������Ʈ�� ������ �̵���Ų�ٸ�, �װ� �θ��� ���� ��ǥ�� �������� �̵��ϴ°Ŵ�.

		��, �̵��� �� �ڽ� ������Ʈ�� ���ÿ��� �θ��� ���� ��ǥ��� �� ������(������� �θ���Ĳ� ��)
		���� ��ȯ�� �ѹ� �� �� ���̴�. �ڽ��� ��ķ� ���̴�. �׸��� ���� ����ķ� �̵��ϴ°Ŵ�.

		��, �ڽ� ������Ʈ�� �θ� ��Ŀ��ٰ� '(�ڱ��ڽ�)�ڽ� ���'�� �ϳ� �� �߰��� �� ����.

		�� �θ��� ���� ����� �ڽ��� ���� ��Ŀ� ������ ���� �ʴ´�.


		�θ� ���� ��� = ���� * ���� ���
		�ڽ� ���� ��� = (�ڽ� ���� ���) *(�θ� ���� ���)

		*/

		// ==== �θ�� ������ ������Ʈ�� �ִٸ� 
		if (mParent)
		{

			mParentMat = mWorld * mParent->mWorld;
			//Matrix parentScale = Matrix::CreateScale(mParent->mWorldScale);
			//Matrix in = parentScale.Invert();
			//Matrix out = in * mParent->mWorld;
			//mWorld *= out;

		}

		//// - ���� ��ǥ, ũ��, ȸ�� ����
		//Quaternion worldRot;
		//mWorld.Decompose(mWorldScale, worldRot, mWorldPos);
		//Vector3 quatToRadRot = worldRot.ToEuler();
		//mWorldRot = quatToRadRot;


		//// ȸ��, ũ�� ��ȯ�� �̷������ ������ ����� ũ�⸦ �ʱ�ȭ �ϱ� ���� Normalize
		//mWorldForward = Vector3::TransformNormal(mRelativeForward, rotation);
		//mWorldRight = Vector3::TransformNormal(mRelativeRight, rotation);
		//mWorldUp = Vector3::TransformNormal(mRelativeUp, rotation);
		//mWorldForward.Normalize();
		//mWorldRight.Normalize();
		//mWorldUp.Normalize();

	}

	void Transform::Render()
	{

	}

	void Transform::BindConstantBuffer()
	{
		// ��� ���ۿ� ����, ��, ���� ����� ��� �����ش�. 
		renderer::TransformCB trCB = {};
		trCB.mWorld = mWorld;
		trCB.mView = Camera::GetGpuViewMatrix();
		trCB.mProjection = Camera::GetGpuProjectionMatrix();

		// ��� ���� �߿� Transform ��� ���۸� ���� �´�. 
		ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Transform];
		cb->SetData(&trCB); // ���� ��� ������ ��� ���ۿ� �־��ش�.
		cb->Bind(eShaderStage::VS); // ��� ���۴� ��� ���̴� �ܰ��̵� ���ε��� �� �ִٴ°� �����̴�. 
	}

}