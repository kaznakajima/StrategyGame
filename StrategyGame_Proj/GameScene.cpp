#include "GameScene.h"
#include "KeyInput.h"
#include "CharacterManager.h"
#include "AIManager.h"

CharacterManager* characterMgr;

// コンストラクタ
GameScene::GameScene()
{
	characterMgr = CharacterManager::Instance();

	Initialize();

	characterMgr->Initialize();
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
		DrawGraph((int)moveX, 0, turnChangeImg, true);
	}
	// エネミーターン
	else {
		DrawGraph((int)moveX, 0, turnChangeImg, true);
		
	}

	// 移動完了したら
	if (moveX < 0) {
		WaitTimer(1000);
		moveX = 672;
		characterMgr->turnAnim = false;
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
	if (characterMgr->playerTurn) {
		KeyInput::Instance()->InputCalc(characterMgr);

		Draw();

		characterMgr->Update(xPos, yPos);

		if (characterMgr->turnAnim) {
			TurnChange(characterMgr->playerTurn);
		}
	}
	else {
		Draw();

		AIManager::Instance()->Update();

		characterMgr->Update(AIManager::Instance()->x, AIManager::Instance()->y);

		if (characterMgr->turnAnim) {
			TurnChange(characterMgr->playerTurn);
		}
	}
}

// メインシーンの描画
void GameScene::Draw()
{
	// 座標更新
	if (characterMgr->playerTurn) {
		xPos = KeyInput::Instance()->xPos;
		yPos = KeyInput::Instance()->yPos;
	}
	else {
		xPos = AIManager::Instance()->x;
		yPos = AIManager::Instance()->y;
	}

	// 描画
	DrawGraph(0 - (int)KeyInput::Instance()->cameraPos.x, 0 - (int)KeyInput::Instance()->cameraPos.y, stageImg, true);

	if (KeyInput::Instance()->isSelect == true && characterMgr->attack == false) {
		characterMgr->Draw();
		characterMgr->GetMoveArrow(xPos, yPos);
	}

	// カーソル表示
	DrawGraph(xPos, yPos, cursorImg, true);
}

