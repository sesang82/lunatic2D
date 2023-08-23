#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "ssEnums.h"
#include "ssMath.h"

// === 16바이트 메모리 크기를 가진 struct를 만들어주는 매크로 
// 이걸 이용해서 상수 버퍼의 이름을 지정해주고, hlsl 레지스터 번호만 기재해주면 됨
#define CB_GETBINDSLOT(name) __CBUFFERBINDSLOT__##name##__
#define CBUFFER(name, slot) static const int CB_GETBINDSLOT(name) = slot; struct alignas(16) name 

// define으로 해둔 거 옆에는 slot번호이므로 hlsl에 똑같은 숫자 기재해주면 됨
#define CBSLOT_TRANSFORM		0
//#define CBSLOT_SPRITE			1
#define CBSLOT_GRID				2
#define CBSLOT_COLLISION		3
#define CBSLOT_ANIMATION2D		4
#define CBSLOT_PROGRESSBAR		5


namespace ss::graphics
{
	enum class eShaderStage
	{
		VS,
		HS,
		DS,
		GS,
		PS,
		CS,
		End,
	};


	// enum 값은 따로 지정하지 않는 한, 기본 값이 0이다.
	// hlsl에서 쓰는 상수버퍼의 이름과 레지스터 번호는 여기 아래에 기재한 enum class의 값을 기준으로 똑같이 작성하자.
	// 상수 버퍼에 담아서 쓸 데이터 종류에 따라 아래 멤버 추가될 예정
	enum class eCBType
	{
		Transform,
		Material, // 이상 있을 시 원복 시키기 
		Grid,
		Collision,
		Animator,
		Progressbar,
		End,
	};

	enum class eSamplerType
	{
		Point, // 아무런 샘플링을 하지 않을 때 쓰는 샘플러(픽셀 아트는 이걸로 해야
		Anisotropic, // 비등방성 샘플링(픽셀의 계단 현상을 완화해주는 알고리즘)
		End,
	};


	// ==== 블렌딩 관련 옵션 ====
	enum class eRSType
	{
		SolidBack,
		SolidFront,
		SolidNone,
		WireframeNone,
		End,
	};

	enum class eDSType
	{
		Less, // 깊이 값이 가까울수록 우선 출력, 깊이 값 기록O
		Greater, // 깊이 값이 멀수록 우선 출력, 깊이 값 기록O
		NoWrite, //  깊이 테스트는 수행O, 깊이 버퍼에 깊이 값 기록x
		None, // 깊이값 관계 없이 모든 픽셀을 그리고 싶을 때. 깊이 테스트x, 깊이 값 기록x
		End,
	};

	enum class eBSType
	{
		Default,
		AlphaBlend,
		OneOne,
		End,
	};


	// 아래 순서대로 카메라가 해당 상태의 물체를 렌더링한다.
	// (현재 기본적으로 불투명한 물체부터 렌더링하도록 설정해둠(재질 클래스에서)
	// 렌더링 모드는 Material클래스에서 제어하도록 해둠(유니티 엔진 모방)
	enum class eRenderingMode
	{
		Opaque, // 불투명 (기본 상태)
		CutOut, // 반투명
		Transparent, // 투명
		End,
	};

	struct GpuBuffer
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		D3D11_BUFFER_DESC desc;

		GpuBuffer()
			: buffer(nullptr)
			, desc{}
		{

		}
		virtual ~GpuBuffer() = default;
	};

	struct DebugMesh
	{
		enums::eColliderType type;
		math::Vector3 position;
		math::Vector3 rotation;
		math::Vector3 scale;
		
		int ColCount;
		float radius;
		float duration;
		float time;

	};

}