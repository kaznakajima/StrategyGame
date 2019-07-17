#include "GameScene.h"
#include "CharacterManager.h"
#include "AIManager.h"
#include <algorithm>

// �R���X�g���N�^
GameScene::GameScene()
{
	LoadFile();
	Initialize();
}

// ������
void GameScene::Initialize()
{
	KeyInput::Instance()->Initialize();
	CharacterManager::Instance()->Initialize();
	CharacterManager::Instance()->SetCameraOffset(-1, true);
	AudioManager::Instance()->playBGM(BGM_PLAYER);
	type = GAMESTATE::TITLE;
}

void GameScene::LoadFile()
{
	// �摜�̓ǂݍ���
	shared_ptr<DrawManager> field(new DrawParts(FIELD_IMG, 0));
	DrawManager::Instance()->AddDrawList(field);
	shared_ptr<DrawManager> cursor(new DrawParts(CURSOR_IMG, 1));
	DrawManager::Instance()->AddDrawList(cursor);
	shared_ptr<DrawManager> playerTurn(new DrawParts(PLAYERTURN_IMG, 2, 1.0f, 0.0f));
	playerTurn->isRemove = true;
	DrawManager::Instance()->AddDrawList(playerTurn);
	/*FileManager::Instance()->GetFileHandle(FIELD_IMG);
	FileManager::Instance()->GetFileHandle(CURSOR_IMG);
	turnChangeImg = FileManager::Instance()->GetFileHandle(PLAYERTURN_IMG);*/
}

// �^�[���J�n
void GameScene::TurnChange(bool playerTurn)
{
	if (Scene::Instance()->fadeState != Scene::Instance()->NONE) return;

	// �ʒu�ύX
	moveX -= 24;

	DrawManager::Instance()->GetDrawParts(PLAYERTURN_IMG)->isVisible = true;

	// �v���C���[�^�[��
	if (playerTurn) {
		DrawManager::Instance()->GetDrawParts(PLAYERTURN_IMG)->SetPosition((int)moveX, 240);
		//DrawRotaGraph((int)moveX, 240, 1.0f, 0.0f, turnChangeImg, true);
	}
	// �G�l�~�[�^�[��
	else {
		DrawManager::Instance()->GetDrawParts(PLAYERTURN_IMG)->SetPosition((int)moveX, 240);
		//DrawRotaGraph((int)moveX, 240, 1.0f, 0.0f, turnChangeImg, true);
	}

	// �ړ�����������
	if (moveX < 336) {

		AudioManager::Instance()->playSE(SE_TURNSTART);

		WaitTimer(1000);
		moveX = 672;
		DrawManager::Instance()->GetDrawParts(PLAYERTURN_IMG)->isVisible = false;
		CharacterManager::Instance()->turnAnim = false;

		if (playerTurn) { 
			//turnChangeImg = FileManager::Instance()->GetFileHandle(ENEMYTURN_IMG);
			AudioManager::Instance()->playBGM(BGM_PLAYER);
		}
		// �G�^�[��
		else {
			AIManager::Instance()->Play();
			//turnChangeImg = FileManager::Instance()->GetFileHandle(PLAYERTURN_IMG);
			AudioManager::Instance()->playBGM(BGM_ENEMY);
		}
	}
}

// �V�[���S�̂̍X�V
void GameScene::Update()
{
	// �����̃^�[��
	if (CharacterManager::Instance()->playerTurn) {
		// ���͑ҋ@
		KeyEvent();

		CharacterManager::Instance()->Update(xPos, yPos);

		StageCreate::Instance()->DrawTerrainDetail(xPos + KeyInput::Instance()->cameraPos.x, yPos);

		// �ǂ��炩�̐w�c�����Ȃ��̂Ȃ�Q�[���I��
		if (CharacterManager::Instance()->isGame == false &&  CharacterManager::Instance()->_playerList.empty()) {
			GameEnd(false);
			return;
		}
		else if (CharacterManager::Instance()->isGame == false && CharacterManager::Instance()->_enemyList.empty()) {
			GameEnd(true);
			return;
		}

		// �^�[���J�n���o
		if (CharacterManager::Instance()->turnAnim) {
			TurnChange(CharacterManager::Instance()->playerTurn);
		}
	}
	// �G�̃^�[��
	else {
		// AI�̍X�V
		AIManager::Instance()->Update();

		CharacterManager::Instance()->Update(AIManager::Instance()->x, AIManager::Instance()->y);

		// �ǂ��炩�̐w�c�����Ȃ��̂Ȃ�Q�[���I��
		if (CharacterManager::Instance()->isGame == false && CharacterManager::Instance()->_playerList.empty()) {
			GameEnd(false);
			return;
		}
		else if (CharacterManager::Instance()->isGame == false && CharacterManager::Instance()->_enemyList.empty()) {
			GameEnd(true);
			return;
		}

		// �^�[���J�n���o
		if (CharacterManager::Instance()->turnAnim) {
			TurnChange(CharacterManager::Instance()->playerTurn);
		}
	}
}

