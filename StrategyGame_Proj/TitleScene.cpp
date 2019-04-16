#include "TitleScene.h"

// ������
void TitleScene::Initialize()
{
	type = TITLESTATE::START;
	LoadFile();
}

// �ǂݍ���
void TitleScene::LoadFile()
{
	FileManager::Instance()->GetFileHandle(FIELD_IMG);
	FileManager::Instance()->GetFileHandle(START_TEXT);
	FileManager::Instance()->GetFileHandle(CLOSE_TEXT);
	FileManager::Instance()->GetFileHandle(CHOISE_IMG);
}

// �X�V
void TitleScene::Update()
{
	// �X�e�[�g�؂�ւ�
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_RIGHT) == 1) {
		/*_type++;
		if (_type > (int)ENEMY) _type = (int)ENEMY;
		type = (PLAYERTYPE)_type;*/
		xPos += 300;
		if (xPos > 360) xPos = 360;
		type = TITLESTATE::END;
	}
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_LEFT) == 1) {
		/*_type--;
		if (_type > (int)PLAYER) _type = (int)PLAYER;
		type = (PLAYERTYPE)_type; */
		xPos -= 300;
		if (xPos < 60) xPos = 60;
		type = TITLESTATE::START;
	}

	// �Q�[���X�^�[�g���Q�[���I��
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_SPACE) == 1) {
		switch (type) {
		// �Q�[���X�^�[�g
		case START:
			Scene::Instance()->ChangeFadeState(Scene::Instance()->FADEOUT, Scene::Instance()->GAME);
			break;
		// �Q�[���I��
		case END:
			DxLib_End();
			break;
		}

	}
}

// �`��
void TitleScene::Draw()
{
	DrawGraph(0, 0, FileManager::Instance()->GetFileHandle(FIELD_IMG), true);
	DrawGraph(100, 300, FileManager::Instance()->GetFileHandle(START_TEXT), true);
	DrawGraph(400, 300, FileManager::Instance()->GetFileHandle(CLOSE_TEXT), true);
	DrawGraph(xPos, 300, FileManager::Instance()->GetFileHandle(CHOISE_IMG), true);
}

// �I������
void TitleScene::Finalize()
{
}
