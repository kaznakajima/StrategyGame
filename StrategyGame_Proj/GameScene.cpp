#include "GameScene.h"
#include "CharacterManager.h"
#include "AIManager.h"

// コンストラクタ
GameScene::GameScene()
{
	Initialize();

	CharacterManager::Instance()->Initialize();
	CharacterManager::Instance()->SetCameraOffset(-1, true);
}

// 初期化
void GameScene::Initialize()
{
	LoadFile();
}

void GameScene::LoadFile()
{
	// 画像の読み込み
	FileManager::Instance()->GetFileHandle(FIELD_IMG);
	FileManager::Instance()->GetFileHandle(CURSOR_IMG);
	turnChangeImg = FileManager::Instance()->GetFileHandle(PLAYERTURN_IMG);
}

void GameScene::UnLoadFile()
{

}

void GameScene::TurnChange(bool playerTurn)
{
	// 位置変更
	moveX -= 24;

	// プレイヤーターン
	if (playerTurn) {
		//DrawGraph((int)moveX, 0, turnChangeImg, true);
		DrawRotaGraph((int)moveX, 240, 1.0f, 0.0f, turnChangeImg, true);
	}
	// エネミーターン
	else {
		//DrawGraph((int)moveX, 0, turnChangeImg, true);
		DrawRotaGraph((int)moveX, 240, 1.0f, 0.0f, turnChangeImg, true);
	}

	// 移動完了したら
	if (moveX < 336) {

		AudioManager::Instance()->playSE(SE_TURNSTART);

		for (int volume = 0; volume < 256; volume += 3) {
			AudioManager::Instance()->VolumeFade(volume);
		}

		WaitTimer(1000);
		moveX = 672;
		CharacterManager::Instance()->turnAnim = false;

		if (playerTurn) { 
			turnChangeImg = FileManager::Instance()->GetFileHandle(ENEMYTURN_IMG);
			AudioManager::Instance()->playBGM(BGM_PLAYER);

			for (int volume = 256; volume >= 0; volume -= 3) {
				AudioManager::Instance()->VolumeFade(volume);
			}
		}
		// エネミーターン
		else {
			AIManager::Instance()->Play();
			turnChangeImg = FileManager::Instance()->GetFileHandle(PLAYERTURN_IMG);
			AudioManager::Instance()->playBGM(BGM_ENEMY);

			for (int volume = 256; volume >= 0; volume -= 3) {
				AudioManager::Instance()->VolumeFade(volume);
			}
		}
	}
}

// シーン全体の更新
void GameScene::Update()
{
	// 自分のターン
	if (CharacterManager::Instance()->playerTurn) {
		// 入力待機
		KeyEvent();

		CharacterManager::Instance()->Update(xPos, yPos);

		// どちらかの陣営がいないのならゲーム終了
		if (CharacterManager::Instance()->isGame == false &&  CharacterManager::Instance()->_playerList.empty()) {
			GameEnd(false);
			return;
		}
		else if (CharacterManager::Instance()->isGame == false && CharacterManager::Instance()->_enemyList.empty()) {
			GameEnd(true);
			return;
		}

		// ターン開始演出
		if (CharacterManager::Instance()->turnAnim) {
			TurnChange(CharacterManager::Instance()->playerTurn);
		}
	}
	// 敵のターン
	else {
		// AIの更新
		AIManager::Instance()->Update();

		CharacterManager::Instance()->Update(AIManager::Instance()->x, AIManager::Instance()->y);

		// どちらかの陣営がいないのならゲーム終了
		if (CharacterManager::Instance()->isGame == false && CharacterManager::Instance()->_playerList.empty()) {
			GameEnd(false);
			return;
		}
		else if (CharacterManager::Instance()->isGame == false && CharacterManager::Instance()->_enemyList.empty()) {
			GameEnd(true);
			return;
		}

		// ターン開始演出
		if (CharacterManager::Instance()->turnAnim) {
			TurnChange(CharacterManager::Instance()->playerTurn);
		}
	}
}

// メインシーンの描画
void GameScene::Draw()
{
	// 座標更新
	if (CharacterManager::Instance()->playerTurn) {
		/*xPos = KeyInput::Instance()->xPos;
		yPos = KeyInput::Instance()->yPos;*/
	}
	else {
		xPos = AIManager::Instance()->x;
		yPos = AIManager::Instance()->y;
	}

	// 描画
	DrawGraph(0 - (int)KeyInput::Instance()->cameraPos.x, 0 - (int)KeyInput::Instance()->cameraPos.y, FileManager::Instance()->GetFileHandle(FIELD_IMG), true);

	// キャラクター選択
	if (KeyInput::Instance()->isSelect == true && CharacterManager::Instance()->attack == false) {
		CharacterManager::Instance()->Draw();
		CharacterManager::Instance()->GetMoveArrow(xPos, yPos);
	}

	// カーソル表示
	DrawGraph(xPos, yPos, FileManager::Instance()->GetFileHandle(CURSOR_IMG), true);
}

