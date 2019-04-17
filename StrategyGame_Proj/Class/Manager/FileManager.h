#pragma once

#include "Singleton.h"
#include <map>
#include <string>

using namespace std;

#define GRAPH_DIRECTORY "Resources\\Image\\"
#define AUDIO_DIRECTORY "Resources\\Audio\\"
#define EFFECT_DIRECTORY "Resources\\Effect\\"
#define CHARADATA_DIRECTORY "Resources\\CharacterData\\"
#define TERRAINDATA_DIRECTORY "Resources\\StageData\\TerrainData\\"

// �摜�A���y�����Ǘ�����N���X
class FileManager : public Singleton<FileManager>
{
public:
	FileManager();
	~FileManager();

	void Initialize();
	int GetFileHandle(string);
	//File�̓ǂݍ���
	int LoadFile(string key);
	void LoadFile(string file, int numAll, int numX, int numY, int sizeX, int sizeY);

	// �f�[�^�l�[���̎擾
	string GetDataName(string file);

	bool LoadCheck(string key);			//�ǂݍ��݂̗L��
	string GetExtension(string file);	//�g���q�̒��o
	string GetFileName(string file);	//�g���q�̔r��
private:

	// �t�@�C���̕ۑ���
	map<std::string, int> fileHandleMap;
};