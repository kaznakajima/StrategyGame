#pragma once
#include "CharacterManager.h"
#include "StageCreate.h"

// キー入力クラス
class KeyInput : public Singleton<KeyInput>
{
public:
	KeyInput();

	// キーが押されているフレーム数を取得
	int Key[256];
	// x座標, y座標
	int xPos, yPos;
	// 疑似的カメラ位置
	VECTOR cameraPos;
	// 選択状態
	bool isSelect = false;

	// キー入力の更新
	int UpdateInput();
	// 移動量計算
	void InputCalc(CharacterManager* characterMgr);
};