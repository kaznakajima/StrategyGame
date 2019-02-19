#pragma once
#include "DxLib.h"
#include "Singleton.h"
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

#define STAGE_FILE_1 "Resources\\StageData\\StageData.csv"
#define BACKGROUND "Resources\\image\\Field.png"

#define SCREEN_WIDTH 672
#define SCREEN_HEIGHT 480

#define STAGE1_WIDTH 720
#define STAGE1_HEIGHT 480

#define STAGE_LONG_SIZE 100

#define MAP_SIZEX 15
#define MAP_SIZEY 10

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
	// ステージのデータ
	int stageList[MAP_SIZEY][MAP_SIZEX];

	// ユニットがいるかどうか
	bool onUnit[MAP_SIZEY][MAP_SIZEX];

	// 移動可能かどうか
	bool checkMove[MAP_SIZEY][MAP_SIZEX];

	// ファイルを開く (string ファイル名 int マップのサイズ)
	static void Open(string);

	static void Draw(int);

	// ステージ上のデータを取得
	static int GetData(int x, int y);

	// ステージの状態を更新
	void StageUpdate(int x, int y);
};