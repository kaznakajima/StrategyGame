#include "FileManager.h"
#include "DxLib.h"

FileManager::FileManager() { }
FileManager::~FileManager() { }

// ������
void FileManager::Initialize()
{
	InitGraph();
	InitSoundMem();
	fileHandleMap.clear();
}

// �t�@�C���̎擾
int FileManager::GetFileHandle(string key)
{
	//�ǂݍ��ݍς�
	if (LoadCheck(key)) return fileHandleMap.at(key);
	//�ǂݍ���łȂ��ꍇ
	return LoadFile(key);
}

// �t�@�C���̃��[�h
int FileManager::LoadFile(string key)
{
	if (LoadCheck(key)) return 0;

	string directory, extension;
	int handle;

	//�g���q
	extension = GetExtension(key);

	//�ǂݍ���
	if (extension == "png" || extension == "jpg" || extension == "bmp") {
		//�O���t�B�b�N
		directory = GRAPH_DIRECTORY + key;
		handle = LoadGraph(directory.c_str());
	}
	else if (extension == "mp3" || extension == "wave" || extension == "ogg") {
			//�I�[�f�B�I
			directory = AUDIO_DIRECTORY + key;
			handle = LoadSoundMem(directory.c_str());
	}
	else extension = "error";


	if (extension == "error") {
		printfDx("Load:Error");
		return -1;
	}

	//�ۑ�
	fileHandleMap.emplace(key, handle);
	return handle;
}

// �t�@�C���̃��[�h(����)
void FileManager::LoadFile(string file, int numAll, int numX, int numY, int sizeX, int sizeY)
{
	const int NUM_ALL_MAX = 64;	//�������̍ő�
	string directory = GRAPH_DIRECTORY + file;
	int graphArr[NUM_ALL_MAX];

	//�ǂݍ���
	LoadDivGraph(directory.c_str(), numAll, numX, numY, sizeX, sizeY, graphArr);

	//�ۑ�
	string key = GetFileName(file) + "_";
	for (int i = 0; i < numAll; i++) {
		fileHandleMap.emplace(key + to_string(i), graphArr[i]);
	}
}

// �f�[�^�l�[���̎擾
string FileManager::GetDataName(string file)
{
	string directory, extension;
	int handle;

	//�g���q
	extension = GetExtension(file);

	if (extension == "dat") {
		directory = CHARADATA_DIRECTORY + file;
	}
	else if (extension == "dat_t") {
		directory = TERRAINDATA_DIRECTORY + file;
	}

	return directory;
}

// ���[�h�ł������ǂ���
bool FileManager::LoadCheck(string key)
{
	//�܂������ǂݍ���łȂ���
	if (fileHandleMap.empty()) return false;
	//�T��
	auto handle = fileHandleMap.find(key);
	if (handle == fileHandleMap.end()) return false;
	return true;
}

// �g���q�𒊏o
string FileManager::GetExtension(string file)
{
	auto fileEnd = file.find(".");
	if (fileEnd == string::npos) return "error";

	file.erase(file.begin(), file.begin() + (int)fileEnd + 1);
	return file;
}

// �t�@�C�����̎擾(�g���q�͏���)
string FileManager::GetFileName(string file)
{
	auto fileEnd = file.find(".");
	if (fileEnd == string::npos) return "error";

	file.erase(file.begin() + (int)fileEnd, file.end());
	return file;
}