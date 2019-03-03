#include "AIManager.h"
#include "StageCreate.h"

AIManager::AIManager()
{
	characterMgr = new CharacterManager();
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
	if (myStatus != nullptr && myStatus->isSelect) characterMgr->Draw();

	characterMgr->Update(x, y);
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

// ����N��
void AIManager::Play()
{
	int minDistance = 100;

	// �v���C���[�Ɉ�ԋ߂����j�b�g���s��������
	for (Character::STATUS &status : enemyList) {
		// ��ԋ߂��L�����N�^�[��I��
		if (minDistance > GetDistance(&status, playerList) && status.canMove) {
			minDistance = GetDistance(&status, playerList);
			myStatus = &status;
		}
	}

	MoveSelect(myStatus);
}

// �������L�����N�^�[�̑I��
void AIManager::MoveSelect(Character::STATUS* status)
{
	x = status->PosX, y = status->PosY;

	ChoiseMovePoint();
}

// �ړ���̑I��
void AIManager::ChoiseMovePoint()
{
	characterMgr->DrawCheck(x, y);
}

// �v���C���[���̃L�����N�^�[�̎擾
int AIManager::GetDistance(Character::STATUS* status, vector<Character::STATUS> playerList)
{
	// �v���C���[���̃L�����N�^�[�Ƃ̋���
	int offsetX = 0, offsetY = 0, offsetTotal = 0;
	// �ŒZ����
	int minDistance = 100;

	// �v���C���[����̋����̌v�Z
	for (Character::STATUS &playerSt : playerList) {
		offsetX = abs(playerSt.PosX - status->PosX);
		offsetY = abs(playerSt.PosY - status->PosY);
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