#pragma once
#include "ssResource.h"
#include "ssGraphicDevice_Dx11.h"

namespace ss
{
	using namespace graphics;

	// 메쉬를 이루는 구성요소인 버텍스 버퍼와 인덱스 버퍼와 관련한 것도 여기에 묶음
	class Mesh : public Resource
	{
	public:
		Mesh();
		~Mesh();
		
		virtual HRESULT Load(const std::wstring& path) override;


		// DX함수 CreateBuffer함수의 실제 인자를 똑같이 받아서 내부에서 CreateBuffer함수를 호출하도록 한다.
		bool CreateVertexBuffer(void* data, UINT Count);
		bool CreateIndexBuffer(void* data, UINT Count);

		void BindBuffer(); //Buffer를 파이프라인에 묶어준다.
		void Render();

		UINT GetIndexCount() { return mIndexCount; }

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		
		// DESC에 작성한 내용을 토대로 한번 버퍼가 생성되면, DESC에 작성한 내용은 수정할 수 없다. (중요)
		D3D11_BUFFER_DESC mVBDesc;
		D3D11_BUFFER_DESC mIBDesc;

		UINT mIndexCount;
	};
}
