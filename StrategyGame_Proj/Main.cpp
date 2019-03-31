#include "KeyInput.h"
#include "StageCreate.h"
#include "BaseScene.h"
#include "FileManager.h"

// 情報の初期化
void Init()
{
	FileManager::Instance()->Initialize();
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ウインドウモードに変更
	ChangeWindowMode(TRUE);

	// 初期化
	DxLib_Init();
	SetGraphMode(672, 480, 32);
	SetWindowSize(672, 480);
	SetDrawScreen(DX_SCREEN_BACK);

	Init();

	// シーン管理クラスのインスタンス
	unique_ptr<Scene> c_Scene = make_unique<Scene>();

	StageCreate::Instance()->Open(STAGE_FILE_1);

	while (ProcessMessage() == 0 && ClearDrawScreen() == 0 && KeyInput::Instance()->UpdateInput() == 0)
	{
		c_Scene->Update(); 
		
		ScreenFlip();
	}

	c_Scene->Finalize();

	// 終了
	DxLib_End();

	return 0;
}