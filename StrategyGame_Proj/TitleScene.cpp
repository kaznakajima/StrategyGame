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
	// �t�F�[�h���łȂ��Ƃ��̂ݎ��s
	if (Scene::Instance()->fadeState == Scene::Instance()->NONE) {
		// �X�e�[�g�؂�ւ�
		if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_RIGHT) == 1) {
			xPos += 350;
			if (xPos > 410) xPos = 410;
			type = TITLESTATE::END;
		}
		if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_LEFT) == 1) {
			xPos -= 350;
			if (xPos < 60) xPos = 60;
			type = TITLESTATE::START;
		}
	}

	// �Q�[���X�^�[�g���Q�[���I��
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_SPACE) == 1 && Scene::Instance()->fadeState == Scene::Instance()->NONE) {
		switch (type) {
		// �Q�[���X�^�[�g
		case START:
			Scene::Instance()->ChangeFadeState(Scene::Instance()->FADEOUT, Scene::Instance()->GAME);
			break;
		// �Q�[���I��
		case END:
			Scene::Instance()->isGame = false;
			break;
		}

	}
}

// �`��
void TitleScene::Draw()
{
	DrawGraph(0, 0, FileManager::Instance()->GetFileHandle(FIELD_IMG), true);
	DrawGraph(100, 370, FileManager::Instance()->GetFileHandle(START_TEXT), true);
	DrawGraph(450, 370, FileManager::Instance()->GetFileHandle(CLOSE_TEXT), true);
	DrawGraph(xPos, 370, FileManager::Instance()->GetFileHandle(CHOISE_IMG), true);
}

// �I������
void TitleScene::Finalize()
{
}
