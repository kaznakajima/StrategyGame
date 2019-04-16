#include "TitleScene.h"

// ������
void TitleScene::Initialize()
{
}

// �ǂݍ���
void TitleScene::LoadFile()
{
}

void TitleScene::UnLoadFile()
{
}

// �X�V
void TitleScene::Update()
{
	// �v���C���[�^�C�v�̕ύX
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_RIGHT) == 1) {
		/*_type++;
		if (_type > (int)ENEMY) _type = (int)ENEMY;
		type = (PLAYERTYPE)_type;*/
		xPos += 240;
		if (xPos > 480) xPos = 480;
	}
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_LEFT) == 1) {
		/*_type--;
		if (_type > (int)PLAYER) _type = (int)PLAYER;
		type = (PLAYERTYPE)_type; */
		xPos -= 240;
		if (xPos < 240) xPos = 240;
	}

	// ���j�b�g���̕ύX
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_UP) == 1) {
		switch (type) {
		case PLAYER:
			playerCount++;
			if (playerCount > 1) playerCount = 2;
			break;
		case ENEMY:
			enemyCount++;
			if (enemyCount > 6) enemyCount = 6;
			break;
		}

	}
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_DOWN) == 1) {
		switch (type) {
		case PLAYER:
			playerCount--;
			if (playerCount < 1) playerCount = 1;
			break;
		case ENEMY:
			enemyCount--;
			if (enemyCount < 1) enemyCount = 1;
			break;
		}
	}

	// �Q�[���X�^�[�g
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_SPACE) == 1) Scene::Instance()->ChangeFadeState(Scene::Instance()->FADEOUT, Scene::Instance()->GAME);
}

// �`��
void TitleScene::Draw()
{
	DrawGraph(0, 0, FileManager::Instance()->GetFileHandle(FIELD_IMG), true);
	DrawGraph(240, 240, FileManager::Instance()->GetFileHandle(START_TEXT), true);
	DrawGraph(480, 240, FileManager::Instance()->GetFileHandle(CLOSE_TEXT), true);
	DrawGraph(xPos, 240, FileManager::Instance()->GetFileHandle(CHOISE_IMG), true);
}

// �I������
void TitleScene::Finalize()
{
}
