#pragma once
#include "BaseScene.h"

#define FIELD_IMG "StageMap.png"
#define START_IMG "GameStart.png"
#define TITLE_LOGO "TitleLogo.png"

class TitleScene : public BaseScene
{
public:
	// 初期化
	void Initialize();
	// 終了
	void Finalize();
	// ファイル読み込み
	void LoadFile();
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
	int xPos = 60;
};
