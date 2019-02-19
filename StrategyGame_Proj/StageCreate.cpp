#include "StageCreate.h"

// �}�b�v�̍\����
struct Cell {
	// �X�e�[�W�̏��
	int map_gh;
	string map_str;
};

// �}�b�v�̏�Ԃ�ۑ�
Cell cell[MAP_SIZEY][MAP_SIZEX];

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

void StageCreate::Draw(int PosX)
{

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
	onUnit[y][x] = false;
	checkMove[y][x] = false;
}
