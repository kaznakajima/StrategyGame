#pragma once
#include "CharacterManager.h"
#include "Character.h"

// �GAI���Ǘ�����N���X
class AIManager : public Singleton<AIManager>
{
public:
	AIManager();
	~AIManager();

	// �ړ���̍��W
	int x = 0, y = 0; 
	int xPos = 0, yPos = 0;

	// AI�𓮂������ǂ���
	bool AIMove;

	bool isDebug = false;

	// ������
	void Initialize();
	// �I��
	void Finalize();

	// �X�V
	void Update();

	// ���݂̓G(AI)�A�v���C���[�̃J�E���g
	void CharacterCount(Character* character);

	// ����N��
	void Play();
private:

	// �G(AI)�L�����N�^�[�̃��X�g
	vector<Character*> enemyList;
	// �v���C���[���̃L�����N�^�[�̃��X�g
	vector<Character*> playerList;

	// ���삷��L�����N�^�[�̃C���X�^���X
	Character* myCharacter;

	// �ړ�����
	bool isMove;
	int moveToX = 0, moveToY = 0;
	// �ړ��ő�l�i�[
	int moveCost;
	// �v���C���[�Ƃ̍ŒZ����
	int minDistance = 100;
	
	// �������L�����N�^�[�̑I��
	void MoveSelect(Character* character);

	// �ړ����I������
	void ChoiseMovePoint(int _x, int _y);

	// �v���C���[���L�����N�^�[�Ƃ̋������擾
	int GetDistancePlayer(Character* character, vector<Character*> playerList);

	// �ړ��n�_�̎擾
	void GetMovePoint(Character* character, int _x, int _y, vector<Character*> playerList);

	// �ړ��n�_���v���C���[�Ɍ�������ŒZ����������
	void CheckCanMove(Character* character, int _x, int _y, Character* playerSt);

	// �G�L�����N�^�[�̃��X�g(���S����)
	void CharacterLost(Character* character);
};