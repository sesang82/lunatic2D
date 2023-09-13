#include "ssConstantBuffer.h"
#include "ssGraphicDevice_Dx11.h"

namespace ss::graphics
{



	ConstantBuffer::ConstantBuffer(const eCBType type)
		: GpuBuffer()
		, mType(type)
	{

	}

	ConstantBuffer::~ConstantBuffer()
	{
	}

	bool ConstantBuffer::Create(size_t size)
	{
		desc.ByteWidth = size;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ss::graphics::GetDevice()->CreateBuffer(buffer.GetAddressOf(), &desc, nullptr);

		return false;
	}

	void ConstantBuffer::SetData(void* data)
	{
		ss::graphics::GetDevice()->SetConstantBuffer(buffer.Get(), data, desc.ByteWidth);
	}

	void ConstantBuffer::Bind(eShaderStage stage)
	{
		ss::graphics::GetDevice()->BindConstantBuffer(stage, mType, buffer.Get());
	}

	void ConstantBuffer::Clear()
	{
		GetDevice()->ClearConstantBuffer(buffer.Get(), desc.ByteWidth);

		UINT stageCount = static_cast<UINT>(eShaderStage::Count);
		for (UINT stage = 0; stage < stageCount; ++stage)
		{
			GetDevice()->BindConstantBuffer(static_cast<eShaderStage>(stage), mType, buffer.Get());
		}
	}

}
