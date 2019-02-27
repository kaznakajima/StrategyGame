#pragma once

// �L�����N�^�[�̃f�[�^
#define CHARACTER_DATA_1 "Resources\\CharacterData\\PlayerData.dat"
#define CHARACTER_DATA_2 "Resources\\CharacterData\\PlayerData_1.dat"
#define CHARACTER_DATA_3 "Resources\\CharacterData\\PlayerData_2.dat"

class CharacterManager
{
public:
	// �v���C���[�̃^�[�����ǂ���
	bool playerTurn = false;
	// �I�𔻒�
	bool isSelect;
	// �ړ�����
	bool isMove;
	// �U������
	bool attack;
	// �S�̂̍U����
	int attackCount = 0;
	// �s���\�ȃ��j�b�g��
	int moveableUnit = 0;

	// ������
	void Initialize();
	// �X�V
	void Update(int x, int y);

	// �^�[���J�n
	void StartTurn();

	// �`�悷�邩�`�F�b�N
	void DrawCheck(int x, int y);
	// �`��
	void Draw();

	// ���͌��m
	void KeyCheck(int x, int y);

	// �ړ��l�̎擾
	void GetMoveCount(int x, int y);
	// �ړ������擾
	void GetMoveArrow(int x, int y);
	// ���j�b�g�̈ړ�
	void CharacterMove(int x, int y);

	// �U���͈͕\��
	void GetAttackArea(int x, int y);
	// �U���̑Ώۂ�I��
	void ChoiseAttack(int x, int y);
	// �U���̏���
	void Attack();

	// �J�����Ƃ̃I�t�Z�b�g�v�Z
	void SetCameraOffset(int dir, bool horizontal);

	// �I��
	void Finalize();
};