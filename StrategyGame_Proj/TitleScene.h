#pragma once
#include "BaseScene.h"
#define FIELD_IMG "StageMap.png"
#define START_TEXT "GameStart.png"
#define CLOSE_TEXT "GameClose.png"
#define CHOISE_IMG "Choise.png"

class TitleScene : public BaseScene
{
public:
	// 初期化
	void Initialize();
	// 終了
	void Finalize();
	// ファイル読み込み
	void LoadFile();
	// 読み込んだファイルの破棄
	void UnLoadFile();
	//更新
	void Update();
	//描画
	void Draw();
private:
	// プレイヤータイプ
	enum TITLESTATE {
		START,
		END
	};
	TITLESTATE type;
	int _type = 0;

	// 座標
	int xPos = 90;
};
