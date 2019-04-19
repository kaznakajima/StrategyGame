#pragma once
#include "DxLib.h"
#include "Singleton.h"
#include "FileManager.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

// �X�e�[�W�f�[�^�t�@�C��
#define STAGE_FILE_1 "Resources\\StageData\\StageData.csv"

// �n�`�f�[�^�t�@�C��
#define TERRAIN_FOREST "forest.dat_t"
#define TERRAIN_HOUSE "home.dat_t"
#define TERRAIN_FORT "fort.dat_t"
#define TERRAIN_GATE "gate.dat_t"

// �摜�t�@�C��
#define TERRAIN_DETAIL "TerrainDetail.png"

// �E�C���h�E�T�C�Y
#define SCREEN_WIDTH 672
#define SCREEN_HEIGHT 480

// �X�e�[�W�T�C�Y
#define STAGE1_WIDTH 720
#define STAGE1_HEIGHT 480

// �`�b�v�T�C�Y
#define CHIP_SIZE 48

class StageCreate : public Singleton <StageCreate>
{
public:
	StageCreate();

	// ���j�b�g�̏����ʒu
	vector<int> p_InitPos;
	vector<int> e_InitPos;

	// �X�e�[�W�T�C�Y
	int MAP_SIZEX = 15;
	int MAP_SIZEY = 10;

	// ��������ׂ��v���C���[�̐�
	int playerCount = 2, enemyCount = 2;

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

	// �n�`�̏ڍ׏��̕\��
	void DrawTerrainDetail(int x, int y);

	// �n�`���ʂ�Ԃ�
	int GetTerrainParam(int x, int y, string _param);

private:
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

	// �n�`���ʂ��Z�b�g����
	void SetTerrainParam(int x, int y, int paramData);
};