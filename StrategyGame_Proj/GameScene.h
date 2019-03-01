#pragma once
#include "BaseScene.h"

// ステージ画像とカーソル画像
#define FIELD_IMG "Resources\\image\\Field.png"
#define CURSOR_IMG "Resources\\image\\Cursor.png"

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
	//更新
	void Update();
	//描画
	void Draw();

private:
	// 選択状態
	bool isSelect = false;
	// ステージ画像
	int stageImg;
	// カーソル画像
	int cursorImg;

	// 現在の座標
	int xPos = 480, yPos = 240;
};