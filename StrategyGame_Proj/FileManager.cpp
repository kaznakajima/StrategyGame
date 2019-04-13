#include "FileManager.h"
#include "DxLib.h"

FileManager::FileManager() { }
FileManager::~FileManager() { }

// 初期化
void FileManager::Initialize()
{
	InitGraph();
	InitSoundMem();
	fileHandleMap.clear();
}

// ファイルの取得
int FileManager::GetFileHandle(string key)
{
	//読み込み済み
	if (LoadCheck(key)) return fileHandleMap.at(key);
	//読み込んでない場合
	return LoadFile(key);
}

// ファイルのロード
int FileManager::LoadFile(string key)
{
	if (LoadCheck(key)) return 0;

	string directory, extension;
	int handle;

	//拡張子
	extension = GetExtension(key);

	//読み込み
	if (extension == "png" || extension == "jpg" || extension == "bmp") {
		//グラフィック
		directory = GRAPH_DIRECTORY + key;
		handle = LoadGraph(directory.c_str());
	}
	else if (extension == "mp3" || extension == "wave" || extension == "ogg") {
			//オーディオ
			directory = AUDIO_DIRECTORY + key;
			handle = LoadSoundMem(directory.c_str());
	}
	else extension = "error";


	if (extension == "error") {
		printfDx("Load:Error");
		return -1;
	}

	//保存
	fileHandleMap.emplace(key, handle);
	return handle;
}

// ファイルのロード(分割)
void FileManager::LoadFile(string file, int numAll, int numX, int numY, int sizeX, int sizeY)
{
	const int NUM_ALL_MAX = 64;	//分割数の最大
	string directory = GRAPH_DIRECTORY + file;
	int graphArr[NUM_ALL_MAX];

	//読み込み
	LoadDivGraph(directory.c_str(), numAll, numX, numY, sizeX, sizeY, graphArr);

	//保存
	string key = GetFileName(file) + "_";
	for (int i = 0; i < numAll; i++) {
		fileHandleMap.emplace(key + to_string(i), graphArr[i]);
	}
}

// データネームの取得
string FileManager::GetDataName(string file)
{
	string directory, extension;
	int handle;

	//拡張子
	extension = GetExtension(file);

	if (extension == "dat") {
		directory = CHARADATA_DIRECTORY + file;
	}
	else if (extension == "dat_t") {
		directory = TERRAINDATA_DIRECTORY + file;
	}

	return directory;
}

// ロードできたかどうか
bool FileManager::LoadCheck(string key)
{
	//まだ何も読み込んでない時
	if (fileHandleMap.empty()) return false;
	//探索
	auto handle = fileHandleMap.find(key);
	if (handle == fileHandleMap.end()) return false;
	return true;
}

// 拡張子を抽出
string FileManager::GetExtension(string file)
{
	auto fileEnd = file.find(".");
	if (fileEnd == string::npos) return "error";

	file.erase(file.begin(), file.begin() + (int)fileEnd + 1);
	return file;
}

// ファイル名の取得(拡張子は除く)
string FileManager::GetFileName(string file)
{
	auto fileEnd = file.find(".");
	if (fileEnd == string::npos) return "error";

	file.erase(file.begin() + (int)fileEnd, file.end());
	return file;
}