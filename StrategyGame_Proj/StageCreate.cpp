#include "StageCreate.h"

// �R���X�g���N�^
StageCreate::StageCreate()
{
	
}

// �X�e�[�W�̏�����
void StageCreate::Stage_Initialize()
{
	// �z��̏�����
	stageList = vector<vector<int>>(MAP_SIZEY, vector<int>(MAP_SIZEX, 0));
	onUnit = vector<vector<string>>(MAP_SIZEY, vector<string>(MAP_SIZEX, "NONE"));
	checkMove = vector<vector<bool>>(MAP_SIZEY, vector<bool>(MAP_SIZEX, false));
	cell = vector<vector<Cell>>(MAP_SIZEY, vector<Cell>(MAP_SIZEX));
	terrain = vector<vector<TERRAIN_PARAM>>(MAP_SIZEY, vector<TERRAIN_PARAM>(MAP_SIZEX));
}

void StageCreate::Open(string pass)
{
	// �t�@�C���̗�
	int width = 0;
	// �t�@�C���̍s
	int height = -1;

	ifstream ifs(pass);
	string str = "";

	// �t�@�C���̒��g����s���ǂݎ��
	while (getline(ifs, str)) {
		string tmp = "";
		istringstream stream(str);

		while (getline(stream, tmp, ',')) {
			if (height == -1) {
				if (width == 0) MAP_SIZEX = atoi(tmp.c_str());
				else { 
					MAP_SIZEY = atoi(tmp.c_str()); 
					Stage_Initialize();
				}
				width++;
			}
			else {
				// �������int�^�ɕύX
				int num = atoi(tmp.c_str());
				// �f�[�^��ۑ�
				cell[height][width].map_gh = num;
				// �n�`���ʂ��Z�b�g
				SetTerrainParam(width, height, cell[height][width].map_gh);
				// ���̕�����
				width++;
			}
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

// �n�`���ʂ��Z�b�g����
void StageCreate::SetTerrainParam(int x, int y, int paramData)
{
	TERRAIN_PARAM _param;
	fstream file;
	switch (paramData)
	{
	// ��
	case 0:
		file.open(FileManager::Instance()->GetDataName(TERRAIN_GATE), ios::in | ios::binary);
		file.read((char*)&_param, sizeof(_param));
		terrain[y][x].DEF = _param.DEF;
		terrain[y][x].AVO = _param.AVO;

		// ���O��ۑ�
		cell[y][x].map_str = FileManager::Instance()->GetFileName(TERRAIN_GATE);

		file.close();
		break;
	// �X
	case -2:
		file.open(FileManager::Instance()->GetDataName(TERRAIN_FOREST), ios::in | ios::binary);
		file.read((char*)&_param, sizeof(_param));
		terrain[y][x].DEF = _param.DEF;
		terrain[y][x].AVO = _param.AVO;

		// ���O��ۑ�
		cell[y][x].map_str = FileManager::Instance()->GetFileName(TERRAIN_FOREST);

		file.close();
		break;
	// ��
	case -3:
		file.open(FileManager::Instance()->GetDataName(TERRAIN_HOUSE), ios::in | ios::binary);
		file.read((char*)&_param, sizeof(_param));
		terrain[y][x].DEF = _param.DEF;
		terrain[y][x].AVO = _param.AVO;

		// ���O��ۑ�
		cell[y][x].map_str = FileManager::Instance()->GetFileName(TERRAIN_HOUSE);

		file.close();
		break;
	// ��
	case -4:
		file.open(FileManager::Instance()->GetDataName(TERRAIN_FORT), ios::in | ios::binary);
		file.read((char*)&_param, sizeof(_param));
		terrain[y][x].DEF = _param.DEF;
		terrain[y][x].AVO = _param.AVO;

		// ���O��ۑ�
		cell[y][x].map_str = FileManager::Instance()->GetFileName(TERRAIN_FORT);

		file.close();
		break;
	// �n�`���ʂȂ�
	default:
		terrain[y][x].DEF = 0;
		terrain[y][x].AVO = 0;
		cell[y][x].map_str = "--";
		break;
	}
}

// �n�`���ʂ��擾����
int StageCreate::GetTerrainParam(int x, int y, string _param)
{
	int returnParam = 0;
	// ����̒n�`���ʂ��Z�b�g
	if (_param == "DEF") {
		returnParam = terrain[y][x].DEF;
	}
	// ����̒n�`���ʂ��Z�b�g
	else if (_param == "AVO") {
		returnParam = terrain[y][x].AVO;
	}

	// �n�`���ʂ̒l��Ԃ�
	return returnParam;
}
