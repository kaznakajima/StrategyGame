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

#define TERRAIN_FOREST "森.dat_t"
#define TERRAIN_HOUSE "家.dat_t"
#define TERRAIN_FORT "砦.dat_t"
#define TERRAIN_GATE "門.dat_t"

#define SCREEN_WIDTH 672
#define SCREEN_HEIGHT 480

#define STAGE1_WIDTH 720
#define STAGE1_HEIGHT 480

#define CHIP_SIZE 48

class StageCreate : public Singleton <StageCreate>
{
public:
	StageCreate();

	// マップの構造体
	struct Cell {
		// ステージの情報
		int map_gh;
		string map_str;
	};

	// 地形効果
	typedef struct {
		int DEF;   // 守備
		int AVO;  // 回避
	}  TERRAIN_PARAM;

	// マップの状態を保存
	vector<vector<Cell>> cell;
	// 地形効果を保存
	vector<vector<TERRAIN_PARAM>> terrain;

	// ステージサイズ
	int MAP_SIZEX = 15;
	int MAP_SIZEY = 10;

	// ステージのデータ
	vector<vector<int>> stageList;

	// ユニットがいるかどうか
	vector<vector<string>> onUnit;

	// 移動可能かどうか
	vector<vector<bool>> checkMove;

	// 初期化
	void Stage_Initialize();

	// ファイルを開く (string ファイル名)
	void Open(string pass);

	// ステージ上のデータを取得
	int GetData(int x, int y);

	// ステージの状態を更新
	void StageUpdate(int x, int y);

	// ユニットがいるマス目をチェック
	void CheckOnUnit(int x, int y, string str);

	// 地形効果をセットする
	void SetTerrainParam(int x, int y, int paramData);

	// 地形効果を返す
	int GetTerrainParam(int x, int y, string _param);
};