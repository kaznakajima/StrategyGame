#include "GameScene.h"
#include "KeyInput.h"
#include "CharacterManager.h"

KeyInput* _key;
CharacterManager* _characterMgr;

// コンストラクタ
GameScene::GameScene()
{
	// インスタンスの生成
	_key = KeyInput::Instance();
	_characterMgr = new CharacterManager();

	Initialize();

	_characterMgr->Initialize();
}

// 初期化
void GameScene::Initialize()
{
	// 画像の読み込み
	stageImg = LoadGraph(FIELD_IMG);
	cursorImg = LoadGraph(CURSOR_IMG);
}

void GameScene::LoadFile()
{

}

void GameScene::UnLoadFile()
{

}

// 更新
void GameScene::Update()
{
	_key->InputCalc(_characterMgr);

	Draw();

	_characterMgr->Update(xPos, yPos);
}

// メインシーンの描画
void GameScene::Draw()
{
	// 座標更新
	xPos = _key->xPos;
	yPos = _key->yPos;

	// 描画
	DrawGraph(0 - _key->cameraPos.x, 0 - _key->cameraPos.y, stageImg, true);

	if (_key->Key[KEY_INPUT_SPACE] == 1) {
		_characterMgr->KeyCheck(xPos, yPos);

		if (isSelect == false) _characterMgr->DrawCheck(xPos, yPos);

		isSelect = _characterMgr->isSelect;
		_key->isSelect = isSelect;
	}

	if (isSelect == true && _characterMgr->attack == false) {
		_characterMgr->Draw();
		_characterMgr->GetMoveArrow(xPos, yPos);
	}

	// カーソル表示
	DrawGraph(xPos, yPos, cursorImg, true);
}

