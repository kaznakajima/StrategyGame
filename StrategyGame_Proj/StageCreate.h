#pragma once
#include "DxLib.h"
#include "Singleton.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

#define STAGE_FILE_1 "Resources\\StageData\\StageData.csv"
#define BACKGROUND "Resources\\image\\Field.png"

#define SCREEN_WIDTH 672
#define SCREEN_HEIGHT 480

#define STAGE1_WIDTH 720
#define STAGE1_HEIGHT 480

#define CHIP_SIZE 48

/*
Mapクラス
MapChipの読み込みをする(テスト)

 使い方(例)
  include して
  Map::Open() でファイルを開き
  Map::Draw() で描画
*/

class StageCreate : public Singleton <StageCreate>
{
public:
	StageCreate();

	// ステージサイズ
	int MAP_SIZEX = 15;
	int MAP_SIZEY = 10;

	// ステージのデータ
	vector<vector<int>> stageList;

	// ユニットがいるかどうか
	vector<vector<string>> onUnit;

	// 移動可能かどうか
	vector<vector<bool>> checkMove;

	// ファイルを開く (string ファイル名)
	void Open(string pass);

	// ステージ上のデータを取得
	int GetData(int x, int y);

	// ステージの状態を更新
	void StageUpdate(int x, int y);

	// ユニットがいるマス目をチェック
	void CheckOnUnit(int x, int y, string str);
};