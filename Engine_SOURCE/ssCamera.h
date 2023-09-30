#pragma once
#include "ssComponent.h"
#include "ssGraphics.h"


// ī�޶� ������Ʈ�� ����İ� ��������� ���� �ִ�
// * ���� ����� Transform ������Ʈ�� ���� �ִ�.
// �������� ī�޶� ���� �̷�����. 
namespace ss
{
	class Camera : public Component
	{
	public:
		enum class eProjectionType
		{
			Perpective,
			OrthoGraphic,
			None,
		};
		

	public:
		Camera();
		~Camera();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		// === ����� ����
		bool		CreateViewMatrix();
		static void SetGpuViewMatrix(Matrix view) { View = view; }
		static Matrix GetGpuViewMatrix() { return View; }
	
		// === ���� ��� ����
		bool		  CreateProjectionMatrix(eProjectionType type); // ���� ����� 2������ ������ ������ enum���� ���ڷ� ����
		static void   SetGpuProjectionMatrix(Matrix projection) { Projection = projection; }
		static Matrix GetGpuProjectionMatrix() { return Projection; }


		// ==== ������ ����� ī�޶�� �ٲٸ鼭 �߰��� �Լ��� ====
		void RegisterCameraInRenderer();

		// ī�޶� Ư�� ���̾�� ���߰ų�, Ȥ�� �����ϴ� ���
	// (��游 ���߰� �ؼ� ī�޶� ���� ����� ���� ���� ȿ���� �� �� ����)
		void TurnLayerMask(eLayerType type, bool enable = true);
		void EnableLayerMasks() { mLayerMask.set(); }		// ��� ���̾� ���߱�
		void DisableLayerMasks() { mLayerMask.reset(); }	// ��� ���̾� ���� 



		// --- �з� -> ī�޶� ���� ������ 

		// Add���ӿ�����Ʈ �Լ��� ��ü�� �߰��� �� �� ��ü�� �������� ��������
		// �������� �������� ������ �ʰ� �׳� �ְ� �ȴ�. ������ �������� �� ��
		// ��ü�� ���¸� ����ؾ��ϴµ�, �̸� ���� Sort�Լ����� ��ü�� �� �з��Ѵ�.
		// ��ü�� ���¿� ���� �з��� ��ü���� �� Render �Լ� �ȿ��� �������ȴ�.
		// (Render��Ŀ� ���� vector�� ���ӿ�����Ʈ���� ���� ���� �����ؼ� ���� �ֱ� ����)
		void AlphaSortGameObjects();
		void ZSortTransparencyGameObjects();
		void DivideAlphaBlendGameObjects(const std::vector<GameObject*> gameObjs);


		void RenderOpaque();		 // �������� ��ü ������ 
		void RenderCutOut();		 // �������� ��ü ������
		void RenderTransparent();	 // ������ ��ü ������

		void EnableDepthStencilState();
		void DisableDepthStencilState();

		// == ����� ���� ���� �Լ��� �̷��� ������ ���� ���� (�������� �ٸ�)
		float GetSize() { return mSize; }
		Matrix& GetViewMatrix() { return mView; }
		Matrix& GetProjectionMatrix() { return mProjection; }

		// �� ������ ���������� 1���� ����� ����� ���� ���
		void SetSize(float size) { mSize = size; }

		// �� ������ ī�޶� ����� Ȯ�볪 ����� �� �ε巯�� �������� ���ؼ� �Ʒ� �Լ� ����� ��
		void SetTargetSize(float newSize)
		{
			mTargetSize = newSize;
			mIsZooming = true;
		}

		void SetLerpSpeed(float speed) { mLerpSpeed = speed; }
		void UpdateZoom();

	private:
		// static���� ������ ������, �ش� ����� Transform�� ��������
	// ���� ��İ� ���� �� ����ؾߵǱ� �����̴�. 
		static Matrix View;
		static Matrix Projection;

		// ī�޶� ���� �� ���� ���� ���� ����� ���������ϹǷ� ���� ���� 
		Matrix mView;
		Matrix mProjection;

		// ���� ���� 
		eProjectionType mType;
		float mAspectRatio; // ��Ⱦ��
		float mNear;		// ī�޶�� ��ü���� �Ÿ�(
		float mFar;
		float mSize;		// ī�޶��� scale���̴�. Ŭ���� ī�޶� �־�����. OrthoGraphic�� �� ���


		// ������ ���� 
		float mCurrentSize;		// ī�޶��� scale���̴�. Ŭ���� ī�޶� �־�����. OrthoGraphic�� �� ���
		float mTargetSize;
		float mLerpSpeed;
		bool mIsZooming;

		// == �������� ���� ���̾� ����

		// ���̾ ���������� ���� üũ�� ���ִ� �뵵�� 0�� 1�� ������ �Ǽ�
		// vector�� �ƴ� bitset�� ����Ѵ�.
		std::bitset<(UINT)eLayerType::End> mLayerMask;
		std::vector<GameObject*> mOpaqueGameObjects;	// ������ ��ü�� �з��� �͵��� �����
		std::vector<GameObject*> mCutOutGameObjects;	// �������� ��ü �з��Ǿ�����
		std::vector<GameObject*> mTransparentGameObjects; // ������ ��ü �з��Ǿ� ���� 
	};
}
