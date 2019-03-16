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
	if (myCharacter != nullptr && myCharacter->myStatus->isSelect) characterMgr->Draw();

	characterMgr->Update(x, y);
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

	MoveSelect(myCharacter);
}

// �������L�����N�^�[�̑I��
void AIManager::MoveSelect(Character* character)
{
	x = character->myStatus->PosX, y = character->myStatus->PosY;

	ChoiseMovePoint();
}

// �ړ���̑I��
void AIManager::ChoiseMovePoint()
{
	characterMgr->DrawCheck(x, y);
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
		offsetX = abs(playerSt->myStatus->PosX - character->myStatus->PosX);
		offsetY = abs(playerSt->myStatus->PosY - character->myStatus->PosY);
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