#pragma once
#include "DxLib.h"
#include "StageCreate.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

#define CAN_MOVE_AREA "Resources\\image\\CanMove.png"
#define CAN_ATTACK_AREA "Resources\\image\\CanAttack.png"
#define ATTACK_DETAIL "Resources\\image\\AttackGraph.png"
#define ARROW "Resources\\image\\Arrow.png"
#define CHARACTER_IMG "Resources\\image\\Character_40�~40.png"

class Character
{
public:
	Character();
	~Character();

	// �v���C���[�X�e�[�^�X�̗�
	enum PLAYER_PARAM
	{
		HP = 0,
		POWER,
		TECHNIQUE,
		SPEED,
		LUCKY,
		DEFENCE,
		MAGIC_DEFENCE,
		PHYSIQUE,
		MOVERANGE,
		TEAM
	};

	// �L�����N�^�[�̃p�����[�^
	typedef struct {
		int HP;
		int POWER;
		int TECHNIQUE;
		int SPEED;
		int LUCKY;
		int DEFENCE;
		int MAGIC_DEFENCE;
		int PHYSIQUE;
		int MOVERANGE;

		// �U���Ɏg���p�����[�^
		int ATTACK_STR;  // �U����
		int ATTACK_CLT;  // �K�E
		int ATTACK_AVO; // ����
		int ATTACK_HIT;  // ������
		int ATTACK_SPEED;
	} PARAM;

	// �L�����N�^�[�̃X�e�[�^�X
	typedef struct {
		PARAM myParam;  // ���g�̃p�����[�^
		string myTeam;  // ���g�̐w�c
		int PosX;             // ���݂�x���W
		int PosY;             // ���݂�y���W
		int _PosX;           // �ړ��O��x���W
		int _PosY;           // �ړ��O��y���W
		int Image[20];    // �L�����N�^�[�p�摜
		float AnimHandle;  // �A�j���[�V�����p�ϐ�
		int AttackRange;    // �U���͈�
		double Count;            // �A�j���[�V�����J�E���g
		bool animReset;  // �A�j���[�V�����̃��Z�b�g 
		bool isSelect;      // �I������Ă��邩
		bool canMove = true;       // �ړ��\��
		bool isAttack;     // �U������
		bool canAttack;   // �U���\��
		bool isDeath;      // ���S����
	}STATUS ;

	// ����
	vector<int> OldPosX;
	vector<int> OldPosY;

	// �ړ��l
	int moveCount;

	// ������
	void Character_Initialize(STATUS* status, string pass, string team, int posX, int posY);
	// �p�����[�^�擾
	void GetCharacterParam(string pass);

	// �`��
	void SpriteDraw(int x, int y, int img);
	// �L�����N�^�[�̃A�j���[�V����
	void CharacterAnim(STATUS* status);

	// �ړ��͈͌v�Z
    void MoveRange(STATUS* status, int x, int y, int moveRange);
	// �U���͈͕`��
	void AttackRange();
	// �ړ������`��
	void DrawMoveArrow(STATUS status, int x, int y, int moveValue);
	// �L�����N�^�[�̈ړ�
	bool CharacterMove(STATUS* status, int moveX, int moveY);

	// �U���`�F�b�N
	void AttackCheck(STATUS* status);
	// �U���͈͕`��
	void AttackableDraw(STATUS status);
	// �U���̏ڍ׏��\��
	void GetAttackDetail(STATUS myStatus, STATUS eStatus);
	// �U���A�j���[�V����
	bool AttackAnimation(STATUS* myStatus, STATUS* eStatus, int count);
	// �U���̌v�Z
	void CharacterAttack(STATUS* myStatus, STATUS* eStatus, int count);
	// �U���̏���
	void CharacterDamage(STATUS* myStatus, STATUS* eStatus, int damage);

	// �J�����Ƃ̃I�t�Z�b�g�v�Z
	void SetCameraOffset(STATUS* status, int dir, bool horizontal);

	// �ړ��G���A�̃��Z�b�g
	void MoveAreaClear();
};