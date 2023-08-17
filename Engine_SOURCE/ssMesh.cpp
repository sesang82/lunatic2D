#include "ssMesh.h"
#include "ssRenderer.h"

namespace ss
{
	Mesh::Mesh()
		: Resource(enums::eResourceType::Mesh)
		, mVertexBuffer(nullptr)
		, mIndexBuffer(nullptr)
		, mVBDesc{}
		, mIBDesc{}
		, mIndexCount(0)
	{
	}

	Mesh::~Mesh()
	{
	}

	HRESULT Mesh::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}

	bool Mesh::CreateVertexBuffer(void* data, UINT Count)
	{
		// ====== 버텍스 버퍼 만들기(DESC->SUBRESOURCE->CREATE함수 호출)
		mVBDesc.ByteWidth = sizeof(renderer::Vertex) * Count;
		mVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		mVBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		mVBDesc.CPUAccessFlags = 0; // NONE

		// SUBRESOURCE로 GPU에 보내줄 데이터를 묶어주기.
		D3D11_SUBRESOURCE_DATA sub = {};
		sub.pSysMem = data;

		// Create 함수는 호출됨과 동시에 GPU메모리에 버퍼를 생성함
		// Create함수의 반환형이 bool이므로 아래처럼 if문으로 묶어서 false와 true를 받도록 함
		if (!GetDevice()->CreateBuffer(mVertexBuffer.GetAddressOf(), &mVBDesc, &sub))
			return false;

		return true;
	}

	bool Mesh::CreateIndexBuffer(void* data, UINT Count)
	{
		// ==== 인덱스 버퍼 만들기
		// Q. 인덱스 버퍼란? 
		// 버텍스 버퍼에 넣어준 정점을 렌더링할 때, 그려낼 순서를 지정하는 버퍼다.
		// 도형을 그릴 때 정점이 겹치는 경우, 2번이나 그려내면 비효율적이다.
		// 1개의 버텍스마다 갖고 있는 내부 용량을 생각하면 엄청나게 크다.
		// 또한, 1개의 vertex마다 1개의 버텍스 쉐이더 함수가 호출된다는 것도 생각해야된다.
		// 때문에 정점을 중복해서 그리지 않고, 인덱스 버퍼를 통해 그려낼 순서를 지정하는 것이다.


		// 버텍스 버퍼와는 다르게 그리는 순서인 '정수형 값'을 담는 버퍼이므로 sizeof값은 UINT로 받는다.  
		// 인덱스 버퍼의 ByteWidth는 정수형 값을 넣은 배열의 size로 해준다.
		mIndexCount = Count;
		mIBDesc.ByteWidth = sizeof(UINT) * Count;
		mIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		mIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT; // 아무론 용도도 아니라고 지정함
		mIBDesc.CPUAccessFlags = 0; // NONE


		// 2. SUBRESOURCE로 GPU에 보내줄 데이터를 묶어주기.
		// 버퍼를 초기화할 때는 해당 구조체의 pSysMem까지만 채워주면 된다.
		// CreateBuffer 함수를 호출하면 GPU에게 버퍼를 생성하라고 지시한다.
		// 그리고 인자로 받은 pSysMem에 채워준 데이터를 복사하게 된다.
		D3D11_SUBRESOURCE_DATA sub = {};
		sub.pSysMem = data;

		if (!GetDevice()->CreateBuffer(mIndexBuffer.GetAddressOf(), &mIBDesc, &sub))
			return false;

		return true;
	}

	void Mesh::BindBuffer()
	{
		UINT stride = sizeof(renderer::Vertex);
		UINT offset = 0;

		GetDevice()->BindVertexBuffer(0, mVertexBuffer.GetAddressOf(), &stride, &offset);
		GetDevice()->BindIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
	void Mesh::Render()
	{
		GetDevice()->DrawIndexed(mIndexCount, 0, 0);
	}
}
