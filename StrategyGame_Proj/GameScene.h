#pragma once
#include "BaseScene.h"

// ステージ画像とカーソル画像
#define FIELD_IMG "Resources\\image\\Field.png"
#define CURSOR_IMG "Resources\\image\\Cursor.png"
#define PLAYERTURN_IMG "Resources\\image\\TurnStart_P.png"
#define ENEMYTURN_IMG "Resources\\image\\TurnStart_E.png"

class GameScene : public BaseScene
{
public:
	// コンストラクタ
	GameScene();

	// 初期化
	void Initialize();
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
	// ステージ画像
	int stageImg;
	// カーソル画像
	int cursorImg;
	// ターン変更用画像
	int turnChangeImg = 0;
	// アニメーション用変数
	float moveX = 720;

	// 現在の座標
	int xPos = 480, yPos = 240;
};