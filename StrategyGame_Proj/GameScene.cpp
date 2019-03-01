#include "GameScene.h"
#include "KeyInput.h"
#include "CharacterManager.h"

KeyInput* _key;
CharacterManager* _characterMgr;

// �R���X�g���N�^
GameScene::GameScene()
{
	// �C���X�^���X�̐���
	_key = KeyInput::Instance();
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
	_key->InputCalc(_characterMgr);

	Draw();

	_characterMgr->Update(xPos, yPos);
}

// ���C���V�[���̕`��
void GameScene::Draw()
{
	// ���W�X�V
	xPos = _key->xPos;
	yPos = _key->yPos;

	// �`��
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

	// �J�[�\���\��
	DrawGraph(xPos, yPos, cursorImg, true);
}

