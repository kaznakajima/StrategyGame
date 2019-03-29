#pragma once
#include "Character.h"

// �L�����N�^�[�̃f�[�^
#define CHARACTER_DATA_1 "Resources\\CharacterData\\PlayerData.dat"
#define CHARACTER_DATA_2 "Resources\\CharacterData\\PlayerData_1.dat"
#define CHARACTER_DATA_3 "Resources\\CharacterData\\PlayerData_2.dat"
#define DAMAGE_DETAIL "Resources\\image\\DamageDetail.png"
#define HP_BAR "Resources\\image\\HP_Bar.png"
#define HP_BARBOX "Resources\\image\\HP_BarBox.png"

class CharacterManager : public Singleton<CharacterManager>
{
public:
	// ��������L�����N�^�[
	vector<Character*> character;

	// �U�����郆�j�b�g
	int attackUnitNum = 0;
	Character* myCharacter = nullptr;
	// �U������郆�j�b�g
	int blockUnitNum = 0;
	Character* eCharacter = nullptr;

	// �^�[���ύX�A�j���[�V����
	bool turnAnim;
	// �v���C���[�̃^�[�����ǂ���
	bool playerTurn = false;
	// �I�𔻒�
	bool isSelect;
	// �ړ�����
	bool isMove;
	// �U������
	bool attack;

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
	// �U�����̃f�[�^�\��
	void DrawAttackParam(Character* attackChara, Character* defenceChara);

	// �J�����Ƃ̃I�t�Z�b�g�v�Z
	void SetCameraOffset(int dir, bool horizontal);

private:
	// HP�p�摜
	int HpBar;
	int HpBarBox;
	int DamageDetail;
	// �S�̂̍U����
	int attackCount = 0;
	// �s���\�ȃ��j�b�g��
	int moveableUnit = 0;

	// �I��
	void Finalize();
};