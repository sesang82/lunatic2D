#pragma once
#include "SesangEngine.h"
#include "ssGraphicDevice_Dx11.h"
#include "ssScene.h"

namespace ss
{
	class Application
	{
	public:
		Application();
		~Application();
		
		void Run();

		void Initialize();
		void Update();
		void LateUpdate();
		void Render();
		void Destroy();
		void Present();

		void SetWindow(HWND hwnd, UINT width, UINT height);

		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }
		HWND GetHwnd() { return mHwnd; }

	private:
		bool mbInitialize = false;
		// 오로지 한개의 객체만 만들수 있는 스마트 포인터
		std::unique_ptr<ss::graphics::GraphicDevice_Dx11> graphicDevice;

		HWND mHwnd;
		UINT mWidth;
		UINT mHeight;
	};
}
