#pragma once
#include "CharacterManager.h"
#include "Character.h"

// �GAI���Ǘ�����N���X
class AIManager
{
public:
	AIManager();

	// AI�𓮂������ǂ���
	bool AIMove;

	// ������
	void Initialize();

	// �X�V
	void Update();

	// ���݂̓G(AI)�A�v���C���[�̃J�E���g
	void CharacterCount(Character::STATUS status);

	// ����N��
	void Play();

	// �������L�����N�^�[�̑I��
	void MoveSelect(Character::STATUS* status);

	// �ړ����I������
	void ChoiseMovePoint();

	// �v���C���[���L�����N�^�[�Ƃ̋������擾
	int GetDistance(Character::STATUS* status, vector<Character::STATUS> playerList);
private:
	// �ړ���̍��W
	int x = 0, y = 0;

	// CharacterManager�̃C���X�^���X
	CharacterManager* characterMgr;

	// �G(AI)�L�����N�^�[�̃��X�g
	vector<Character::STATUS> enemyList;
	// �v���C���[���̃L�����N�^�[�̃��X�g
	vector<Character::STATUS> playerList;

	// ���삷��L�����N�^�[�̃C���X�^���X
	Character::STATUS* myStatus;

	// �G�L�����N�^�[�̃��X�g(���S����)
	void CharacterLost(Character::STATUS* status);
};