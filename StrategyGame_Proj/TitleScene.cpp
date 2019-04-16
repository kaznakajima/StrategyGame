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
	// ステート切り替え
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_RIGHT) == 1) {
		/*_type++;
		if (_type > (int)ENEMY) _type = (int)ENEMY;
		type = (PLAYERTYPE)_type;*/
		xPos += 300;
		if (xPos > 360) xPos = 360;
		type = TITLESTATE::END;
	}
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_LEFT) == 1) {
		/*_type--;
		if (_type > (int)PLAYER) _type = (int)PLAYER;
		type = (PLAYERTYPE)_type; */
		xPos -= 300;
		if (xPos < 60) xPos = 60;
		type = TITLESTATE::START;
	}

	// ゲームスタートかゲーム終了
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_SPACE) == 1) {
		switch (type) {
		// ゲームスタート
		case START:
			Scene::Instance()->ChangeFadeState(Scene::Instance()->FADEOUT, Scene::Instance()->GAME);
			break;
		// ゲーム終了
		case END:
			DxLib_End();
			break;
		}

	}
}

// 描画
void TitleScene::Draw()
{
	DrawGraph(0, 0, FileManager::Instance()->GetFileHandle(FIELD_IMG), true);
	DrawGraph(100, 300, FileManager::Instance()->GetFileHandle(START_TEXT), true);
	DrawGraph(400, 300, FileManager::Instance()->GetFileHandle(CLOSE_TEXT), true);
	DrawGraph(xPos, 300, FileManager::Instance()->GetFileHandle(CHOISE_IMG), true);
}

// 終了処理
void TitleScene::Finalize()
{
}
