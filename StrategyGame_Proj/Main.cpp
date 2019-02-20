#include "KeyInput.h"
#include "StageCreate.h"

// ステージ画像
int Handle;
// カーソル画像
int Cursor;

CharacterManager* characterMgr;
KeyInput* _keyInput;

// ユニットの選択状態
bool isSelect;

// 座標
int xPos = 480, yPos = 240;

// 情報の初期化
void Init()
{
	// キャラクターマネージャーのインスタンス
	characterMgr = new CharacterManager();

	_keyInput = KeyInput::Instance();

	Handle = LoadGraph("Field.png");
	Cursor = LoadGraph("Cursor.png");
}

// 描画メソッド
void gpDraw()
{
	// 座標更新
	xPos = _keyInput->xPos;
	yPos = _keyInput->yPos;

	// 描画
	DrawGraph(0 - _keyInput->cameraPos.x, 0 - _keyInput->cameraPos.y, Handle, true);

	if (_keyInput->Key[KEY_INPUT_SPACE] == 1) {
		characterMgr->KeyCheck(xPos, yPos);

		if (isSelect == false) characterMgr->DrawCheck(xPos, yPos);

		isSelect = characterMgr->isSelect;
		_keyInput->isSelect = isSelect;
	}

	if (isSelect == true && characterMgr->attack == false) {
		characterMgr->Draw();
		characterMgr->GetMoveArrow(xPos, yPos);
	}

	DrawGraph(xPos, yPos, Cursor, true);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ウインドウモードに変更
	ChangeWindowMode(TRUE);

	// 初期化
	DxLib_Init();
	SetGraphMode(672, 480, 32);
	SetWindowSize(672, 480);
	SetDrawScreen(DX_SCREEN_BACK);

	Init();

	StageCreate::Instance()->Open(STAGE_FILE_1);

	characterMgr->Initialize();

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && _keyInput->UpdateInput() == 0)
	{
		// 移動量計算
		_keyInput->InputCalc(characterMgr);

		// 描画
		gpDraw();

		characterMgr->GetAttackArea(xPos, yPos);

		characterMgr->Update();

		if (isSelect == false) characterMgr->CharacterMove(xPos, yPos);

		if (characterMgr->attack) characterMgr->Attack();
	}

	// 終了
	DxLib_End();

	return 0;
}