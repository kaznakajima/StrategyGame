#pragma once
#include "DxLib.h"
#include "Singleton.h"
#include "FileManager.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

// ステージデータファイル
#define STAGE_FILE_1 "Resources\\StageData\\StageData.csv"

// 地形データファイル
#define TERRAIN_FOREST "forest.dat_t"
#define TERRAIN_HOUSE "home.dat_t"
#define TERRAIN_FORT "fort.dat_t"
#define TERRAIN_GATE "gate.dat_t"

// 画像ファイル
#define TERRAIN_DETAIL "TerrainDetail.png"

// ウインドウサイズ
#define SCREEN_WIDTH 672
#define SCREEN_HEIGHT 480

// ステージサイズ
#define STAGE1_WIDTH 720
#define STAGE1_HEIGHT 480

// チップサイズ
#define CHIP_SIZE 48

class StageCreate : public Singleton <StageCreate>
{
public:
	StageCreate();

	// ユニットの初期位置
	vector<int> p_InitPos;
	vector<int> e_InitPos;

	// ステージサイズ
	int MAP_SIZEX = 15;
	int MAP_SIZEY = 10;

	// 生成するべきプレイヤーの数
	int playerCount = 2, enemyCount = 2;

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

	// 地形の詳細情報の表示
	void DrawTerrainDetail(int x, int y);

	// 地形効果を返す
	int GetTerrainParam(int x, int y, string _param);

private:
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

	// 地形効果をセットする
	void SetTerrainParam(int x, int y, int paramData);
};