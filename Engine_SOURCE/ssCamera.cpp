#include "ssCamera.h"
#include "ssTransform.h"
#include "ssGameObject.h"
#include "ssApplication.h"
#include "ssRenderer.h"
#include "ssScene.h"
#include "ssSceneManager.h"
#include "ssMeshRenderer.h"

extern ss::Application application;

namespace ss
{

	// static 변수 초기화(단위행렬로 각각 초기화해둔다)
	Matrix Camera::View = Matrix::Identity;
	Matrix Camera::Projection = Matrix::Identity;

	// ==== 반투명, 투명한 녀석들은 z값으로 정렬 필요
	// 현재 우리는 z값이 클수록 멀리 있는 것으로 판정하고 있으므로
	// 그에 맞게 함수를 기재해줌 
	bool CompareZSort(GameObject* a, GameObject* b)
	{
		if (a->GetComponent<Transform>()->GetPosition().z
			<= b->GetComponent<Transform>()->GetPosition().z)
			return false;

		return true;
	}

	Camera::Camera()
		: Component(eComponentType::Camera)
		, mType(eProjectionType::OrthoGraphic) // 2D기 때문에 투영방식은 일단 Ortho로 고정
		, mAspectRatio(1.0f)
		, mNear(1.0f) // near랑 far는 보통 1과 1000으로 많이 쓴다
		, mFar(1000.0f)
		, mSize(2.f)
		, mLayerMask{}
		, mOpaqueGameObjects{}
		, mCutOutGameObjects{}
		, mTransparentGameObjects{}
		, mView(Matrix::Identity)
		, mProjection(Matrix::Identity)
	{
		// 특정한 레이어만 가릴지 아니면 보일지를 결정하려면 우선
	// 어떤 레이어가 있는지 봐야하므로 카메라가 모든 레이어를 비추게 한다.
		EnableLayerMasks();

		mCurrentSize = mSize; // 시작 시에 현재 크기를 초기화
		mTargetSize = mSize; // 목표 크기도 초기 크기와 동일하게 설정
		mLerpSpeed = 0.005f; // 보간 속도 설정. 이 값을 조절하여 확대/축소 속도를 변경 
		mIsZooming = false; // 확대/축소 중인지를 나타내는 플래그
	}

	Camera::~Camera()
	{
	}

	void Camera::Initialize()
	{

	}

	void Camera::Update()
	{
	}

	void Camera::LateUpdate()
	{
		// 물체의 위치는 계속 갱신되어야하므로 뷰행렬과 투영행렬 또한 Update에 넣어서
	// 계속 행렬을 갱신해준다. 
		CreateViewMatrix();
		CreateProjectionMatrix(mType);

		// 카메라를 렌더러에 등록해준다. (이걸 LateUpdate에서 해준 이유 다시한번 여쭈기)
	// 쌤이 동적할당이 자연스럽게 된다고 하셨는데....
		RegisterCameraInRenderer();

		//// 카메라 컴포넌트의 transform을 얻어온다.
	//Transform* tr = GetOwner()->GetComponent<Transform>();


	//// 만약에 지정된 타겟이 있다면
	//if (mTarget)
	//{
	//	if (mTarget->GetState() == GameObject::eState::Dead)
	//		mTarget = nullptr;

	//	else if (mTarget->GetState() == GameObject::eState::Active)
	//	{
	//		// 게임오브젝트의 transform을 얻어온다.
	//		Vector3 ObjPos = mTarget->GetComponent<Transform>()->GetPosition();
	//		mCameraPos = ObjPos + mTargetOffset;
	//	}
	//}

		UpdateZoom(); // 확대/축소 업데이트


	}

