#include "GameScene.h"
#include "CharacterManager.h"
#include "AIManager.h"

// �R���X�g���N�^
GameScene::GameScene()
{
	Initialize();

	CharacterManager::Instance()->Initialize();
	CharacterManager::Instance()->SetCameraOffset(-1, true);
}

// ������
void GameScene::Initialize()
{
	LoadFile();
}

void GameScene::LoadFile()
{
	// �摜�̓ǂݍ���
	FileManager::Instance()->GetFileHandle(FIELD_IMG);
	FileManager::Instance()->GetFileHandle(CURSOR_IMG);
	turnChangeImg = FileManager::Instance()->GetFileHandle(PLAYERTURN_IMG);
}

void GameScene::UnLoadFile()
{

}

void GameScene::TurnChange(bool playerTurn)
{
	// �ʒu�ύX
	moveX -= 24;

	// �v���C���[�^�[��
	if (playerTurn) {
		//DrawGraph((int)moveX, 0, turnChangeImg, true);
		DrawRotaGraph((int)moveX, 240, 1.0f, 0.0f, turnChangeImg, true);
	}
	// �G�l�~�[�^�[��
	else {
		//DrawGraph((int)moveX, 0, turnChangeImg, true);
		DrawRotaGraph((int)moveX, 240, 1.0f, 0.0f, turnChangeImg, true);
	}

	// �ړ�����������
	if (moveX < 336) {

		AudioManager::Instance()->playSE(SE_TURNSTART);

		for (int volume = 0; volume < 256; volume += 3) {
			AudioManager::Instance()->VolumeFade(volume);
		}

		WaitTimer(1000);
		moveX = 672;
		CharacterManager::Instance()->turnAnim = false;

		if (playerTurn) { 
			turnChangeImg = FileManager::Instance()->GetFileHandle(ENEMYTURN_IMG);
			AudioManager::Instance()->playBGM(BGM_PLAYER);

			for (int volume = 256; volume >= 0; volume -= 3) {
				AudioManager::Instance()->VolumeFade(volume);
			}
		}
		// �G�l�~�[�^�[��
		else {
			AIManager::Instance()->Play();
			turnChangeImg = FileManager::Instance()->GetFileHandle(PLAYERTURN_IMG);
			AudioManager::Instance()->playBGM(BGM_ENEMY);

			for (int volume = 256; volume >= 0; volume -= 3) {
				AudioManager::Instance()->VolumeFade(volume);
			}
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
	// ���W�X�V
	if (CharacterManager::Instance()->playerTurn) {
		/*xPos = KeyInput::Instance()->xPos;
		yPos = KeyInput::Instance()->yPos;*/
	}
	else {
		xPos = AIManager::Instance()->x;
		yPos = AIManager::Instance()->y;
	}

	// �`��
	DrawGraph(0 - (int)KeyInput::Instance()->cameraPos.x, 0 - (int)KeyInput::Instance()->cameraPos.y, FileManager::Instance()->GetFileHandle(FIELD_IMG), true);

	// �L�����N�^�[�I��
	if (KeyInput::Instance()->isSelect == true && CharacterManager::Instance()->attack == false) {
		CharacterManager::Instance()->Draw();
		CharacterManager::Instance()->GetMoveArrow(xPos, yPos);
	}

	// �J�[�\���\��
	DrawGraph(xPos, yPos, FileManager::Instance()->GetFileHandle(CURSOR_IMG), true);
}

// �L�[���͏���
void GameScene::KeyEvent()
{
	if (CharacterManager::Instance()->isMove || CharacterManager::Instance()->playerTurn == false || CharacterManager::Instance()->turnAnim) return;

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
		if (CharacterManager::Instance()->isDetail) {
			CharacterManager::Instance()->ChangeDetailCharacter(CharacterManager::Instance()->checkCharacter, 1);
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
		if (CharacterManager::Instance()->isDetail) {
			CharacterManager::Instance()->ChangeDetailCharacter(CharacterManager::Instance()->checkCharacter, -1);
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
	if(moveY < 180) moveY += 24;

	// �N���A�\��
	if (isClear) {
		DrawGraph(0, (int)moveY, FileManager::Instance()->GetFileHandle(CLEAR_IMG), true);
	}
	// �Q�[���I�[�o�[
	else {
		DrawGraph(0, (int)moveY, FileManager::Instance()->GetFileHandle(GAMEOVER_IMG), true);
	}
}

void GameScene::Finalize()
{
	AIManager::Instance()->Finalize();
	CharacterManager::Instance()->Finalize();
}