// ���C���V�[���̕`��
void GameScene::Draw()
{
	// �`��
	DrawManager::Instance()->GetDrawParts(FIELD_IMG)->SetPosition(0 - (int)KeyInput::Instance()->cameraPos.x, 0 - (int)KeyInput::Instance()->cameraPos.y);
	//DrawGraph(0 - (int)KeyInput::Instance()->cameraPos.x, 0 - (int)KeyInput::Instance()->cameraPos.y, FileManager::Instance()->GetFileHandle(FIELD_IMG), true);


	// �L�����N�^�[�I��
	if (KeyInput::Instance()->isSelect == true && CharacterManager::Instance()->attack == false) {
		CharacterManager::Instance()->Draw();
		CharacterManager::Instance()->GetMoveArrow(xPos, yPos);
	}

	// �J�[�\���\��
	DrawManager::Instance()->GetDrawParts(CURSOR_IMG)->SetPosition(xPos, yPos);
	//DrawGraph(xPos, yPos, FileManager::Instance()->GetFileHandle(CURSOR_IMG), true);

	for_each(DrawManager::Instance()->drawList.begin(), DrawManager::Instance()->drawList.end(), [](shared_ptr<DrawManager>& draw) {
		draw->Draw();
	});
}

// �L�[���͏���
void GameScene::KeyEvent()
{
	// �t�F�[�h�����͂����Ȃ�
	if (Scene::Instance()->fadeState != Scene::Instance()->NONE) return;
	if (CharacterManager::Instance()->isMove || CharacterManager::Instance()->turnAnim || CharacterManager::Instance()->isGame == false) return;

	// �����ꂽ�L�[���͂ɂ���ď��������s
	// �E�L�[�������ꂽ��
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_RIGHT) == 1) {
		if (CharacterManager::Instance()->isDetail) return;

		// �E�Ɉړ�
		xPos += CHIP_SIZE;

		// ��ʒ[����o�Ȃ��悤�ɐ���
		if (xPos >= SCREEN_WIDTH - CHIP_SIZE) xPos = SCREEN_WIDTH - CHIP_SIZE;

		if (xPos > CHIP_SIZE * 10 && KeyInput::Instance()->cameraPos.x + SCREEN_WIDTH < STAGE1_WIDTH) {
			KeyInput::Instance()->cameraPos.x += CHIP_SIZE;
			CharacterManager::Instance()->SetCameraOffset(-1, true);
			xPos -= CHIP_SIZE;
		}

		if (KeyInput::Instance()->isSelect) CharacterManager::Instance()->GetMoveCount(xPos, yPos);
	}
	// ���L�[�������ꂽ��
	else if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_DOWN) == 1) {
		// ���j�b�g�̏ڍׂ�\��
		if (CharacterManager::Instance()->isDetail) {
			CharacterManager::Instance()->ChangeDetailCharacter(CharacterManager::Instance()->checkCharacter, 1);
			// �J�[�\���ʒu�����j�b�g�ɍ��킹��
			xPos = CharacterManager::Instance()->checkCharacter->myStatus->xPos;
			yPos = CharacterManager::Instance()->checkCharacter->myStatus->yPos;
			return;
		}

		// ���Ɉړ�
		yPos += CHIP_SIZE;

		if (yPos >= SCREEN_HEIGHT - CHIP_SIZE) yPos = SCREEN_HEIGHT - CHIP_SIZE;

		if (KeyInput::Instance()->isSelect) CharacterManager::Instance()->GetMoveCount(xPos, yPos);
	}
	// ���L�[�������ꂽ��
	else if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_LEFT) == 1) {
		if (CharacterManager::Instance()->isDetail) return;

		// ���Ɉړ�
		xPos -= CHIP_SIZE;

		if (xPos <= 0) xPos = 0;

		if (xPos < CHIP_SIZE * 3 && KeyInput::Instance()->cameraPos.x > 0) {
			KeyInput::Instance()->cameraPos.x -= CHIP_SIZE;
			CharacterManager::Instance()->SetCameraOffset(1, true);
			xPos += CHIP_SIZE;
		}

		if (KeyInput::Instance()->isSelect) CharacterManager::Instance()->GetMoveCount(xPos, yPos);
	}
	// ��L�[�������ꂽ��
	else if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_UP) == 1) {
		// ���j�b�g�̏ڍׂ�\��
		if (CharacterManager::Instance()->isDetail) {
			CharacterManager::Instance()->ChangeDetailCharacter(CharacterManager::Instance()->checkCharacter, -1);
			// �J�[�\���ʒu�����j�b�g�ɍ��킹��
			xPos = CharacterManager::Instance()->checkCharacter->myStatus->xPos;
			yPos = CharacterManager::Instance()->checkCharacter->myStatus->yPos;
			return;
		}

		// ��Ɉړ�
		yPos -= CHIP_SIZE;

		if (yPos <= 0) yPos = 0;

		if (KeyInput::Instance()->isSelect) CharacterManager::Instance()->GetMoveCount(xPos, yPos);
	}

	// ����{�^��
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_SPACE) == 1) {
		// �ڍו\�����̓��^�[��
		if (CharacterManager::Instance()->isDetail) return;

		// ���͌��m
		CharacterManager::Instance()->KeyCheck(xPos, yPos);

		if (KeyInput::Instance()->isSelect == false) CharacterManager::Instance()->DrawCheck(xPos, yPos);

		KeyInput::Instance()->isSelect = CharacterManager::Instance()->isSelect;
	}

	// �ڍו\���{�^��
	if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_TAB) == 1) {
		CharacterManager::Instance()->CheckDetail(xPos, yPos);
	}
}

