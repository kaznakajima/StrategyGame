#pragma once
#include "DxLib.h"
#include "Singleton.h"
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

#define STAGE_FILE_1 "Resources\\StageData\\StageData.csv"
#define BACKGROUND "Resources\\image\\Field.png"

#define SCREEN_WIDTH 672
#define SCREEN_HEIGHT 480

#define STAGE1_WIDTH 720
#define STAGE1_HEIGHT 480

#define STAGE_LONG_SIZE 100

#define MAP_SIZEX 15
#define MAP_SIZEY 10

#define CHIP_SIZE 48

/*
Map�N���X
MapChip�̓ǂݍ��݂�����(�e�X�g)

 �g����(��)
  include ����
  Map::Open() �Ńt�@�C�����J��
  Map::Draw() �ŕ`��
*/

class StageCreate : public Singleton <StageCreate>
{
public:
	// �X�e�[�W�̃f�[�^
	int stageList[MAP_SIZEY][MAP_SIZEX];

	// ���j�b�g�����邩�ǂ���
	bool onUnit[MAP_SIZEY][MAP_SIZEX];

	// �ړ��\���ǂ���
	bool checkMove[MAP_SIZEY][MAP_SIZEX];

	// �t�@�C�����J�� (string �t�@�C���� int �}�b�v�̃T�C�Y)
	static void Open(string);

	static void Draw(int);

	// �X�e�[�W��̃f�[�^���擾
	static int GetData(int x, int y);

	// �X�e�[�W�̏�Ԃ��X�V
	void StageUpdate(int x, int y);
};