#include "DxLib.h"
#include "KeyInput.h"

KeyInput::KeyInput()
{
	// 疑似的カメラの初期化
	xPos = 480; yPos = 240;
	cameraPos.x = 0; cameraPos.y = 0; cameraPos.z = -10.0f;
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

// 移動量計算
void KeyInput::InputCalc(CharacterManager* characterMgr)
{
	if (characterMgr->isMove || characterMgr->playerTurn == false || characterMgr->turnAnim) return;

	// 押されたキー入力によって処理を実行
	// 右キーが押されたら
	if (Key[KEY_INPUT_RIGHT] == 1) {
		if (characterMgr->isDetail) return;
		

		// 右に移動
		xPos += CHIP_SIZE;

		// 画面端から出ないように制限
		if (xPos >= SCREEN_WIDTH - CHIP_SIZE) xPos = SCREEN_WIDTH - CHIP_SIZE;

		if (xPos > CHIP_SIZE * 10 && cameraPos.x + SCREEN_WIDTH < STAGE1_WIDTH) {
			cameraPos.x += CHIP_SIZE;
			characterMgr->SetCameraOffset(-1, true);
		}

		if (isSelect) characterMgr->GetMoveCount(xPos, yPos);
	}
	// 下キーが押されたら
	else if (Key[KEY_INPUT_DOWN] == 1) {
		if (characterMgr->isDetail) {
			characterMgr->ChangeDetailCharacter(CharacterManager::Instance()->checkCharacter, 1);
			xPos = CharacterManager::Instance()->checkCharacter->myStatus->xPos;
			yPos = CharacterManager::Instance()->checkCharacter->myStatus->yPos;
			return;
		}

		// 下に移動
		yPos += CHIP_SIZE;

		if (yPos >= SCREEN_HEIGHT - CHIP_SIZE) yPos = SCREEN_HEIGHT - CHIP_SIZE;

		if (isSelect) characterMgr->GetMoveCount(xPos, yPos);
	}
	// 左キーが押されたら
	else if (Key[KEY_INPUT_LEFT] == 1) {
		if (characterMgr->isDetail) return;

		// 左に移動
		xPos -= CHIP_SIZE;

		if (xPos <= 0) xPos = 0;

		if (xPos < CHIP_SIZE * 3 && cameraPos.x > 0) {
			cameraPos.x -= CHIP_SIZE;
			characterMgr->SetCameraOffset(1, true);
		}

		if (isSelect) characterMgr->GetMoveCount(xPos, yPos);
	}
	// 上キーが押されたら
	else if (Key[KEY_INPUT_UP] == 1) {
		if (characterMgr->isDetail) { 
			characterMgr->ChangeDetailCharacter(CharacterManager::Instance()->checkCharacter, -1); 
			xPos = CharacterManager::Instance()->checkCharacter->myStatus->xPos;
			yPos = CharacterManager::Instance()->checkCharacter->myStatus->yPos;
			return;
		}

		// 上に移動
		yPos -= CHIP_SIZE;

		if (yPos <= 0) yPos = 0;

		if (isSelect) characterMgr->GetMoveCount(xPos, yPos);
	}

	// 決定ボタン
	if (Key[KEY_INPUT_SPACE] == 1) {
		// 詳細表示中はリターン
		if (characterMgr->isDetail) return;

		// 入力検知
		characterMgr->KeyCheck(xPos, yPos);

		if (isSelect == false) characterMgr->DrawCheck(xPos, yPos);

		isSelect = characterMgr->isSelect;
	}

	// 詳細表示ボタン
	if (Key[KEY_INPUT_TAB] == 1) {
		characterMgr->CheckDetail(xPos, yPos);
	}
}