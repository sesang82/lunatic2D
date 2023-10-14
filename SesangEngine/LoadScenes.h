#pragma once
#include "..\Engine_SOURCE\ssSceneManager.h"
#include "ssPlayScene.h"
#include "ssTitleScene.h"
#include "ssBoss1Scene.h"
#include "ssBoss2Scene.h"
#include "ssBoss4Scene.h"
#include "ssLibraryScene.h"
#include "ssEntranceScene.h"
#include "ssStage1Scene.h"
#include "ssIntroScene.h"
#include "ssEntryScene.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\x64\\Debug\\SesangEngine.lib")
#else
#pragma comment(lib, "..\\x64\\Release\\SesangEngine.lib")
#endif


namespace ss
{
	void InitializeScenes()
	{
		// ==각 씬의 Initialize는 Scene을 Create할 때 호출됨.
		// new연산자를 CreateScene 함수 안에 아예 넣었기 때문에 아래처럼 하면 됨 
		SceneManager::CreateScene<TitleScene>(L"TitleScene");
		SceneManager::CreateScene<LibraryScene>(L"LibraryScene");
		SceneManager::CreateScene<EntranceScene>(L"EntranceScene");
		SceneManager::CreateScene<EntryScene>(L"EntryScene");
		SceneManager::CreateScene<Stage1Scene>(L"Stage1Scene");
		SceneManager::CreateScene<Boss1Scene>(L"Boss1Scene");
		SceneManager::CreateScene<Boss2Scene>(L"Boss2Scene");
		//SceneManager::CreateScene<PlayScene>(L"PlayScene");


		// =======Load할 때는 On Enter와 On Exit만 호출됨.
		SceneManager::LoadScene(L"LibraryScene");
	}
}