#include "GameScene.h"
#include "KeyInput.h"
#include "CharacterManager.h"
#include "AIManager.h"

CharacterManager* _characterMgr;

// コンストラクタ
GameScene::GameScene()
{
	_characterMgr = CharacterManager::Instance();

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
	if (_characterMgr->playerTurn) {
		KeyInput::Instance()->InputCalc(_characterMgr);

		Draw();

		_characterMgr->Update(xPos, yPos);
	}
	else {
		Draw();

		AIManager::Instance()->Update();

		_characterMgr->Update(AIManager::Instance()->x, AIManager::Instance()->y);
	}
	
}

// メインシーンの描画
void GameScene::Draw()
{
	// 座標更新
	if (_characterMgr->playerTurn) {
		xPos = KeyInput::Instance()->xPos;
		yPos = KeyInput::Instance()->yPos;
	}
	else {
		xPos = AIManager::Instance()->x;
		yPos = AIManager::Instance()->y;
	}

	// 描画
	DrawGraph(0.0f - KeyInput::Instance()->cameraPos.x, 0.0f - KeyInput::Instance()->cameraPos.y, stageImg, true);

	if (KeyInput::Instance()->Key[KEY_INPUT_SPACE] == 1) {
		_characterMgr->KeyCheck(xPos, yPos);

		if (isSelect == false) _characterMgr->DrawCheck(xPos, yPos);

		isSelect = _characterMgr->isSelect;
		KeyInput::Instance()->isSelect = isSelect;
	}

	if (isSelect == true && _characterMgr->attack == false) {
		_characterMgr->Draw();
		_characterMgr->GetMoveArrow(xPos, yPos);
	}

	// カーソル表示
	DrawGraph(xPos, yPos, cursorImg, true);
}