	void Camera::Render()
	{
		// 트랜스폼의 상수버퍼에서, 쓰고있는 카메라의 행렬을 알아야하므로 
	// static변수에 값을 옮겨준다. 
		View = mView;
		Projection = mProjection;

		// Add게임오브젝트 함수로 그냥 넣어진 것들 렌더링 모드에 따른 분류 작업 
		AlphaSortGameObjects();
		ZSortTransparencyGameObjects();  // 투명도가 있는 물체들은 Z값으로 정렬을 해줘야한다.

		// ==== 불투명 물체 렌더링
		RenderOpaque(); // 불투명한 물체들은 그냥 그리면 된다.

		// ==== 반투명, 투명 물체 렌더링
		DisableDepthStencilState(); // 깊이값 기록x, 깊이값 테스트x
		RenderCutOut();	// 반투명 물체 렌더링 
		RenderTransparent(); // 투명 물체 렌더링
		EnableDepthStencilState(); // less모드로 다시 바꿈(또 그려질 불투명 물체를 위해)

		// Q. 반투명, 투명 물체를 렌더링할 때는 왜 깊이버퍼를 쓰다말았다 하는걸까?
		/*
		투명한 물체는 뒤에 있는 물체를 보여주어야 합니다.

		그러나 깊이 쓰기가 활성화된 상태에서 투명한 물체를 렌더링하면,
		투명한 물체의 깊이 정보가 깊이 버퍼에 기록되어 뒤에 있는 물체가 가려지게 됩니다.
		따라서 투명한 물체를 렌더링할 때는 깊이 쓰기를 비활성화하여 뒤에 있는 물체가 가려지지 않도록 합니다.

		반투명한 물체는 뒤에 있는 물체와 혼합되어 보여야 합니다.

		그러나 깊이 테스트가 활성화된 상태에서 반투명한 물체를 렌더링하면,
		반투명한 물체가 뒤에 있는 물체를 완전히 가려버릴 수 있습니다.
		따라서 반투명한 물체를 렌더링할 때는 깊이 테스트를 비활성화하여 뒤에 있는 물체와 혼합되도록 합니다.

		이렇게 하면 반투명하거나 투명한 물체를 올바르게 렌더링할 수 있습니다.
		그러나 이 방법은 물체의 렌더링 순서에 매우 민감합니다.

		일반적으로는 먼 물체부터 가까운 물체 순으로 렌더링해야 합니다.
		그래서 일반적으로는 불투명한 물체를 먼저 렌더링하고, 그 다음에 반투명하거나 투명한 물체를 렌더링합니다.
		이렇게 하면 불투명한 물체는 깊이 테스트와 깊이 쓰기를 사용하여 올바르게 렌더링되고,
		반투명하거나 투명한 물체는 뒤에 있는 물체와 올바르게 혼합됩니다.

		*/
	}

	// 뷰 행렬은 크기는 상관이 없기 때문에 이동과 회전 행렬만 구해준다. 
	bool Camera::CreateViewMatrix()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();

		// View Translate Matrix (이동 행렬)
		mView = Matrix::Identity;
		mView *= Matrix::CreateTranslation(-pos);


		// View Rotation Matrix (회전 행렬)
		Vector3 up = tr->Up();
		Vector3 right = tr->Right();
		Vector3 foward = tr->Forward();

		Matrix viewRotate;
		viewRotate._11 = right.x;	viewRotate._12 = up.x;	viewRotate._13 = foward.x;
		viewRotate._21 = right.y;	viewRotate._22 = up.y;	viewRotate._23 = foward.y;
		viewRotate._31 = right.z;	viewRotate._32 = up.z;	viewRotate._33 = foward.z;

		// 이동행렬을 넣어줬던 것에 회전행렬도 마저 곱해서 넣어준다. 
		// 최종적으로 뷰행렬 완성! 
		mView *= viewRotate;

