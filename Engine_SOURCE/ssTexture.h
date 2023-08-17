#pragma once
#include "ssResource.h"
#include "ssGraphics.h"

#include "../External/DirectXTex/Include/DirectXTex.h"
#include "../External/DirectXTex/Include/DirectXTex.inl"

// Tex라이브러리가 디버그버전, 릴리즈 버전으로 따로 나눠 빌드되어있기 때문에 아래처럼 해둠
#ifdef _DEBUG
#pragma comment(lib, "..\\External\\DirectXTex\\Lib\\Debug\\DirectXTex.lib")
#else
#pragma comment(lib, "..\\External\\DirectXTex\\Lib\\Release\\DirectXTex.lib")
#endif

namespace ss::graphics
{
	// DX에서 쓰는 Texture는 GPU에 바인딩할 때 쉐이더리소스view를 대신 사용한다.
	class Texture : public Resource
	{
	public:
		Texture();
		~Texture();

		virtual HRESULT Load(const std::wstring& path) override;
		void BindShader(eShaderStage stage, UINT startSlot);
		void Clear(); // GPU에 바인딩해줬던 텍스처를 해제하는 함수

	private:
		ScratchImage mImage;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture;

		// ID3D11Texture2D를 대신하는 역할을 하는 -view
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;

		D3D11_TEXTURE2D_DESC mDesc; // Create텍스처 함수를 쓰려면 desc를 채워줘야한다

	public:
		size_t GetWidth() { return mImage.GetMetadata().width; }
		size_t GetHeight() { return mImage.GetMetadata().height; }

	};
}
