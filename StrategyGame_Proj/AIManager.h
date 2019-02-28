#pragma once
#include "Character.h"

// �GAI���Ǘ�����N���X
class AIManager
{
public:
	// ������
	void Initialize();

	// ���݂̓G(AI)�A�v���C���[�̃J�E���g
	void CharacterCount(Character::STATUS status);

	// �������L�����N�^�[�̑I��
	void MoveSelect(Character::STATUS* status);

	// �ړ����I������
	void ChoiseMovePoint();

	// �v���C���[���̃L�����N�^�[�̎擾
	int SerchCharacter(Character::STATUS* status, vector<Character::STATUS> playerList);
private:
	// �G(AI)�L�����N�^�[�̃��X�g
	vector<Character::STATUS> enemyList;
	// �v���C���[���̃L�����N�^�[�̃��X�g
	vector<Character::STATUS> playerList;

	// �G�L�����N�^�[�̃��X�g(���S����)
	void CharacterLost(Character::STATUS* status);
};