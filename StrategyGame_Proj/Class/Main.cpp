#include "KeyInput.h"
#include "StageCreate.h"
#include "BaseScene.h"
#include "FileManager.h"

// 情報の初期化
void Init()
{
	FileManager::Instance()->Initialize();
}

// アルファ値を返す
int FadeAlpha(int _alpha)
{
	switch (Scene::Instance()->fadeState) {
	case Scene::FADEIN:
		_alpha += 3;
		if (_alpha > 255) _alpha = 255;
		Scene::Instance()->SceneFade(_alpha, 255);
		break;
	case Scene::FADEOUT:
		_alpha -= 3;
		if (_alpha < 0) _alpha = 0;
		Scene::Instance()->SceneFade(_alpha, 0);
		break;
	default:
		break;
	}

	return _alpha;
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

	// フェード用アルファ値
	int alpha = 0;

	while (ProcessMessage() == 0 && ClearDrawScreen() == 0 && KeyInput::Instance()->UpdateInput() == 0)
	{
		// フェード後のアルファ値の取得
		if(Scene::Instance()->fadeState != Scene::NONE) alpha = FadeAlpha(alpha);

		// 重なり防止
		if (alpha != 0) {
			Scene::Instance()->Draw();

			Scene::Instance()->Update();
		}
		ScreenFlip();
	}

	Scene::Instance()->Finalize();

	// 終了
	DxLib_End();

	return 0;
}