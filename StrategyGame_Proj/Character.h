#pragma once
#include "DxLib.h"
#include "StageCreate.h"
#include "AudioManager.h"
#include "Weapon.h"
#include "TimeCount.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define CAN_MOVE_AREA "CanMove.png"
#define CAN_ATTACK_AREA "CanAttack.png"
#define ATTACK_DETAIL "AttackGraph.png"
#define CHARACTER_DETAIL "CharacterParam.png"
#define ARROW "Resources\\Image\\Arrow.png"
#define CHARACTER_IMG "Resources\\Image\\Character_48�~48.png"

class Character
{
public:
	Character();

	// �L�����N�^�[�̃p�����[�^
	typedef struct {
		int LEVEL;
		int MaxHP;
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
		string myTeam;                  // ���g�̐w�c
		int HP;                              // ���݂�HP
		int xPos, yPos;                   // ���݂�x���W, ���݂�y���W
		int _xPos, _yPos;               // �ړ��O��x���W, �ړ��O��y���W                           
		int Image[20];                   // �L�����N�^�[�p�摜
		float AnimHandle;              // �A�j���[�V�����p�ϐ�
		int AttackRange;                // �U���͈�
		bool animReset;                // �A�j���[�V�����̃��Z�b�g 
		bool isSelect;                    // �I������Ă��邩
		bool canMove = true;        // �ړ��\��
		bool isAttack;                   // �U������
		bool canAttack = false;     // �U���\��
		bool isDeath = false;        // ���S����
	}STATUS ;

	// ����
	vector<int> OldPosX;
	vector<int> OldPosY;

	// ���g�̃X�e�[�^�X
	unique_ptr<STATUS> myStatus = make_unique<STATUS>();
	// ���g�̎g�p���镐��
	vector<unique_ptr<Weapon>> Item;

	// �ړ��ł���G���A
	vector<vector<int>> moveToPos = vector<vector<int>>(StageCreate::Instance()->MAP_SIZEY, vector<int>(StageCreate::Instance()->MAP_SIZEX, -1));

	// �ړ���̓��؂��ǂ���
	vector<vector<bool>> moveArrow = vector<vector<bool>>(StageCreate::Instance()->MAP_SIZEY, vector<bool>(StageCreate::Instance()->MAP_SIZEX, 0));

	// �ړ��l
	int moveCount;

	// ���g�̃p�����[�^�f�[�^
	string myData;

	// ������
	void Character_Initialize(string pass, string team, int posX, int posY);
	// �p�����[�^�擾
	void GetCharacterParam(string pass);
	// ���j�b�g�̏ڍ׏��\��
	void DrawCharacterDetail();

	// ���j�b�g�̈ʒu���擾
	VECTOR GetCharacterPosition();

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
	void GetAttackDetail(shared_ptr<Character> const &eCharacter);
	// �U���A�j���[�V����
	bool AttackAnimation(shared_ptr<Character> const &eCharacter, int count);
	// �U���̌v�Z
	void CharacterAttack(shared_ptr<Character> const &eCharacter, int count);
	// �U���̏���
	void ApplyDamage(shared_ptr<Character> const &eCharacter, int damage);

	// �J�����Ƃ̃I�t�Z�b�g�v�Z
	void SetCameraOffset(int dir, bool horizontal);

	// �ړ��G���A�̃��Z�b�g
	void MoveAreaClear(vector<shared_ptr<Character>> const &_character);

	// �I������
	void Finalize();

private:
	// �ړ��o�H
	int ArrowImage[6];

	// �A�C�e���ǉ�
	void AddItem(string itemName);

	// ���x���A�b�v
	void LevelUp();

	// �A�C�e���̏�����
	int itemCount = 0;

	// �p�����[�^
	int _param[10];
};