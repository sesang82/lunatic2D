#include "ssApplication.h"
#include "ssInput.h"
#include "ssTime.h"
#include "ssRenderer.h"
#include "ssSceneManager.h"
#include "ssCollisionManager.h"
#include "../SesangEngine/ssGameState.h"
#include "ssFmod.h"


namespace ss
{
	Application::Application()
		: graphicDevice(nullptr)
		, mHwnd(NULL)
		, mWidth(-1)
		, mHeight(-1)
	{

	}

	Application::~Application()
	{
		GameState::DestroyInst();
	}

	void Application::Run()
	{
		Update();
		LateUpdate();
		Render();
		Destroy();
	}

	void Application::Initialize()
	{
		Time::Initiailize();
		Input::Initialize();
		Fmod::Initialize();

		renderer::Initialize();

		GameState::GetInst().AddState(L"Player", 100, 50);
	
		SceneManager::Initialize();
	}

	void Application::Update()
	{
		Time::Update();
		Input::Update();


		SceneManager::Update();
	}

	void Application::LateUpdate()
	{
		CollisionManager::Update(); // 쌤은 Update에 해두긴햇음
		SceneManager::LateUpdate();
	
	}

	void Application::Render()
	{
		Time::Render();

		graphicDevice->ClearTarget();
		graphicDevice->UpdateViewPort();
		
		//SceneManager::Render();
		renderer::Render();  // 카메라가 렌더링을 담당하게 함 
		//graphicDevice->Draw();

		
	}

	void Application::Destroy()
	{
		SceneManager::Destroy();
	}

	void Application::Present()
	{
		graphicDevice->Present();
	}

	void Application::SetWindow(HWND hwnd, UINT width, UINT height)
	{
		if (graphicDevice == nullptr)
		{
			mHwnd = hwnd;
			mWidth = width;
			mHeight = height;

			graphicDevice = std::make_unique<ss::graphics::GraphicDevice_Dx11>();
			ss::graphics::GetDevice() = graphicDevice.get();
		}

		RECT rt = { 0, 0, (LONG)width , (LONG)height };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
		SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
		ShowWindow(mHwnd, true);
		UpdateWindow(mHwnd);
	}

}
