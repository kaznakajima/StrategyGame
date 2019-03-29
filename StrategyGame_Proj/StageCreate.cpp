#include "StageCreate.h"

// マップの構造体
struct Cell {
	// ステージの情報
	int map_gh;
	string map_str;
};

// マップの状態を保存
vector<vector<Cell>> cell;

StageCreate::StageCreate()
{
	stageList = vector<vector<int>>(MAP_SIZEY, vector<int>(MAP_SIZEX, 0));
	onUnit = vector<vector<string>>(MAP_SIZEY, vector<string>(MAP_SIZEX, "NONE"));
	checkMove = vector<vector<bool>>(MAP_SIZEY, vector<bool>(MAP_SIZEX, false));
	cell = vector<vector<Cell>>(MAP_SIZEY, vector<Cell>(MAP_SIZEX));
}

void StageCreate::Open(string pass)
{
	// ファイルの列
	int width = 0;
	// ファイルの行
	int height = 0;

	ifstream ifs(pass);
	string str = "";

	// ファイルの中身を一行ずつ読み取る
	while (getline(ifs, str)) {
		string tmp = "";
		istringstream stream(str);

		while (getline(stream, tmp, ',')) {
			// 文字列を保存
			cell[height][width].map_str = tmp;
			// 文字列をint型に変更
			int num = atoi(cell[height][width].map_str.c_str());
			// データを保存
			cell[height][width].map_gh = num;
			// 次の文字へ
			width++;
		}

		// 先頭へ
		width = 0;
		// 改行
		height++;
	}
}

// 読み込んだステージ情報を取得
int StageCreate::GetData(int x, int y) 
{
	return cell[y][x].map_gh;
}

// ステージ上の情報を更新
void StageCreate::StageUpdate(int x, int y)
{
	stageList[y][x] = GetData(x, y);
	onUnit[y][x] = "NONE";
	checkMove[y][x] = false;
}

// ユニットのいるマス目をチェック
void StageCreate::CheckOnUnit(int x, int y, string str)
{
	onUnit[y][x] = str;
}
