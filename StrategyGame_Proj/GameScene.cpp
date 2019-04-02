#include "GameScene.h"
#include "FileManager.h"
#include "KeyInput.h"
#include "CharacterManager.h"
#include "AIManager.h"

// コンストラクタ
GameScene::GameScene()
{
	Initialize();

	CharacterManager::Instance()->Initialize();
}

// 初期化
void GameScene::Initialize()
{
	// 画像の読み込み
	stageImg = LoadGraph(FIELD_IMG);
	cursorImg = LoadGraph(CURSOR_IMG);
	turnChangeImg = LoadGraph(PLAYERTURN_IMG);
}

void GameScene::LoadFile()
{

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
		WaitTimer(1000);
		moveX = 672;
		CharacterManager::Instance()->turnAnim = false;
		if (playerTurn) turnChangeImg = LoadGraph(ENEMYTURN_IMG);
		// エネミーターン
		else {
			AIManager::Instance()->Play();
			turnChangeImg = LoadGraph(PLAYERTURN_IMG);
		}
	}
}

// シーン全体の更新
void GameScene::Update()
{
	// 自分のターン
	if (CharacterManager::Instance()->playerTurn) {
		// 入力待機
		KeyInput::Instance()->InputCalc(CharacterManager::Instance());

		Draw();

		CharacterManager::Instance()->Update(xPos, yPos);

		// ターン開始演出
		if (CharacterManager::Instance()->turnAnim) {
			TurnChange(CharacterManager::Instance()->playerTurn);
		}
	}
	// 敵のターン
	else {
		Draw();

		// AIの更新
		AIManager::Instance()->Update();

		CharacterManager::Instance()->Update(AIManager::Instance()->x, AIManager::Instance()->y);

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
		xPos = KeyInput::Instance()->xPos;
		yPos = KeyInput::Instance()->yPos;
	}
	else {
		xPos = AIManager::Instance()->x;
		yPos = AIManager::Instance()->y;
	}

	// 描画
	DrawGraph(0 - (int)KeyInput::Instance()->cameraPos.x, 0 - (int)KeyInput::Instance()->cameraPos.y, stageImg, true);

	// キャラクター選択
	if (KeyInput::Instance()->isSelect == true && CharacterManager::Instance()->attack == false) {
		CharacterManager::Instance()->Draw();
		CharacterManager::Instance()->GetMoveArrow(xPos, yPos);
	}

	// カーソル表示
	DrawGraph(xPos, yPos, cursorImg, true);
}

void GameScene::Finalize()
{
	AIManager::Instance()->Finalize();
	CharacterManager::Instance()->Finalize();
}

