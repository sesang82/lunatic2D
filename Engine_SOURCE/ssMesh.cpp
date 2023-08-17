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
		// ====== ���ؽ� ���� �����(DESC->SUBRESOURCE->CREATE�Լ� ȣ��)
		mVBDesc.ByteWidth = sizeof(renderer::Vertex) * Count;
		mVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		mVBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		mVBDesc.CPUAccessFlags = 0; // NONE

		// SUBRESOURCE�� GPU�� ������ �����͸� �����ֱ�.
		D3D11_SUBRESOURCE_DATA sub = {};
		sub.pSysMem = data;

		// Create �Լ��� ȣ��ʰ� ���ÿ� GPU�޸𸮿� ���۸� ������
		// Create�Լ��� ��ȯ���� bool�̹Ƿ� �Ʒ�ó�� if������ ��� false�� true�� �޵��� ��
		if (!GetDevice()->CreateBuffer(mVertexBuffer.GetAddressOf(), &mVBDesc, &sub))
			return false;

		return true;
	}

	bool Mesh::CreateIndexBuffer(void* data, UINT Count)
	{
		// ==== �ε��� ���� �����
		// Q. �ε��� ���۶�? 
		// ���ؽ� ���ۿ� �־��� ������ �������� ��, �׷��� ������ �����ϴ� ���۴�.
		// ������ �׸� �� ������ ��ġ�� ���, 2���̳� �׷����� ��ȿ�����̴�.
		// 1���� ���ؽ����� ���� �ִ� ���� �뷮�� �����ϸ� ��û���� ũ��.
		// ����, 1���� vertex���� 1���� ���ؽ� ���̴� �Լ��� ȣ��ȴٴ� �͵� �����ؾߵȴ�.
		// ������ ������ �ߺ��ؼ� �׸��� �ʰ�, �ε��� ���۸� ���� �׷��� ������ �����ϴ� ���̴�.


		// ���ؽ� ���ۿʹ� �ٸ��� �׸��� ������ '������ ��'�� ��� �����̹Ƿ� sizeof���� UINT�� �޴´�.  
		// �ε��� ������ ByteWidth�� ������ ���� ���� �迭�� size�� ���ش�.
		mIndexCount = Count;
		mIBDesc.ByteWidth = sizeof(UINT) * Count;
		mIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		mIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT; // �ƹ��� �뵵�� �ƴ϶�� ������
		mIBDesc.CPUAccessFlags = 0; // NONE


		// 2. SUBRESOURCE�� GPU�� ������ �����͸� �����ֱ�.
		// ���۸� �ʱ�ȭ�� ���� �ش� ����ü�� pSysMem������ ä���ָ� �ȴ�.
		// CreateBuffer �Լ��� ȣ���ϸ� GPU���� ���۸� �����϶�� �����Ѵ�.
		// �׸��� ���ڷ� ���� pSysMem�� ä���� �����͸� �����ϰ� �ȴ�.
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
