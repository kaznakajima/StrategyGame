#include "GameScene.h"
#include "CharacterManager.h"
#include "AIManager.h"

// コンストラクタ
GameScene::GameScene()
{
	Initialize();
}

// 初期化
void GameScene::Initialize()
{
	KeyInput::Instance()->Initialize();
	CharacterManager::Instance()->Initialize();
	CharacterManager::Instance()->SetCameraOffset(-1, true);
	AudioManager::Instance()->playBGM(BGM_PLAYER);
	type = GAMESTATE::TITLE;
	LoadFile();
}

void GameScene::LoadFile()
{
	// 画像の読み込み
	FileManager::Instance()->GetFileHandle(FIELD_IMG);
	FileManager::Instance()->GetFileHandle(CURSOR_IMG);
	turnChangeImg = FileManager::Instance()->GetFileHandle(PLAYERTURN_IMG);
}

// ターン開始
void GameScene::TurnChange(bool playerTurn)
{
	if (Scene::Instance()->fadeState != Scene::Instance()->NONE) return;

	// 位置変更
	moveX -= 24;

	// プレイヤーターン
	if (playerTurn) {
		DrawRotaGraph((int)moveX, 240, 1.0f, 0.0f, turnChangeImg, true);
	}
	// エネミーターン
	else {
		DrawRotaGraph((int)moveX, 240, 1.0f, 0.0f, turnChangeImg, true);
	}

	// 移動完了したら
	if (moveX < 336) {

		AudioManager::Instance()->playSE(SE_TURNSTART);

		WaitTimer(1000);
		moveX = 672;
		CharacterManager::Instance()->turnAnim = false;

		if (playerTurn) { 
			turnChangeImg = FileManager::Instance()->GetFileHandle(ENEMYTURN_IMG);
			AudioManager::Instance()->playBGM(BGM_PLAYER);
		}
		// 敵ターン
		else {
			AIManager::Instance()->Play();
			turnChangeImg = FileManager::Instance()->GetFileHandle(PLAYERTURN_IMG);
			AudioManager::Instance()->playBGM(BGM_ENEMY);
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

		StageCreate::Instance()->DrawTerrainDetail(xPos + KeyInput::Instance()->cameraPos.x, yPos);

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
	// フェード中入力させない
	if (Scene::Instance()->fadeState != Scene::Instance()->NONE) return;
	if (CharacterManager::Instance()->isMove || CharacterManager::Instance()->turnAnim || CharacterManager::Instance()->isGame == false) return;

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
		// ユニットの詳細を表示
		if (CharacterManager::Instance()->isDetail) {
			CharacterManager::Instance()->ChangeDetailCharacter(CharacterManager::Instance()->checkCharacter, 1);
			// カーソル位置をユニットに合わせる
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
		// ユニットの詳細を表示
		if (CharacterManager::Instance()->isDetail) {
			CharacterManager::Instance()->ChangeDetailCharacter(CharacterManager::Instance()->checkCharacter, -1);
			// カーソル位置をユニットに合わせる
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
	if (moveY < 180) { moveY += 24; moveX = 60; }

	// クリア表示
	if (isClear) {
		DrawGraph(0, (int)moveY, FileManager::Instance()->GetFileHandle(CLEAR_IMG), true);
		DrawGraph(100, 550 - (int)moveY, FileManager::Instance()->GetFileHandle(TOTITLE_IMG), true);
		DrawGraph(450, 550 - (int)moveY, FileManager::Instance()->GetFileHandle(CLOSE_IMG), true);
		DrawGraph((int)moveX, 550 - (int)moveY, FileManager::Instance()->GetFileHandle(CHOISE_IMG), true);
	}
	// ゲームオーバー
	else {
		DrawGraph(0, (int)moveY, FileManager::Instance()->GetFileHandle(GAMEOVER_IMG), true);
		DrawGraph(100, 550 - (int)moveY, FileManager::Instance()->GetFileHandle(TOTITLE_IMG), true);
		DrawGraph(450, 550 - (int)moveY, FileManager::Instance()->GetFileHandle(CLOSE_IMG), true);
		DrawGraph((int)moveX, 550 - (int)moveY, FileManager::Instance()->GetFileHandle(CHOISE_IMG), true);
	}

	// 移動が完了したら
	if (moveY >= 180)
	{
		// フェード中は入力を受け付けない
		if (Scene::Instance()->fadeState == Scene::Instance()->NONE) {
			// ステート切り替え
			if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_RIGHT) == 1) {
				moveX += 350;
				if (moveX > 410) moveX = 410;
				type = GAMESTATE::END;
			}
			if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_LEFT) == 1) {
				moveX -= 350;
				if (moveX < 60) moveX = 60;
				type = GAMESTATE::TITLE;
			}
		}

		// タイトルに戻るかゲーム終了
		if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_SPACE) == 1) {
			switch (type) {
			case GAMESTATE::TITLE:
				Scene::Instance()->ChangeFadeState(Scene::Instance()->FADEOUT, Scene::Instance()->TITLE);
				Finalize();
				break;
			case GAMESTATE::END:
				Scene::Instance()->isGame = false;
				break;
			}
		}
	}
}

// 終了処理
void GameScene::Finalize()
{
	AIManager::Instance()->Finalize();
	CharacterManager::Instance()->Finalize();
}

