#include "KeyInput.h"
#include "StageCreate.h"
#include "BaseScene.h"

// シーン管理クラスのインスタンス
Scene* c_Scene;

KeyInput* _keyInput;

// 情報の初期化
void Init()
{
	_keyInput = KeyInput::Instance();

	c_Scene = new Scene();
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

	StageCreate::Instance()->Open(STAGE_FILE_1);

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && _keyInput->UpdateInput() == 0)
	{
		c_Scene->Update();
	}

	// 終了
	DxLib_End();

	return 0;
}