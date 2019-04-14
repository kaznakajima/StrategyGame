#pragma once
#include "BaseScene.h"

// ステージ画像とカーソル画像
#define FIELD_IMG "StageMap.png"
#define CURSOR_IMG "Cursor.png"
#define PLAYERTURN_IMG "PlayerTurn.png"
#define ENEMYTURN_IMG "EnemyTurn.png"
#define CLEAR_IMG "GameClear.png"
#define GAMEOVER_IMG "GameOver.png"

class GameScene : public BaseScene
{
public:
	// コンストラクタ
	GameScene();

	// 初期化
	void Initialize();
	// 終了
	void Finalize();
	// ファイル読み込み
	void LoadFile();
	// 読み込んだファイルの破棄
	void UnLoadFile();
	// ターン変更
	void TurnChange(bool playerTurn);
	//更新
	void Update();
	//描画
	void Draw();

private:
	// キー入力処理
	void KeyEvent();
	// ゲーム終了
	void GameEnd(bool isClear);

	// ステージ画像
	int stageImg;
	// カーソル画像
	int cursorImg;
	// ターン変更用画像
	int turnChangeImg = 0;
	// アニメーション用変数
	float moveX = 672;
	float moveY = -48;

	// 現在の座標
	int xPos = 480, yPos = 240;
};