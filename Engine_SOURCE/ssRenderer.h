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
	// === ���ؽ� ���� ������
	// 1���� ���ؽ��� ������ �־�� �� ������ struct�� ����
	struct Vertex
	{
		Vector3 pos;
		Vector4 color;
		Vector2 uv; // �ؽ�ó ���� ��ǥ (0���� 1 �� ���)
	};

	// ========== ������ۿ�
// ��ũ�θ� �̿��Ͽ� 16����Ʈ ������ ���ĵ� ����ü�� �����.
// ���⼭�� �ش� ����ü�� ����� ��� ��� ���۷� �̿�
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
		// �浹 ������ �ƴ���
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




	// ���� ����ϴ� �͵��� extern���� �صд�. 
	extern ss::graphics::ConstantBuffer* constantBuffer[(UINT)eCBType::End];

	// ���÷��� ���� ���� �ɼǵ� ���� 
	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState[];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[];

	// �������� ī�޶� ���� �̷����µ�, ī�޶�� �������� ���� �� �����Ƿ�
	// ���Ϳ��ٰ� ��Ƶ�. ����׿� �޽��� ��������

	// ī�޶��� render���� ī�޶� ���� �� ���� ��츦 ����Ͽ� clear�� ���ִ��Ͷ�
	// ������ 1���� �� �־���� ���� ī�޶� �������� �Ʒ�ó�� �ص�
	extern ss::Camera* mainCamera; 
	extern std::vector<ss::Camera*> cameras;
	extern std::vector<DebugMesh> debugMeshs;

	void Initialize();
	void Render();
	void Release();

	void PushDebugMeshAttribute(DebugMesh& mesh);

	// ** ������ϸ� ���� � �Լ� �ִ��� �� �˾ƺ��� �Ϸ��� �����ڵ忡 ���� �Լ� �߰��ص�
	void LoadMesh();
	void SetupState();
	void LoadBuffer(); // ��� ���� ����� �뵵
	// �� �Լ� �ȿ��� ���̴��� �������ָ� �ȴ�.
	void LoadShader();
	void LoadMaterial();
}
