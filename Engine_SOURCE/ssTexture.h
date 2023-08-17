#pragma once
#include "ssResource.h"
#include "ssGraphics.h"

#include "../External/DirectXTex/Include/DirectXTex.h"
#include "../External/DirectXTex/Include/DirectXTex.inl"

// Tex���̺귯���� ����׹���, ������ �������� ���� ���� ����Ǿ��ֱ� ������ �Ʒ�ó�� �ص�
#ifdef _DEBUG
#pragma comment(lib, "..\\External\\DirectXTex\\Lib\\Debug\\DirectXTex.lib")
#else
#pragma comment(lib, "..\\External\\DirectXTex\\Lib\\Release\\DirectXTex.lib")
#endif

namespace ss::graphics
{
	// DX���� ���� Texture�� GPU�� ���ε��� �� ���̴����ҽ�view�� ��� ����Ѵ�.
	class Texture : public Resource
	{
	public:
		Texture();
		~Texture();

		virtual HRESULT Load(const std::wstring& path) override;
		void BindShader(eShaderStage stage, UINT startSlot);
		void Clear(); // GPU�� ���ε������ �ؽ�ó�� �����ϴ� �Լ�

	private:
		ScratchImage mImage;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture;

		// ID3D11Texture2D�� ����ϴ� ������ �ϴ� -view
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;

		D3D11_TEXTURE2D_DESC mDesc; // Create�ؽ�ó �Լ��� ������ desc�� ä������Ѵ�

	public:
		size_t GetWidth() { return mImage.GetMetadata().width; }
		size_t GetHeight() { return mImage.GetMetadata().height; }

	};
}