// �Q�[���I��
void GameScene::GameEnd(bool isClear)
{
	// �ʒu�ύX
	if (moveY < 180) { moveY += 24; moveX = 60; }

	// �N���A�\��
	if (isClear) {
		DrawGraph(0, (int)moveY, FileManager::Instance()->GetFileHandle(CLEAR_IMG), true);
		DrawGraph(100, 550 - (int)moveY, FileManager::Instance()->GetFileHandle(TOTITLE_IMG), true);
		DrawGraph(450, 550 - (int)moveY, FileManager::Instance()->GetFileHandle(CLOSE_IMG), true);
		DrawGraph((int)moveX, 550 - (int)moveY, FileManager::Instance()->GetFileHandle(CHOISE_IMG), true);
	}
	// �Q�[���I�[�o�[
	else {
		DrawGraph(0, (int)moveY, FileManager::Instance()->GetFileHandle(GAMEOVER_IMG), true);
		DrawGraph(100, 550 - (int)moveY, FileManager::Instance()->GetFileHandle(TOTITLE_IMG), true);
		DrawGraph(450, 550 - (int)moveY, FileManager::Instance()->GetFileHandle(CLOSE_IMG), true);
		DrawGraph((int)moveX, 550 - (int)moveY, FileManager::Instance()->GetFileHandle(CHOISE_IMG), true);
	}

	// �ړ�������������
	if (moveY >= 180)
	{
		// �t�F�[�h���͓��͂��󂯕t���Ȃ�
		if (Scene::Instance()->fadeState == Scene::Instance()->NONE) {
			// �X�e�[�g�؂�ւ�
			if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_RIGHT) == 1) {
				moveX += 350;
				if (moveX > 410) moveX = 410;
				type = GAMESTATE::END;
			}
			if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_LEFT) == 1) {
				moveX -= 350;
				if (moveX < 60) moveX = 60;
				type = GAMESTATE::TITLE;
			}
		}

		// �^�C�g���ɖ߂邩�Q�[���I��
		if (KeyInput::Instance()->Keyboard_Get(KEY_INPUT_SPACE) == 1) {
			switch (type) {
			case GAMESTATE::TITLE:
				Scene::Instance()->ChangeFadeState(Scene::Instance()->FADEOUT, Scene::Instance()->TITLE);
				Finalize();
				break;
			case GAMESTATE::END:
				Scene::Instance()->isGame = false;
				break;
			}
		}
	}
}

// �I������
void GameScene::Finalize()
{
	AIManager::Instance()->Finalize();
	CharacterManager::Instance()->Finalize();
}

