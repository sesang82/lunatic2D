#pragma once
#include "SesangEngine.h"
#include "ssGraphicDevice_Dx11.h"
#include "ssMesh.h"
#include "ssShader.h"
#include "ssConstantBuffer.h"
#include "ssCamera.h"

using namespace ss::math;
using namespace ss::graphics;
namespace renderer
{
	// === 버텍스 버퍼 생성용
	// 1개의 버텍스가 가지고 있어야 할 정보를 struct로 기재
	struct Vertex
	{
		Vector3 pos;
		Vector4 color;
		Vector2 uv; // 텍스처 전용 좌표 (0에서 1 값 사용)
	};

	// ========== 상수버퍼용
// 매크로를 이용하여 16바이트 단위로 정렬된 구조체를 만든다.
// 여기서는 해당 구조체를 행렬을 담는 상수 버퍼로 이용
	CBUFFER(TransformCB, CBSLOT_TRANSFORM)
	{
		Matrix mWorld;
		Matrix mView;
		Matrix mProjection;
	};



	CBUFFER(GridCB, CBSLOT_GRID)
	{
		Vector4 CameraPosition;
		Vector2 CameraScale;
		Vector2 Resolution;
	};

	CBUFFER(ColCB, CBSLOT_COLLISION)
	{
		// 충돌 중인지 아닌지
		int c_ColCount;
		int c_Padding[3];
	};

	CBUFFER(AnimatorCB, CBSLOT_ANIMATION2D)
	{
		Vector2 spriteLeftTop;
		Vector2 spriteSize;
		Vector2 spriteOffset;
		Vector2 atlasSize;
		UINT animationType;
		UINT reverse;
		UINT padding[2];
	};

	CBUFFER(ProgressbarCB, CBSLOT_PROGRESSBAR)
	{
		float p_currentHP;
		float p_MaxHP;
		float p_CurrentSP;
		float p_MaxSP;

		UINT p_ProgressType;
		UINT p_TexType;
		UINT p_Padding[2];
	};




	// 자주 사용하는 것들을 extern으로 해둔다. 
	extern ss::graphics::ConstantBuffer* constantBuffer[(UINT)eCBType::End];

	// 샘플러와 블렌딩 관련 옵션들 모음 
	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState[];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[];

	// 렌더링은 카메라를 통해 이뤄지는데, 카메라는 여러개가 있을 수 있으므로
	// 벡터에다가 담아둠. 디버그용 메쉬도 마찬가지

	// 카메라의 render에서 카메라가 여러 대 있을 경우를 대비하여 clear를 해주는터라
	// 씬마다 1개씩 꼭 있어야할 메인 카메라가 없어져서 아래처럼 해둠
	extern ss::Camera* mainCamera; 
	extern std::vector<ss::Camera*> cameras;
	extern std::vector<DebugMesh> debugMeshs;

	void Initialize();
	void Render();
	void Release();

	void PushDebugMeshAttribute(DebugMesh& mesh);

	// ** 헤더파일만 보고 어떤 함수 있는지 잘 알아보게 하려고 원본코드에 없는 함수 추가해둠
	void LoadMesh();
	void SetupState();
	void LoadBuffer(); // 상수 버퍼 만드는 용도
	// 이 함수 안에서 쉐이더를 제작해주면 된다.
	void LoadShader();
	void LoadMaterial();
}
