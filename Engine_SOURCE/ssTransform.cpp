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
		// 이동 회전 크기 변경
	}

	void Transform::LateUpdate()
	{
		// 렌더링에 사용될 위치값을 업데이트.
		// mWorld을 월드행렬을 넣을 용도로 사용하기 전에 단위행렬로 만든다
		mWorld = Matrix::Identity;


		// 월드 행렬은 반드시 [크기*회전*이동] 이 순서로 곱해줘야한다.
		// 이동을 먼저 곱하고 회전을 하면 전혀 엉뚱한 위치로 obj가 가있게 된다. 

		// ====== <로컬 좌표계 기준>으로 크기 회전 이동 생성
		// 크기 행렬을 만든다
		Matrix scale = Matrix::CreateScale(mRelativeScale);
		mWorldScale = mRelativeScale;

		// 회전 행렬을 만든다
		Matrix rotation;
		rotation = Matrix::CreateRotationX(mRelativeRot.x);
		rotation *= Matrix::CreateRotationY(mRelativeRot.y);
		rotation *= Matrix::CreateRotationZ(mRelativeRot.z);
		mWorldRot = mRelativeRot;
		
		// 이동 행렬을 만든다
		Vector3 pos = mRelativePos;

		Matrix position;
		position.Translation(pos);
		mWorldPos = pos;

		// === 
		// == 객체의 로컬 좌표계를 <월드 좌표계>로 변환한다.
		// 이 행렬을 사용하면 객체의 로컬 좌표(예: 객체의 중심이 (0,0,0)인 좌표)를
		// 월드 좌표(즉, 게임 세계에서의 실제 위치)로 변환할 수 있습니다.
		mWorld = scale * rotation * position;


		// === 방향벡터에 회전행렬을 곱해준다.
		// 객체의 로컬 방향 벡터(up, forward, right)를 월드 좌표계로 변환해주는 것.
		// 이렇게 하면 객체가 회전할 때 그 방향 벡터도 적절히 회전하게 됩니다.
		mWorldForward = mRelativeUp = Vector3::TransformNormal(Vector3::Up, rotation);
		mWorldRight = mRelativeForward = Vector3::TransformNormal(Vector3::Forward, rotation);
		mWorldUp = mRelativeRight = Vector3::TransformNormal(Vector3::Right, rotation);


		// == ↓ 부모 오브젝트와 자식 오브젝트의 행렬 설명
		/*
		부모 오브젝트를 이동시킨다면 월드 좌표계를 기준으로 이동하게 될 것이다
		(우리 엔진이 pos를 월드 좌표계 기준으로 삼고 있음)
		자식 오브젝트를 옆으로 이동시킨다면, 그건 부모의 월드 좌표를 기준으로 이동하는거다.

		즉, 이동을 한 자식 오브젝트는 로컬에서 부모의 월드 좌표계로 온 다음에(여기까진 부모행렬꺼 씀)
		공간 변환을 한번 더 한 것이다. 자식의 행렬로 말이다. 그리고 나서 뷰행렬로 이동하는거다.

		즉, 자식 오브젝트는 부모 행렬에다가 '(자기자신)자식 행렬'이 하나 더 추가된 것 뿐임.

		즉 부모의 월드 행렬은 자식의 월드 행렬에 영향을 주지 않는다.


		부모 월드 행렬 = 로컬 * 월드 행렬
		자식 월드 행렬 = (자식 로컬 행렬) *(부모 월드 행렬)

		*/

		// ==== 부모로 설정된 오브젝트가 있다면 
		if (mParent)
		{

			mParentMat = mWorld * mParent->mWorld;
			//Matrix parentScale = Matrix::CreateScale(mParent->mWorldScale);
			//Matrix in = parentScale.Invert();
			//Matrix out = in * mParent->mWorld;
			//mWorld *= out;

		}

		//// - 월드 좌표, 크기, 회전 갱신
		//Quaternion worldRot;
		//mWorld.Decompose(mWorldScale, worldRot, mWorldPos);
		//Vector3 quatToRadRot = worldRot.ToEuler();
		//mWorldRot = quatToRadRot;


		//// 회전, 크기 변환이 이루어졌기 때문에 변경된 크기를 초기화 하기 위해 Normalize
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
		// 상수 버퍼에 월드, 뷰, 투영 행렬을 담아 보내준다. 
		renderer::TransformCB trCB = {};
		trCB.mWorld = mWorld;
		trCB.mView = Camera::GetGpuViewMatrix();
		trCB.mProjection = Camera::GetGpuProjectionMatrix();

		// 상수 버퍼 중에 Transform 상수 버퍼를 갖고 온다. 
		ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Transform];
		cb->SetData(&trCB); // 월드 행렬 정보를 상수 버퍼에 넣어준다.
		cb->Bind(eShaderStage::VS); // 상수 버퍼는 어느 쉐이더 단계이든 바인딩할 수 있다는게 장점이다. 
	}

}