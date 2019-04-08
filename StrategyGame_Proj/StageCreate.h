#pragma once
#include "DxLib.h"
#include "Singleton.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

#define STAGE_FILE_1 "Resources\\StageData\\StageData.csv"
#define BACKGROUND "Resources\\image\\Field.png"

#define SCREEN_WIDTH 672
#define SCREEN_HEIGHT 480

#define STAGE1_WIDTH 720
#define STAGE1_HEIGHT 480

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
	StageCreate();

	// �X�e�[�W�T�C�Y
	int MAP_SIZEX = 15;
	int MAP_SIZEY = 10;

	// �X�e�[�W�̃f�[�^
	vector<vector<int>> stageList;

	// ���j�b�g�����邩�ǂ���
	vector<vector<string>> onUnit;

	// �ړ��\���ǂ���
	vector<vector<bool>> checkMove;

	// �t�@�C�����J�� (string �t�@�C����)
	void Open(string pass);

	// �X�e�[�W��̃f�[�^���擾
	int GetData(int x, int y);

	// �X�e�[�W�̏�Ԃ��X�V
	void StageUpdate(int x, int y);

	// ���j�b�g������}�X�ڂ��`�F�b�N
	void CheckOnUnit(int x, int y, string str);
};