		return true;
	}

	bool Camera::CreateProjectionMatrix(eProjectionType type)
	{
		RECT rect = {};
		GetClientRect(application.GetHwnd(), &rect); // 실제 해상도 크기를 가져온다.

		// ==== 종횡비를 구하기 위해 해상도의 가로길이와 세로길이를 구한다.
		float width = rect.right - rect.left;	// 해상도의 가로길이
		float height = rect.bottom - rect.top;	// 해상도의 세로길이 
		mAspectRatio = width / height;; 		// 종횡비를 구한다. 


		// 2D 포폴용인경우 OrthoGraphic 투영방식을 사용한다.
		if (type == eProjectionType::OrthoGraphic)
		{
			//float OrthorGraphicRatio = mSize / 1000.0f;
			//width *= OrthorGraphicRatio;
			//height *= OrthorGraphicRatio;

			//mProjection = Matrix::CreateOrthographicLH(width, height, mNear, mFar);

			//// mSize값에 따라 카메라의 뷰포트 크기를 정한다. 
			mProjection = Matrix::CreateOrthographicLH(width * (1.f / mSize), height * (1.f / mSize), mNear, mFar);
		}
		else
		{
			mProjection = Matrix::CreatePerspectiveFieldOfViewLH(XM_2PI / 6.0f, mAspectRatio, mNear, mFar);
		}

		return true;
	}

	void Camera::RegisterCameraInRenderer()
	{
		// 카메라 vector에 호출한 자기 자신을 넣어준다.
		renderer::cameras.push_back(this);
	}

	void Camera::TurnLayerMask(eLayerType type, bool enable)
	{
		// set함수는 std::bitset의 멤버 함수다. 결과값으로 true(=1)를 반환한다.
		mLayerMask.set((UINT)type, enable);
	}

	void Camera::AlphaSortGameObjects()
	{

		// Add게임오브젝트로 그냥 넣어진 물체들을 물체의 상태에 맞게 각 vector에 분류해준다.
		// Opaque, CutOut, Transparent
		// (일단 우리는 기본적으로 불투명한 물체를 먼저 렌더링 하도록 해뒀음) 
		// sort해주기 전에 각 vector를 비워준다.
		mOpaqueGameObjects.clear();
		mCutOutGameObjects.clear();
		mTransparentGameObjects.clear();

		// == Alpha sorting

		// 활성화된 씬을 가져온다.
		Scene* scene = SceneManager::GetActiveScene();
		for (size_t i = 0; i < (UINT)eLayerType::End; i++)
		{
			if (mLayerMask[i] == true)
			{
				// 씬에 있는 레이어들을 들고온다.
				Layer& layer = scene->GetLayer((eLayerType)i);

				// 레이어에 있는 게임오브젝트들을 gameObjs 벡터에 넣어준다.
				const std::vector<GameObject*> gameObjs
					= layer.GetGameObjects();

				// Add게임오브젝트로 그냥 넣어진 물체들을 물체의 상태에 맞게
				// 각 vector에 분류해준다.
				DivideAlphaBlendGameObjects(gameObjs);
			}
		}
	}

	void Camera::ZSortTransparencyGameObjects()
	{
		std::sort(mCutOutGameObjects.begin()
			, mCutOutGameObjects.end()
			, CompareZSort);
		std::sort(mTransparentGameObjects.begin()
			, mTransparentGameObjects.end()
			, CompareZSort);
	}

	void Camera::DivideAlphaBlendGameObjects(const std::vector<GameObject*> gameObjs)
	{
		// Add게임오브젝트로 그냥 넣어진 물체들을 물체의 상태에 맞게 각 vector에 분류해준다.
		// Opaque, CutOut, Transparent
		// (일단 우리는 기본적으로 불투명한 물체를 먼저 렌더링 하도록 해뒀음)

		for (GameObject* obj : gameObjs)
		{
			// 1. 일단 렌더링을 하는 게임오브젝트인지 확인한다.
			MeshRenderer* mr
				= obj->GetComponent<MeshRenderer>();

			// 메쉬렌더러가 없다면
			if (mr == nullptr)
				continue;

			// 메쉬렌더러가 있다면(재질로부터 렌더링 모드를 갖고온다)
			std::shared_ptr<Material> mt = mr->GetMaterial();
			eRenderingMode mode = mt->GetRenderingMode();

			// 렌더링될 물체에 따라 각기 다른 vector에 담아서
			// 각자 렌더링할 준비를 마친다.
			// (z값은 고려되지 않은 상태로 렌더링 모드만 고려해서 분류)
			switch (mode)
			{
			case ss::graphics::eRenderingMode::Opaque:
				mOpaqueGameObjects.push_back(obj);
				break;
			case ss::graphics::eRenderingMode::CutOut:
				mCutOutGameObjects.push_back(obj);
				break;
			case ss::graphics::eRenderingMode::Transparent:
				mTransparentGameObjects.push_back(obj);
				break;
			default:
				break;
			}
		}
	}

	void Camera::RenderOpaque()
	{
		for (GameObject* gameObj : mOpaqueGameObjects)
		{
			if (gameObj == nullptr)
				continue;
			if (gameObj->GetState()
				!= GameObject::eState::Active)
				continue;


			gameObj->Render();
		}
	}

	void Camera::RenderCutOut()
	{
		for (GameObject* gameObj : mCutOutGameObjects)
		{
			if (gameObj == nullptr)
				continue;
			if (gameObj->GetState()
				!= GameObject::eState::Active)
				continue;

			gameObj->Render();
		}
	}

	void Camera::RenderTransparent()
	{
		for (GameObject* gameObj : mTransparentGameObjects)
		{
			if (gameObj == nullptr)
				continue;
			if (gameObj->GetState()
				!= GameObject::eState::Active)
				continue;

			gameObj->Render();
		}
	}

	void Camera::EnableDepthStencilState()
	{
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState
			= renderer::depthStencilStates[(UINT)eDSType::Less];
		GetDevice()->BindDepthStencilState(dsState.Get());
	}

	// z값 정렬을 할 때, 
	void Camera::DisableDepthStencilState()
	{
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState
			= renderer::depthStencilStates[(UINT)eDSType::None];
		GetDevice()->BindDepthStencilState(dsState.Get());
	}
	void Camera::UpdateZoom()
	{
		if (mIsZooming)
		{
			mCurrentSize = mCurrentSize + (mTargetSize - mCurrentSize) * mLerpSpeed;

			// 확대/축소가 거의 완료되었을 때
			if (abs(mCurrentSize - mTargetSize) < 0.01f)
			{
				mCurrentSize = mTargetSize;
				mIsZooming = false;
			}

			mSize = mCurrentSize; // 현재 크기를 카메라 크기에 반영
		}
	}
}
