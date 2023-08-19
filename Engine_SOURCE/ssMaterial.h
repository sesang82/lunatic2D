#pragma once
#include "ssResource.h"
#include "ssShader.h"
#include "ssTexture.h"

namespace ss::graphics
{

	// 유니티처럼 Material 안에 쉐이더와 텍스처가 담기도록 해둠
   // Material => 텍스처를 어떻게 입혀줄 것인지에 대한 정보가 담긴 것 
	class Material : public Resource
	{
	public:
		Material();
		~Material();

		virtual HRESULT Load(const std::wstring& path) override;

		void Binds(); // 텍스처나 쉐이더가 있을 때만 Bind 해주게 함
		void Binds(UINT slotNum);

		void Clear(); //  바인딩해줬던 Material을 해제하는 함수 

		void SetShader(std::shared_ptr<Shader> shader) { mShader = shader; }
		void SetTexture(std::shared_ptr<Texture> texture) { mTexture = texture; }

		//void SetTexture(std::shared_ptr<Texture> texture, UINT slotNum)
		//{ 
		//	mTexture = texture;
		//	Binds(slotNum);
		//
		//}
		std::shared_ptr<Texture> GetTexture() { return mTexture; }
		
		// == 렌더링 관련
		void SetRenderingMode(eRenderingMode mode) { mMode = mode; }
		eRenderingMode GetRenderingMode() { return mMode; } // * 렌더러에 이미지 잘 로딩하고 있는게 맞는지 확인하기


	private:
		std::shared_ptr<Shader> mShader;
		std::shared_ptr<Texture> mTexture;

		// renderer에서 텍스처의 렌더링 모드를 지정해줄 수 있다.
	   // enum문 순서대로 렌더링되기 때문에, 물체가 z값이 같아도
	   // 렌더링 모드가 어떠냐에 따라 출력 순서가 달라질 것 
	   // (우선순위 : 1순위 - z값 // z값이 같다면??  2순위 - 렌더링 모드 ) 
		eRenderingMode mMode;
	};
}