// キー入力処理
void GameScene::KeyEvent()
{
	if (CharacterManager::Instance()->isMove || CharacterManager::Instance()->playerTurn == false || CharacterManager::Instance()->turnAnim) return;

	// 押されたキー入力によって処理を実行
	// 右キーが押されたら
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_RIGHT) == 1) {
		if (CharacterManager::Instance()->isDetail) return;

		// 右に移動
		xPos += CHIP_SIZE;

		// 画面端から出ないように制限
		if (xPos >= SCREEN_WIDTH - CHIP_SIZE) xPos = SCREEN_WIDTH - CHIP_SIZE;

		if (xPos > CHIP_SIZE * 10 && KeyInput::Instance()->cameraPos.x + SCREEN_WIDTH < STAGE1_WIDTH) {
			KeyInput::Instance()->cameraPos.x += CHIP_SIZE;
			CharacterManager::Instance()->SetCameraOffset(-1, true);
			xPos -= CHIP_SIZE;
		}

		if (KeyInput::Instance()->isSelect) CharacterManager::Instance()->GetMoveCount(xPos, yPos);
	}
	// 下キーが押されたら
	else if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_DOWN) == 1) {
		if (CharacterManager::Instance()->isDetail) {
			CharacterManager::Instance()->ChangeDetailCharacter(CharacterManager::Instance()->checkCharacter, 1);
			xPos = CharacterManager::Instance()->checkCharacter->myStatus->xPos;
			yPos = CharacterManager::Instance()->checkCharacter->myStatus->yPos;
			return;
		}

		// 下に移動
		yPos += CHIP_SIZE;

		if (yPos >= SCREEN_HEIGHT - CHIP_SIZE) yPos = SCREEN_HEIGHT - CHIP_SIZE;

		if (KeyInput::Instance()->isSelect) CharacterManager::Instance()->GetMoveCount(xPos, yPos);
	}
	// 左キーが押されたら
	else if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_LEFT) == 1) {
		if (CharacterManager::Instance()->isDetail) return;

		// 左に移動
		xPos -= CHIP_SIZE;

		if (xPos <= 0) xPos = 0;

		if (xPos < CHIP_SIZE * 3 && KeyInput::Instance()->cameraPos.x > 0) {
			KeyInput::Instance()->cameraPos.x -= CHIP_SIZE;
			CharacterManager::Instance()->SetCameraOffset(1, true);
			xPos += CHIP_SIZE;
		}

		if (KeyInput::Instance()->isSelect) CharacterManager::Instance()->GetMoveCount(xPos, yPos);
	}
	// 上キーが押されたら
	else if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_UP) == 1) {
		if (CharacterManager::Instance()->isDetail) {
			CharacterManager::Instance()->ChangeDetailCharacter(CharacterManager::Instance()->checkCharacter, -1);
			xPos = CharacterManager::Instance()->checkCharacter->myStatus->xPos;
			yPos = CharacterManager::Instance()->checkCharacter->myStatus->yPos;
			return;
		}

		// 上に移動
		yPos -= CHIP_SIZE;

		if (yPos <= 0) yPos = 0;

		if (KeyInput::Instance()->isSelect) CharacterManager::Instance()->GetMoveCount(xPos, yPos);
	}

	// 決定ボタン
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_SPACE) == 1) {
		// 詳細表示中はリターン
		if (CharacterManager::Instance()->isDetail) return;

		// 入力検知
		CharacterManager::Instance()->KeyCheck(xPos, yPos);

		if (KeyInput::Instance()->isSelect == false) CharacterManager::Instance()->DrawCheck(xPos, yPos);

		KeyInput::Instance()->isSelect = CharacterManager::Instance()->isSelect;
	}

	// 詳細表示ボタン
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_TAB) == 1) {
		CharacterManager::Instance()->CheckDetail(xPos, yPos);
	}
}

// ゲーム終了
void GameScene::GameEnd(bool isClear)
{
	// 位置変更
	if(moveY < 180) moveY += 24;

	// クリア表示
	if (isClear) {
		DrawGraph(0, (int)moveY, FileManager::Instance()->GetFileHandle(CLEAR_IMG), true);
	}
	// ゲームオーバー
	else {
		DrawGraph(0, (int)moveY, FileManager::Instance()->GetFileHandle(GAMEOVER_IMG), true);
	}
}

void GameScene::Finalize()
{
	AIManager::Instance()->Finalize();
	CharacterManager::Instance()->Finalize();
}

