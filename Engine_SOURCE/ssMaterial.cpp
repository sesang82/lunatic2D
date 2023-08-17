#include "ssMaterial.h"


namespace ss::graphics
{
	Material::Material()
		: Resource(ss::enums::eResourceType::Material)
		, mShader(nullptr)
		, mTexture(nullptr)
		, mMode(eRenderingMode::Opaque)
	{

	}

	Material::~Material()
	{

	}

	HRESULT Material::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}

	void Material::Binds()
	{
		if (mTexture)
			mTexture->BindShader(eShaderStage::PS, 0);

		if (mShader)
			mShader->Binds();
	}

	// hp바 같은거 바인딩할 때 요거 쓰기 (쓸 슬롯은 hlsl에 작성한거 참고)
	void Material::Binds(UINT slotNum)
	{
		if (mTexture)
			mTexture->BindShader(eShaderStage::PS, slotNum);

		if (mShader)
			mShader->Binds();
	}

	void Material::Clear()
	{
		mTexture->Clear();
	}
}
