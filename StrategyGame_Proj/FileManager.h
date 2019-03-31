#pragma once

#include "Singleton.h"
#include <map>
#include <string>

using namespace std;

#define GRAPH_DIRECTORY "Resources\\image\\"
#define AUDIO_DIRECTORY "Resources\\Audio\\"
#define EFFECT_DIRECTORY "Resources\\Effect\\"

// 画像、音楽等を管理するクラス
class FileManager : public Singleton<FileManager>
{
public:
	FileManager();
	~FileManager();

	void Initialize();
	int GetFileHandle(string);
	//Fileの読み込み
	int LoadFile(string key);
	void LoadFile(string file, int numAll, int numX, int numY, int sizeX, int sizeY);
private:

	bool LoadCheck(string key);			//読み込みの有無
	string GetExtension(string file);	//拡張子の抽出
	string GetFileName(string file);	//拡張子の排除

	// ファイルの保存先
	map<std::string, int> fileHandleMap;
};