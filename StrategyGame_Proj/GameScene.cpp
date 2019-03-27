#include "GameScene.h"
#include "KeyInput.h"
#include "CharacterManager.h"
#include "AIManager.h"

CharacterManager* characterMgr;

// �R���X�g���N�^
GameScene::GameScene()
{
	characterMgr = CharacterManager::Instance();

	Initialize();

	characterMgr->Initialize();
}

// ������
void GameScene::Initialize()
{
	// �摜�̓ǂݍ���
	stageImg = LoadGraph(FIELD_IMG);
	cursorImg = LoadGraph(CURSOR_IMG);
	turnChangeImg = LoadGraph(PLAYERTURN_IMG);
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
		DrawGraph((int)moveX, 0, turnChangeImg, true);
	}
	// �G�l�~�[�^�[��
	else {
		DrawGraph((int)moveX, 0, turnChangeImg, true);
		
	}

	// �ړ�����������
	if (moveX < 0) {
		WaitTimer(1000);
		moveX = 672;
		characterMgr->turnAnim = false;
		if (playerTurn) turnChangeImg = LoadGraph(ENEMYTURN_IMG);
		// �G�l�~�[�^�[��
		else {
			AIManager::Instance()->Play();
			turnChangeImg = LoadGraph(PLAYERTURN_IMG);
		}
	}
}

// �V�[���S�̂̍X�V
void GameScene::Update()
{
	if (characterMgr->playerTurn) {
		KeyInput::Instance()->InputCalc(characterMgr);

		Draw();

		characterMgr->Update(xPos, yPos);

		if (characterMgr->turnAnim) {
			TurnChange(characterMgr->playerTurn);
		}
	}
	else {
		Draw();

		AIManager::Instance()->Update();

		characterMgr->Update(AIManager::Instance()->x, AIManager::Instance()->y);

		if (characterMgr->turnAnim) {
			TurnChange(characterMgr->playerTurn);
		}
	}
}

// ���C���V�[���̕`��
void GameScene::Draw()
{
	// ���W�X�V
	if (characterMgr->playerTurn) {
		xPos = KeyInput::Instance()->xPos;
		yPos = KeyInput::Instance()->yPos;
	}
	else {
		xPos = AIManager::Instance()->x;
		yPos = AIManager::Instance()->y;
	}

	// �`��
	DrawGraph(0 - (int)KeyInput::Instance()->cameraPos.x, 0 - (int)KeyInput::Instance()->cameraPos.y, stageImg, true);

	if (KeyInput::Instance()->isSelect == true && characterMgr->attack == false) {
		characterMgr->Draw();
		characterMgr->GetMoveArrow(xPos, yPos);
	}

	// �J�[�\���\��
	DrawGraph(xPos, yPos, cursorImg, true);
}

