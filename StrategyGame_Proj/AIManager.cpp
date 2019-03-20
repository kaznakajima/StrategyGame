#include "AIManager.h"
#include "StageCreate.h"

AIManager::AIManager()
{
	characterMgr = CharacterManager::Instance();
}

// ������
void AIManager::Initialize()
{
	// ���X�g�̏�����
	playerList.clear();
	enemyList.clear();
}

// �X�V
void AIManager::Update()
{
	if (myCharacter != nullptr) {
		if (characterMgr->isSelect) {
			characterMgr->Draw();
			characterMgr->GetMoveArrow(x, y);
			characterMgr->KeyCheck(x, y);
		}
		if (myCharacter->myStatus->canAttack) characterMgr->DrawCheck(xPos, yPos);
	}
}

// ���݂̓G(AI)�A�v���C���[�̃J�E���g
void AIManager::CharacterCount(Character* character)
{
	if (character->myStatus->myTeam == "Player") {
		playerList.push_back(character);
	}
	else if (character->myStatus->myTeam == "Enemy") {
		enemyList.push_back(character);
	} 
}

// ����N��
void AIManager::Play()
{
	int minDistance = 100;

	// �v���C���[�Ɉ�ԋ߂����j�b�g���s��������
	for (Character* character : enemyList) {
		// ��ԋ߂��L�����N�^�[��I��
		if (minDistance > GetDistance(character, playerList) && character->myStatus->canMove) {
			minDistance = GetDistance(character, playerList);
			myCharacter = character;
		}
	}

	if(myCharacter != nullptr) MoveSelect(myCharacter);
}

// �������L�����N�^�[�̑I��
void AIManager::MoveSelect(Character* character)
{
	x = character->myStatus->PosX, y = character->myStatus->PosY;

	characterMgr->DrawCheck(x, y);

	for (int stageY = 0; stageY < StageCreate::Instance()->MAP_SIZEY; stageY++) {
		for (int stageX = 0; stageX < StageCreate::Instance()->MAP_SIZEX; stageX++) {
			if (StageCreate::Instance()->checkMove[stageY][stageX] == true) {
				x = stageX * CHIP_SIZE;
				y = stageY * CHIP_SIZE;
			}
		}
	}

	for (Character* _character : playerList) {
		xPos = _character->myStatus->PosX, yPos = _character->myStatus->PosY;
		if (yPos / CHIP_SIZE > 9 && StageCreate::Instance()->checkMove[yPos / CHIP_SIZE + 1][xPos / CHIP_SIZE] == true) ChoiseMovePoint(xPos, yPos + CHIP_SIZE);
		if (yPos / CHIP_SIZE < 0 && StageCreate::Instance()->checkMove[yPos / CHIP_SIZE - 1][xPos / CHIP_SIZE] == true) ChoiseMovePoint(xPos, yPos - CHIP_SIZE);
		if (xPos / CHIP_SIZE < 14 && StageCreate::Instance()->checkMove[yPos / CHIP_SIZE][xPos / CHIP_SIZE + 1] == true) ChoiseMovePoint(xPos + CHIP_SIZE, yPos);
		if (xPos / CHIP_SIZE > 0 && StageCreate::Instance()->checkMove[yPos / CHIP_SIZE][xPos / CHIP_SIZE - 1] == true) ChoiseMovePoint(xPos - CHIP_SIZE, yPos);
	}
}

// �ړ���̑I��
void AIManager::ChoiseMovePoint(int xPos, int yPos)
{
	if (StageCreate::Instance()->onUnit[yPos / CHIP_SIZE][xPos / CHIP_SIZE] != "NONE") return;

	x = xPos, y = yPos;
}

// �v���C���[���̃L�����N�^�[�̎擾
int AIManager::GetDistance(Character* character, vector<Character*> playerList)
{
	// �v���C���[���̃L�����N�^�[�Ƃ̋���
	int offsetX = 0, offsetY = 0, offsetTotal = 0;
	// �ŒZ����
	int minDistance = 100;

	// �v���C���[����̋����̌v�Z
	for (Character* playerSt : playerList) {
		offsetTotal = 0;
		offsetX = abs(character->myStatus->PosX - playerSt->myStatus->PosX) / CHIP_SIZE;
		offsetY = abs(character->myStatus->PosY - playerSt->myStatus->PosY) / CHIP_SIZE;
		offsetTotal = offsetX + offsetY;

		// �ŒZ�����̍X�V
		if (minDistance > offsetTotal) {
			minDistance = offsetTotal;
		}
	}

	return minDistance;
}

// �G�L�����N�^�[�̃��X�g(���S����)
void AIManager::CharacterLost(Character* character)
{

}