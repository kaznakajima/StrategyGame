#include "TitleScene.h"

// 初期化
void TitleScene::Initialize()
{
	type = TITLESTATE::START;
	LoadFile();
}

// 読み込み
void TitleScene::LoadFile()
{
	FileManager::Instance()->GetFileHandle(FIELD_IMG);
	FileManager::Instance()->GetFileHandle(START_TEXT);
	FileManager::Instance()->GetFileHandle(CLOSE_TEXT);
	FileManager::Instance()->GetFileHandle(CHOISE_IMG);
}

// 更新
void TitleScene::Update()
{
	// フェード中でないときのみ実行
	if (Scene::Instance()->fadeState == Scene::Instance()->NONE) {
		// ステート切り替え
		if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_RIGHT) == 1) {
			xPos += 350;
			if (xPos > 410) xPos = 410;
			type = TITLESTATE::END;
		}
		if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_LEFT) == 1) {
			xPos -= 350;
			if (xPos < 60) xPos = 60;
			type = TITLESTATE::START;
		}
	}

	// ゲームスタートかゲーム終了
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_SPACE) == 1 && Scene::Instance()->fadeState == Scene::Instance()->NONE) {
		switch (type) {
		// ゲームスタート
		case START:
			Scene::Instance()->ChangeFadeState(Scene::Instance()->FADEOUT, Scene::Instance()->GAME);
			break;
		// ゲーム終了
		case END:
			Scene::Instance()->isGame = false;
			break;
		}

	}
}

// 描画
void TitleScene::Draw()
{
	DrawGraph(0, 0, FileManager::Instance()->GetFileHandle(FIELD_IMG), true);
	DrawGraph(100, 370, FileManager::Instance()->GetFileHandle(START_TEXT), true);
	DrawGraph(450, 370, FileManager::Instance()->GetFileHandle(CLOSE_TEXT), true);
	DrawGraph(xPos, 370, FileManager::Instance()->GetFileHandle(CHOISE_IMG), true);
}

// 終了処理
void TitleScene::Finalize()
{
}
