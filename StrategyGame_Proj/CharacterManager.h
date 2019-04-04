#pragma once
#include "Character.h"

// ���j�b�g��
#define NAME_LOAD "Load"
#define NAME_SUPPORT "Support"
#define NAME_ENEMY1 "Enemy1"
#define NAME_ENEMY2 "Enemy2"

// �L�����N�^�[�̃f�[�^
#define CHARACTER_DATA_1 "Resources\\CharacterData\\���[�h.dat"
#define CHARACTER_DATA_2 "Resources\\CharacterData\\�T�|�[�g.dat"
#define CHARACTER_DATA_3 "Resources\\CharacterData\\�G1.dat"
#define CHARACTER_DATA_4 "Resources\\CharacterData\\�G2.dat"
#define DAMAGE_DETAIL "DamageDetail.png"
#define HP_BAR "HP_Bar.png"
#define HP_BARBOX "HP_BarBox.png"

class CharacterManager : public Singleton<CharacterManager>
{
public:
	~CharacterManager();

	// ��������L�����N�^�[
	vector<Character*> character;
	// vector<unique_ptr<Character>> _character;

	// �U�����郆�j�b�g
	int attackUnitNum = 0;
	Character* myCharacter = nullptr;
	// �U������郆�j�b�g
	int blockUnitNum = 0;
	Character* eCharacter = nullptr;
	// �S�̂̍U����
	int attackCount = 0;

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

	// �I������
	void Finalize();

private:
	// HP�p�摜
	int HpBar;
	int HpBarBox;
	int DamageDetail;
	// �s���\�ȃ��j�b�g��
	int moveableUnit = 0;
};