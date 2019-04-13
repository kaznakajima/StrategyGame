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

	// ����N��
	void Play();
private:

	// �G(AI)�L�����N�^�[�̃��X�g
	vector<shared_ptr<Character>> _enemyList;
	// �v���C���[���̃L�����N�^�[�̃��X�g
	vector<shared_ptr<Character>> _playerList;

	// ���삷��L�����N�^�[�̃C���X�^���X
	shared_ptr<Character> _myCharacter = nullptr;

	// �ړ�����
	bool isMove;
	int moveToX = 0, moveToY = 0;
	// �ړ��ő�l�i�[
	int moveCost;
	// �v���C���[�Ƃ̍ŒZ����
	int minDistance = 100;
	
	// �������L�����N�^�[�̑I��
	void MoveSelect(shared_ptr<Character> const &character);

	// �ړ����I������
	void ChoiseMovePoint(int _x, int _y);

	// �v���C���[���L�����N�^�[�Ƃ̋������擾
	int GetDistancePlayer(shared_ptr<Character> const &character, vector<shared_ptr<Character>> const &playerList);

	// �ړ��n�_�̎擾
	void GetMovePoint(shared_ptr<Character> const &character, int _x, int _y, vector<shared_ptr<Character>> const &playerList);

	// �ړ��n�_���v���C���[�Ɍ�������ŒZ����������
	void CheckCanMove(shared_ptr<Character> const &character, int _x, int _y, shared_ptr<Character> const &playerSt);

	// �G�L�����N�^�[�̃��X�g(���S����)
	void CharacterLost(Character* character);
};