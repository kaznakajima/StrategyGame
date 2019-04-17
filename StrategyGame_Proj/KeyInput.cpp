#include "DxLib.h"
#include "KeyInput.h"

KeyInput::KeyInput()
{
	Initialize();
}

void KeyInput::Initialize()
{
	xPos = 480; yPos = 240;
	// 疑似的カメラの初期化
	cameraPos.x = 48; cameraPos.y = 0; cameraPos.z = -10.0f;
}

// キー入力の更新
int KeyInput::UpdateInput()
{
	// 現在のキー入力を格納
	char tmpKey[256];

	// 全てのキー入力状態を得る
	GetHitKeyStateAll(tmpKey);

	for (int i = 0; i < 256; i++)
	{
		// キーが押されたら
		if (tmpKey[i] != 0)
		{
			// 加算
			Key[i]++;
		}
		else
		{
			// 0にする
			Key[i] = 0;
		}
	}
	return 0;
}

// 入力しているキーを返す
int KeyInput::Keyboard_Get(int KeyCode)
{
	return Key[KeyCode];
}