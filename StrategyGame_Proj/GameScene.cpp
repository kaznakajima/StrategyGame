#include "GameScene.h"
#include "FileManager.h"
#include "KeyInput.h"
#include "CharacterManager.h"
#include "AIManager.h"

// �R���X�g���N�^
GameScene::GameScene()
{
	Initialize();

	CharacterManager::Instance()->Initialize();
}

// ������
void GameScene::Initialize()
{
	// �摜�̓ǂݍ���
	FileManager::Instance()->GetFileHandle(FIELD_IMG);
	FileManager::Instance()->GetFileHandle(CURSOR_IMG);
	turnChangeImg = FileManager::Instance()->GetFileHandle(PLAYERTURN_IMG);
	/*stageImg = LoadGraph(FIELD_IMG);
	cursorImg = LoadGraph(CURSOR_IMG);
	turnChangeImg = LoadGraph(PLAYERTURN_IMG);*/
}

void GameScene::LoadFile()
{

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
		KeyInput::Instance()->InputCalc(CharacterManager::Instance());

		Draw();

		CharacterManager::Instance()->Update(xPos, yPos);

		// �^�[���J�n���o
		if (CharacterManager::Instance()->turnAnim) {
			TurnChange(CharacterManager::Instance()->playerTurn);
		}
	}
	// �G�̃^�[��
	else {
		Draw();

		// AI�̍X�V
		AIManager::Instance()->Update();

		CharacterManager::Instance()->Update(AIManager::Instance()->x, AIManager::Instance()->y);

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
		xPos = KeyInput::Instance()->xPos;
		yPos = KeyInput::Instance()->yPos;
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

void GameScene::Finalize()
{
	AIManager::Instance()->Finalize();
	CharacterManager::Instance()->Finalize();
}

