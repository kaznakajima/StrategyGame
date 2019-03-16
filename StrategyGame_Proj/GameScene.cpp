#include "GameScene.h"
#include "KeyInput.h"
#include "CharacterManager.h"
#include "AIManager.h"

CharacterManager* _characterMgr;

// �R���X�g���N�^
GameScene::GameScene()
{
	_characterMgr = new CharacterManager();

	Initialize();

	_characterMgr->Initialize();
}

// ������
void GameScene::Initialize()
{
	// �摜�̓ǂݍ���
	stageImg = LoadGraph(FIELD_IMG);
	cursorImg = LoadGraph(CURSOR_IMG);
}

void GameScene::LoadFile()
{

}

void GameScene::UnLoadFile()
{

}

// �X�V
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
	}
	
}

// ���C���V�[���̕`��
void GameScene::Draw()
{
	// ���W�X�V
	xPos = KeyInput::Instance()->xPos;
	yPos = KeyInput::Instance()->yPos;

	// �`��
	DrawGraph(0 - KeyInput::Instance()->cameraPos.x, 0 - KeyInput::Instance()->cameraPos.y, stageImg, true);

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

	// �J�[�\���\��
	DrawGraph(xPos, yPos, cursorImg, true);
}

