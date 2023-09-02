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

	// static ���� �ʱ�ȭ(������ķ� ���� �ʱ�ȭ�صд�)
	Matrix Camera::View = Matrix::Identity;
	Matrix Camera::Projection = Matrix::Identity;

	// ==== ������, ������ �༮���� z������ ���� �ʿ�
	// ���� �츮�� z���� Ŭ���� �ָ� �ִ� ������ �����ϰ� �����Ƿ�
	// �׿� �°� �Լ��� �������� 
	bool CompareZSort(GameObject* a, GameObject* b)
	{
		if (a->GetComponent<Transform>()->GetPosition().z
			<= b->GetComponent<Transform>()->GetPosition().z)
			return false;

		return true;
	}

	Camera::Camera()
		: Component(eComponentType::Camera)
		, mType(eProjectionType::OrthoGraphic) // 2D�� ������ ��������� �ϴ� Ortho�� ����
		, mAspectRatio(1.0f)
		, mNear(1.0f) // near�� far�� ���� 1�� 1000���� ���� ����
		, mFar(1000.0f)
		, mSize(2.f)
		, mLayerMask{}
		, mOpaqueGameObjects{}
		, mCutOutGameObjects{}
		, mTransparentGameObjects{}
		, mView(Matrix::Identity)
		, mProjection(Matrix::Identity)
	{
		// Ư���� ���̾ ������ �ƴϸ� �������� �����Ϸ��� �켱
	// � ���̾ �ִ��� �����ϹǷ� ī�޶� ��� ���̾ ���߰� �Ѵ�.
		EnableLayerMasks();

		mCurrentSize = mSize; // ���� �ÿ� ���� ũ�⸦ �ʱ�ȭ
		mTargetSize = mSize; // ��ǥ ũ�⵵ �ʱ� ũ��� �����ϰ� ����
		mLerpSpeed = 0.005f; // ���� �ӵ� ����. �� ���� �����Ͽ� Ȯ��/��� �ӵ��� ���� 
		mIsZooming = false; // Ȯ��/��� �������� ��Ÿ���� �÷���
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
		// ��ü�� ��ġ�� ��� ���ŵǾ���ϹǷ� ����İ� ������� ���� Update�� �־
	// ��� ����� �������ش�. 
		CreateViewMatrix();
		CreateProjectionMatrix(mType);

		// ī�޶� �������� ������ش�. (�̰� LateUpdate���� ���� ���� �ٽ��ѹ� ���ޱ�)
	// ���� �����Ҵ��� �ڿ������� �ȴٰ� �ϼ̴µ�....
		RegisterCameraInRenderer();

		//// ī�޶� ������Ʈ�� transform�� ���´�.
	//Transform* tr = GetOwner()->GetComponent<Transform>();


	//// ���࿡ ������ Ÿ���� �ִٸ�
	//if (mTarget)
	//{
	//	if (mTarget->GetState() == GameObject::eState::Dead)
	//		mTarget = nullptr;

	//	else if (mTarget->GetState() == GameObject::eState::Active)
	//	{
	//		// ���ӿ�����Ʈ�� transform�� ���´�.
	//		Vector3 ObjPos = mTarget->GetComponent<Transform>()->GetPosition();
	//		mCameraPos = ObjPos + mTargetOffset;
	//	}
	//}

		UpdateZoom(); // Ȯ��/��� ������Ʈ


	}

	void Camera::Render()
	{
		// Ʈ�������� ������ۿ���, �����ִ� ī�޶��� ����� �˾ƾ��ϹǷ� 
	// static������ ���� �Ű��ش�. 
		View = mView;
		Projection = mProjection;

		// Add���ӿ�����Ʈ �Լ��� �׳� �־��� �͵� ������ ��忡 ���� �з� �۾� 
		AlphaSortGameObjects();
		ZSortTransparencyGameObjects();  // ������ �ִ� ��ü���� Z������ ������ ������Ѵ�.

		// ==== ������ ��ü ������
		RenderOpaque(); // �������� ��ü���� �׳� �׸��� �ȴ�.

		// ==== ������, ���� ��ü ������
		DisableDepthStencilState(); // ���̰� ���x, ���̰� �׽�Ʈx
		RenderCutOut();	// ������ ��ü ������ 
		RenderTransparent(); // ���� ��ü ������
		EnableDepthStencilState(); // less���� �ٽ� �ٲ�(�� �׷��� ������ ��ü�� ����)

		// Q. ������, ���� ��ü�� �������� ���� �� ���̹��۸� ���ٸ��Ҵ� �ϴ°ɱ�?
		/*
		������ ��ü�� �ڿ� �ִ� ��ü�� �����־�� �մϴ�.

		�׷��� ���� ���Ⱑ Ȱ��ȭ�� ���¿��� ������ ��ü�� �������ϸ�,
		������ ��ü�� ���� ������ ���� ���ۿ� ��ϵǾ� �ڿ� �ִ� ��ü�� �������� �˴ϴ�.
		���� ������ ��ü�� �������� ���� ���� ���⸦ ��Ȱ��ȭ�Ͽ� �ڿ� �ִ� ��ü�� �������� �ʵ��� �մϴ�.

		�������� ��ü�� �ڿ� �ִ� ��ü�� ȥ�յǾ� ������ �մϴ�.

		�׷��� ���� �׽�Ʈ�� Ȱ��ȭ�� ���¿��� �������� ��ü�� �������ϸ�,
		�������� ��ü�� �ڿ� �ִ� ��ü�� ������ �������� �� �ֽ��ϴ�.
		���� �������� ��ü�� �������� ���� ���� �׽�Ʈ�� ��Ȱ��ȭ�Ͽ� �ڿ� �ִ� ��ü�� ȥ�յǵ��� �մϴ�.

		�̷��� �ϸ� �������ϰų� ������ ��ü�� �ùٸ��� �������� �� �ֽ��ϴ�.
		�׷��� �� ����� ��ü�� ������ ������ �ſ� �ΰ��մϴ�.

		�Ϲ������δ� �� ��ü���� ����� ��ü ������ �������ؾ� �մϴ�.
		�׷��� �Ϲ������δ� �������� ��ü�� ���� �������ϰ�, �� ������ �������ϰų� ������ ��ü�� �������մϴ�.
		�̷��� �ϸ� �������� ��ü�� ���� �׽�Ʈ�� ���� ���⸦ ����Ͽ� �ùٸ��� �������ǰ�,
		�������ϰų� ������ ��ü�� �ڿ� �ִ� ��ü�� �ùٸ��� ȥ�յ˴ϴ�.

		*/
	}

	// �� ����� ũ��� ����� ���� ������ �̵��� ȸ�� ��ĸ� �����ش�. 
	bool Camera::CreateViewMatrix()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();

		// View Translate Matrix (�̵� ���)
		mView = Matrix::Identity;
		mView *= Matrix::CreateTranslation(-pos);


		// View Rotation Matrix (ȸ�� ���)
		Vector3 up = tr->Up();
		Vector3 right = tr->Right();
		Vector3 foward = tr->Forward();

		Matrix viewRotate;
		viewRotate._11 = right.x;	viewRotate._12 = up.x;	viewRotate._13 = foward.x;
		viewRotate._21 = right.y;	viewRotate._22 = up.y;	viewRotate._23 = foward.y;
		viewRotate._31 = right.z;	viewRotate._32 = up.z;	viewRotate._33 = foward.z;

		// �̵������ �־���� �Ϳ� ȸ����ĵ� ���� ���ؼ� �־��ش�. 
		// ���������� ����� �ϼ�! 
		mView *= viewRotate;

		return true;
	}

	bool Camera::CreateProjectionMatrix(eProjectionType type)
	{
		RECT rect = {};
		GetClientRect(application.GetHwnd(), &rect); // ���� �ػ� ũ�⸦ �����´�.

		// ==== ��Ⱦ�� ���ϱ� ���� �ػ��� ���α��̿� ���α��̸� ���Ѵ�.
		float width = rect.right - rect.left;	// �ػ��� ���α���
		float height = rect.bottom - rect.top;	// �ػ��� ���α��� 
		mAspectRatio = width / height;; 		// ��Ⱦ�� ���Ѵ�. 


		// 2D �������ΰ�� OrthoGraphic ��������� ����Ѵ�.
		if (type == eProjectionType::OrthoGraphic)
		{
			//float OrthorGraphicRatio = mSize / 1000.0f;
			//width *= OrthorGraphicRatio;
			//height *= OrthorGraphicRatio;

			//mProjection = Matrix::CreateOrthographicLH(width, height, mNear, mFar);

			//// mSize���� ���� ī�޶��� ����Ʈ ũ�⸦ ���Ѵ�. 
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
		// ī�޶� vector�� ȣ���� �ڱ� �ڽ��� �־��ش�.
		renderer::cameras.push_back(this);
	}

	void Camera::TurnLayerMask(eLayerType type, bool enable)
	{
		// set�Լ��� std::bitset�� ��� �Լ���. ��������� true(=1)�� ��ȯ�Ѵ�.
		mLayerMask.set((UINT)type, enable);
	}

	void Camera::AlphaSortGameObjects()
	{

		// Add���ӿ�����Ʈ�� �׳� �־��� ��ü���� ��ü�� ���¿� �°� �� vector�� �з����ش�.
		// Opaque, CutOut, Transparent
		// (�ϴ� �츮�� �⺻������ �������� ��ü�� ���� ������ �ϵ��� �ص���) 
		// sort���ֱ� ���� �� vector�� ����ش�.
		mOpaqueGameObjects.clear();
		mCutOutGameObjects.clear();
		mTransparentGameObjects.clear();

		// == Alpha sorting

		// Ȱ��ȭ�� ���� �����´�.
		Scene* scene = SceneManager::GetActiveScene();
		for (size_t i = 0; i < (UINT)eLayerType::End; i++)
		{
			if (mLayerMask[i] == true)
			{
				// ���� �ִ� ���̾���� ���´�.
				Layer& layer = scene->GetLayer((eLayerType)i);

				// ���̾ �ִ� ���ӿ�����Ʈ���� gameObjs ���Ϳ� �־��ش�.
				const std::vector<GameObject*> gameObjs
					= layer.GetGameObjects();

				// Add���ӿ�����Ʈ�� �׳� �־��� ��ü���� ��ü�� ���¿� �°�
				// �� vector�� �з����ش�.
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
		// Add���ӿ�����Ʈ�� �׳� �־��� ��ü���� ��ü�� ���¿� �°� �� vector�� �з����ش�.
		// Opaque, CutOut, Transparent
		// (�ϴ� �츮�� �⺻������ �������� ��ü�� ���� ������ �ϵ��� �ص���)

		for (GameObject* obj : gameObjs)
		{
			// 1. �ϴ� �������� �ϴ� ���ӿ�����Ʈ���� Ȯ���Ѵ�.
			MeshRenderer* mr
				= obj->GetComponent<MeshRenderer>();

			// �޽��������� ���ٸ�
			if (mr == nullptr)
				continue;

			// �޽��������� �ִٸ�(�����κ��� ������ ��带 ����´�)
			std::shared_ptr<Material> mt = mr->GetMaterial();
			eRenderingMode mode = mt->GetRenderingMode();

			// �������� ��ü�� ���� ���� �ٸ� vector�� ��Ƽ�
			// ���� �������� �غ� ��ģ��.
			// (z���� ������� ���� ���·� ������ ��常 ����ؼ� �з�)
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

	// z�� ������ �� ��, 
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

			// Ȯ��/��Ұ� ���� �Ϸ�Ǿ��� ��
			if (abs(mCurrentSize - mTargetSize) < 0.01f)
			{
				mCurrentSize = mTargetSize;
				mIsZooming = false;
			}

			mSize = mCurrentSize; // ���� ũ�⸦ ī�޶� ũ�⿡ �ݿ�
		}
	}
}
