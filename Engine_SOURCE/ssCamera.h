#pragma once
#include "ssComponent.h"
#include "ssGraphics.h"


// 카메라 컴포넌트는 뷰행렬과 투영행렬을 갖고 있다
// * 월드 행렬은 Transform 컴포넌트가 갖고 있다.
// 렌더링은 카메라에 의해 이뤄진다. 
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

		// === 뷰행렬 관련
		bool		CreateViewMatrix();
		static void SetGpuViewMatrix(Matrix view) { View = view; }
		static Matrix GetGpuViewMatrix() { return View; }
	
		// === 투영 행렬 관련
		bool		  CreateProjectionMatrix(eProjectionType type); // 투영 방식이 2가지로 나뉘기 때문에 enum문을 인자로 받음
		static void   SetGpuProjectionMatrix(Matrix projection) { Projection = projection; }
		static Matrix GetGpuProjectionMatrix() { return Projection; }


		// ==== 렌더링 방식을 카메라로 바꾸면서 추가된 함수들 ====
		void RegisterCameraInRenderer();

		// 카메라가 특정 레이어만을 비추거나, 혹은 무시하는 기능
	// (배경만 비추게 해서 카메라를 흔들면 배경이 흔드는 듯한 효과를 줄 수 있음)
		void TurnLayerMask(eLayerType type, bool enable = true);
		void EnableLayerMasks() { mLayerMask.set(); }		// 모든 레이어 비추기
		void DisableLayerMasks() { mLayerMask.reset(); }	// 모든 레이어 끄기 



		// --- 분류 -> 카메라에 의한 렌더링 

		// Add게임오브젝트 함수는 물체를 추가할 때 그 물체가 반투명한 상태인지
		// 불투명한 상태인지 따지지 않고 그냥 넣게 된다. 때문에 렌더링을 할 때
		// 물체의 상태를 고려해야하는데, 이를 위해 Sort함수에서 물체를 각 분류한다.
		// 물체의 상태에 따라 분류된 물체들은 각 Render 함수 안에서 렌더링된다.
		// (Render방식에 따라 vector로 게임오브젝트들을 각각 따로 저장해서 갖고 있기 때문)
		void AlphaSortGameObjects();
		void ZSortTransparencyGameObjects();
		void DivideAlphaBlendGameObjects(const std::vector<GameObject*> gameObjs);


		void RenderOpaque();		 // 불투명한 물체 렌더링 
		void RenderCutOut();		 // 반투명한 물체 렌더링
		void RenderTransparent();	 // 투명한 물체 렌더링

		void EnableDepthStencilState();
		void DisableDepthStencilState();

		// == 멤버를 갖고 오는 함수는 이렇게 밑으로 따로 빼둠 (위에꺼랑 다름)
		float GetSize() { return mSize; }
		Matrix& GetViewMatrix() { return mView; }
		Matrix& GetProjectionMatrix() { return mProjection; }

		// 한 씬에서 고정적으로 1개의 사이즈를 사용할 때만 사용
		void SetSize(float size) { mSize = size; }

		// 한 씬에서 카메라 사이즈를 확대나 축소할 때 부드러운 움직임을 위해선 아래 함수 사용할 것
		void SetTargetSize(float newSize)
		{
			mTargetSize = newSize;
			mIsZooming = true;
		}

		void SetLerpSpeed(float speed) { mLerpSpeed = speed; }
		void UpdateZoom();

	private:
		// static으로 선언한 이유는, 해당 행렬을 Transform에 가져가서
	// 월드 행렬과 곱할 때 사용해야되기 때문이다. 
		static Matrix View;
		static Matrix Projection;

		// 카메라가 여러 대 있을 때는 각자 행렬을 계산해줘야하므로 따로 빼둠 
		Matrix mView;
		Matrix mProjection;

		// 투영 관련 
		eProjectionType mType;
		float mAspectRatio; // 종횡비
		float mNear;		// 카메라와 물체와의 거리(
		float mFar;
		float mSize;		// 카메라의 scale값이다. 클수록 카메라가 멀어진다. OrthoGraphic일 때 사용


		// 사이즈 관련 
		float mCurrentSize;		// 카메라의 scale값이다. 클수록 카메라가 멀어진다. OrthoGraphic일 때 사용
		float mTargetSize;
		float mLerpSpeed;
		bool mIsZooming;

		// == 렌더링을 위한 레이어 관련

		// 레이어를 렌더링할지 말지 체크만 해주는 용도라 0과 1만 있으면 되서
		// vector가 아닌 bitset을 사용한다.
		std::bitset<(UINT)eLayerType::End> mLayerMask;
		std::vector<GameObject*> mOpaqueGameObjects;	// 투명한 물체로 분류된 것들이 저장됨
		std::vector<GameObject*> mCutOutGameObjects;	// 반투명한 물체 분류되어있음
		std::vector<GameObject*> mTransparentGameObjects; // 투명한 물체 분류되어 있음 
	};
}
