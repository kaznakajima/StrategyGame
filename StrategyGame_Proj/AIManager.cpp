#include "AIManager.h"

// ������
void AIManager::Initialize()
{
	// ���X�g�̏�����
	enemyList.clear();
}

// ���݂̓G(AI)�A�v���C���[�̃J�E���g
void AIManager::CharacterCount(Character::STATUS status)
{
	if (status.myTeam == "Player") {
		playerList.push_back(status);
	}
	else if (status.myTeam == "Enemy") {
		enemyList.push_back(status);
	} 
}

// �������L�����N�^�[�̑I��
void AIManager::MoveSelect(Character::STATUS* status)
{
	status->isSelect = true;

	SerchCharacter(status, playerList);
}

// �ړ���̑I��
void AIManager::ChoiseMovePoint()
{
	// �ړ���̍��W
	int x = 0, y = 0;
}

// �v���C���[���̃L�����N�^�[�̎擾
int AIManager::SerchCharacter(Character::STATUS* status, vector<Character::STATUS> playerList)
{
	// �v���C���[���̃L�����N�^�[�Ƃ̋���
	int offsetX = 0, offsetY = 0, offsetTotal = 0;
	// �ŒZ����
	int minDistance = 100;

	// �v���C���[����̋����̌v�Z
	for (unsigned int num = 0; num < playerList.size(); num++) {
		offsetX = playerList[num].PosX - status->PosX;
		offsetY = playerList[num].PosY - status->PosY;
		offsetTotal = offsetX + offsetY;

		// �ŒZ�����̍X�V
		if (minDistance > offsetTotal) {
			minDistance = offsetTotal;
		}
	}

	return minDistance;
}

// �G�L�����N�^�[�̃��X�g(���S����)
void AIManager::CharacterLost(Character::STATUS* status)
{

}