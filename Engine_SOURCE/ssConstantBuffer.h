#pragma once
#include "ssGraphics.h"

namespace ss::graphics
{
	class ConstantBuffer : public GpuBuffer
	{
	public:
		ConstantBuffer(const eCBType type);
		~ConstantBuffer();

		bool Create(size_t size);
		void SetData(void* data);
		void Bind(eShaderStage stage);
		void Clear();

	private:
		const eCBType mType;
	};
}
