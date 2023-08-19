#pragma once
#include "ssResource.h"
#include "ssShader.h"
#include "ssTexture.h"

namespace ss::graphics
{

	// ����Ƽó�� Material �ȿ� ���̴��� �ؽ�ó�� ��⵵�� �ص�
   // Material => �ؽ�ó�� ��� ������ �������� ���� ������ ��� �� 
	class Material : public Resource
	{
	public:
		Material();
		~Material();

		virtual HRESULT Load(const std::wstring& path) override;

		void Binds(); // �ؽ�ó�� ���̴��� ���� ���� Bind ���ְ� ��
		void Binds(UINT slotNum);

		void Clear(); //  ���ε������ Material�� �����ϴ� �Լ� 

		void SetShader(std::shared_ptr<Shader> shader) { mShader = shader; }
		void SetTexture(std::shared_ptr<Texture> texture) { mTexture = texture; }

		//void SetTexture(std::shared_ptr<Texture> texture, UINT slotNum)
		//{ 
		//	mTexture = texture;
		//	Binds(slotNum);
		//
		//}
		std::shared_ptr<Texture> GetTexture() { return mTexture; }
		
		// == ������ ����
		void SetRenderingMode(eRenderingMode mode) { mMode = mode; }
		eRenderingMode GetRenderingMode() { return mMode; } // * �������� �̹��� �� �ε��ϰ� �ִ°� �´��� Ȯ���ϱ�


	private:
		std::shared_ptr<Shader> mShader;
		std::shared_ptr<Texture> mTexture;

		// renderer���� �ؽ�ó�� ������ ��带 �������� �� �ִ�.
	   // enum�� ������� �������Ǳ� ������, ��ü�� z���� ���Ƶ�
	   // ������ ��尡 ��Ŀ� ���� ��� ������ �޶��� �� 
	   // (�켱���� : 1���� - z�� // z���� ���ٸ�??  2���� - ������ ��� ) 
		eRenderingMode mMode;
	};
}
