#include "StageCreate.h"

// �}�b�v�̍\����
struct Cell {
	// �X�e�[�W�̏��
	int map_gh;
	string map_str;
};

// �}�b�v�̏�Ԃ�ۑ�
vector<vector<Cell>> cell;

StageCreate::StageCreate()
{
	stageList = vector<vector<int>>(MAP_SIZEY, vector<int>(MAP_SIZEX, 0));
	onUnit = vector<vector<string>>(MAP_SIZEY, vector<string>(MAP_SIZEX, "NONE"));
	checkMove = vector<vector<bool>>(MAP_SIZEY, vector<bool>(MAP_SIZEX, false));
	cell = vector<vector<Cell>>(MAP_SIZEY, vector<Cell>(MAP_SIZEX));
}

void StageCreate::Open(string pass)
{
	// �t�@�C���̗�
	int width = 0;
	// �t�@�C���̍s
	int height = 0;

	ifstream ifs(pass);
	string str = "";

	// �t�@�C���̒��g����s���ǂݎ��
	while (getline(ifs, str)) {
		string tmp = "";
		istringstream stream(str);

		while (getline(stream, tmp, ',')) {
			// �������ۑ�
			cell[height][width].map_str = tmp;
			// �������int�^�ɕύX
			int num = atoi(cell[height][width].map_str.c_str());
			// �f�[�^��ۑ�
			cell[height][width].map_gh = num;
			// ���̕�����
			width++;
		}

		// �擪��
		width = 0;
		// ���s
		height++;
	}
}

// �ǂݍ��񂾃X�e�[�W�����擾
int StageCreate::GetData(int x, int y) 
{
	return cell[y][x].map_gh;
}

// �X�e�[�W��̏����X�V
void StageCreate::StageUpdate(int x, int y)
{
	stageList[y][x] = GetData(x, y);
	onUnit[y][x] = "NONE";
	checkMove[y][x] = false;
}

// ���j�b�g�̂���}�X�ڂ��`�F�b�N
void StageCreate::CheckOnUnit(int x, int y, string str)
{
	onUnit[y][x] = str;
}
