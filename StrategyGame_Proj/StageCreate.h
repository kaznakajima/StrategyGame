#pragma once
#include "DxLib.h"
#include "Singleton.h"
#include "FileManager.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

#define STAGE_FILE_1 "Resources\\StageData\\StageData.csv"
#define BACKGROUND "Resources\\image\\Field.png"

#define TERRAIN_FOREST "�X.dat_t"
#define TERRAIN_HOUSE "��.dat_t"
#define TERRAIN_FORT "��.dat_t"
#define TERRAIN_GATE "��.dat_t"

#define SCREEN_WIDTH 672
#define SCREEN_HEIGHT 480

#define STAGE1_WIDTH 720
#define STAGE1_HEIGHT 480

#define CHIP_SIZE 48

class StageCreate : public Singleton <StageCreate>
{
public:
	StageCreate();

	// �}�b�v�̍\����
	struct Cell {
		// �X�e�[�W�̏��
		int map_gh;
		string map_str;
	};

	// �n�`����
	typedef struct {
		int DEF;   // ���
		int AVO;  // ���
	}  TERRAIN_PARAM;

	// �}�b�v�̏�Ԃ�ۑ�
	vector<vector<Cell>> cell;
	// �n�`���ʂ�ۑ�
	vector<vector<TERRAIN_PARAM>> terrain;

	// �X�e�[�W�T�C�Y
	int MAP_SIZEX = 15;
	int MAP_SIZEY = 10;

	// �X�e�[�W�̃f�[�^
	vector<vector<int>> stageList;

	// ���j�b�g�����邩�ǂ���
	vector<vector<string>> onUnit;

	// �ړ��\���ǂ���
	vector<vector<bool>> checkMove;

	// ������
	void Stage_Initialize();

	// �t�@�C�����J�� (string �t�@�C����)
	void Open(string pass);

	// �X�e�[�W��̃f�[�^���擾
	int GetData(int x, int y);

	// �X�e�[�W�̏�Ԃ��X�V
	void StageUpdate(int x, int y);

	// ���j�b�g������}�X�ڂ��`�F�b�N
	void CheckOnUnit(int x, int y, string str);

	// �n�`���ʂ��Z�b�g����
	void SetTerrainParam(int x, int y, int paramData);

	// �n�`���ʂ�Ԃ�
	int GetTerrainParam(int x, int y, string _param);
};