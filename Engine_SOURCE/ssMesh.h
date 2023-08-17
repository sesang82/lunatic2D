#pragma once
#include "ssResource.h"
#include "ssGraphicDevice_Dx11.h"

namespace ss
{
	using namespace graphics;

	// �޽��� �̷�� ��������� ���ؽ� ���ۿ� �ε��� ���ۿ� ������ �͵� ���⿡ ����
	class Mesh : public Resource
	{
	public:
		Mesh();
		~Mesh();
		
		virtual HRESULT Load(const std::wstring& path) override;


		// DX�Լ� CreateBuffer�Լ��� ���� ���ڸ� �Ȱ��� �޾Ƽ� ���ο��� CreateBuffer�Լ��� ȣ���ϵ��� �Ѵ�.
		bool CreateVertexBuffer(void* data, UINT Count);
		bool CreateIndexBuffer(void* data, UINT Count);

		void BindBuffer(); //Buffer�� ���������ο� �����ش�.
		void Render();

		UINT GetIndexCount() { return mIndexCount; }

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		
		// DESC�� �ۼ��� ������ ���� �ѹ� ���۰� �����Ǹ�, DESC�� �ۼ��� ������ ������ �� ����. (�߿�)
		D3D11_BUFFER_DESC mVBDesc;
		D3D11_BUFFER_DESC mIBDesc;

		UINT mIndexCount;
	};
}
