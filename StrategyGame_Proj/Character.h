#pragma once
#include "DxLib.h"
#include "StageCreate.h"
#include "AttackPhase.h"
#include "TimeCount.h"
#include "FileManager.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define CAN_MOVE_AREA "CanMove.png"
#define CAN_ATTACK_AREA "CanAttack.png"
#define ATTACK_DETAIL "AttackGraph.png"
#define ARROW "Resources\\image\\Arrow.png"
#define CHARACTER_IMG "Resources\\image\\_Character_40�~40.png"

class Character
{
public:
	Character();

	// �v���C���[�X�e�[�^�X�̗�
	enum PLAYER_PARAM
	{
		LEVEL = 0,
		HP,
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
		int LEVEL;
		int MaxHP;
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
		PARAM myParam;               // ���g�̃p�����[�^
		string NAME;                      // ���j�b�g��
		string myData;                    // ���g�̃f�[�^
		string myTeam;                   // ���g�̐w�c
		int PosX;                             // ���݂�x���W
		int PosY;                             // ���݂�y���W
		int _PosX;                           // �ړ��O��x���W
		int _PosY;                           // �ړ��O��y���W
		int Image[20];                   // �L�����N�^�[�p�摜
		float AnimHandle;              // �A�j���[�V�����p�ϐ�
		int AttackRange;                // �U���͈�
		bool animReset;                // �A�j���[�V�����̃��Z�b�g 
		bool isSelect;                    // �I������Ă��邩
		bool canMove = true;       // �ړ��\��
		bool isAttack;                  // �U������
		bool canAttack = false;   // �U���\��
		bool isDeath = false;      // ���S����
	}STATUS ;

	// ����
	vector<int> OldPosX;
	vector<int> OldPosY;

	unique_ptr<STATUS> myStatus = make_unique<STATUS>();

	// �ړ��ł���G���A
	vector<vector<int>> moveToPos = vector<vector<int>>(StageCreate::Instance()->MAP_SIZEY, vector<int>(StageCreate::Instance()->MAP_SIZEX, -1));

	// �ړ���̓��؂��ǂ���
	vector<vector<bool>> moveArrow = vector<vector<bool>>(StageCreate::Instance()->MAP_SIZEY, vector<bool>(StageCreate::Instance()->MAP_SIZEX, 0));

	// �ړ��l
	int moveCount;

	// ������
	void Character_Initialize(string pass, string name, string team, int posX, int posY);
	// �p�����[�^�擾
	void GetCharacterParam(string pass);
	void SetParam(string name);

	// �`��
	void SpriteDraw(int x, int y, int img);
	// �L�����N�^�[�̃A�j���[�V����
	void CharacterAnim();

	// �ړ��ĊJ(�^�[���J�n���ɌĂяo��)
	void TurnStart();
	// �ړ��͈͌v�Z
    void MoveRange(int x, int y, int moveRange);
	// �U���͈͕`��
	void AttackRange();
	// �ړ������`��
	void DrawMoveArrow(int x, int y, int moveValue);
	// �L�����N�^�[�̈ړ�
	bool CharacterMove(int moveX, int moveY);

	// �U���`�F�b�N
	void AttackCheck();
	// �U���͈͕`��
	void AttackableDraw();
	// �U���̏ڍ׏��\��
	void GetAttackDetail(Character* eCharacter);
	// �U���A�j���[�V����
	bool AttackAnimation(Character* eCharacter, int count);
	// �U���̌v�Z
	void CharacterAttack(Character* eCharacter, int count);
	// �U���̏���
	void CharacterDamage(Character* eCharacter, int damage);

	// �J�����Ƃ̃I�t�Z�b�g�v�Z
	void SetCameraOffset(int dir, bool horizontal);

	// �ړ��G���A�̃��Z�b�g
	void MoveAreaClear(vector<Character*> _character);

	// �I������
	void Finalize();

private:
	// �ړ��o�H
	int ArrowImage[6];

	// ���x���A�b�v
	void LevelUp();

	// �p�����[�^
	string name;
	int _param[10];
};