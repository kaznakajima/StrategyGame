#include "StageCreate.h"

// コンストラクタ
StageCreate::StageCreate()
{
	
}

// ステージの初期化
void StageCreate::Stage_Initialize()
{
	// 配列の初期化
	stageList = vector<vector<int>>(MAP_SIZEY, vector<int>(MAP_SIZEX, 0));
	onUnit = vector<vector<string>>(MAP_SIZEY, vector<string>(MAP_SIZEX, "NONE"));
	checkMove = vector<vector<bool>>(MAP_SIZEY, vector<bool>(MAP_SIZEX, false));
	cell = vector<vector<Cell>>(MAP_SIZEY, vector<Cell>(MAP_SIZEX));
	terrain = vector<vector<TERRAIN_PARAM>>(MAP_SIZEY, vector<TERRAIN_PARAM>(MAP_SIZEX));
	p_InitPos.clear();
	e_InitPos.clear();
}

void StageCreate::Open(string pass)
{
	// ファイルの列
	int width = 0;
	// ファイルの行
	int height = -1;

	ifstream ifs(pass);
	string str = "";

	// ファイルの中身を一行ずつ読み取る
	while (getline(ifs, str)) {
		string tmp = "";
		istringstream stream(str);

		// カンマ区切りで文字列取得
		while (getline(stream, tmp, ',')) {
			// マップサイズ取得
			if (height == -1) {
				if (width == 0) MAP_SIZEX = atoi(tmp.c_str());
				else { 
					MAP_SIZEY = atoi(tmp.c_str()); 
					Stage_Initialize();
				}
				width++;
			}
			// ステージデータ取得
			else {
				// 文字列をint型に変更
				int num = atoi(tmp.c_str());
				// データを保存
				cell[height][width].map_gh = num;
				// 地形効果をセット
				SetTerrainParam(width, height, cell[height][width].map_gh);
				// 次の文字へ
				width++;
			}
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

// 地形の詳細データ表示
void StageCreate::DrawTerrainDetail(int x, int y)
{	
	// 描画位置の補正用変数
	int drawOffset = 0;

	// 自身の位置を見て、描画位置に補正をかける
	if (x > STAGE1_WIDTH / 2) {
		DrawGraph(drawOffset, 405, FileManager::Instance()->GetFileHandle(TERRAIN_DETAIL), true);
	}
	else if (x < STAGE1_WIDTH / 2) {
		drawOffset = 572;
		DrawGraph(drawOffset, 405, FileManager::Instance()->GetFileHandle(TERRAIN_DETAIL), true);
	}

	// 地形データ表示
	DrawFormatString(drawOffset + 30, 410, GetColor(0, 0, 0), cell[y / CHIP_SIZE][x / CHIP_SIZE].map_str.c_str());
	DrawFormatString(drawOffset + 10, 435, GetColor(0, 0, 0), "DEF");
	DrawFormatString(drawOffset + 10, 460, GetColor(0, 0, 0), "AVO");
	DrawFormatString(drawOffset + 75, 435, GetColor(0, 0, 0), "%d", terrain[y / CHIP_SIZE][x / CHIP_SIZE].DEF);
	DrawFormatString(drawOffset + 75, 460, GetColor(0, 0, 0), "%d", terrain[y / CHIP_SIZE][x / CHIP_SIZE].AVO);

}

// 地形効果をセットする
void StageCreate::SetTerrainParam(int x, int y, int paramData)
{
	TERRAIN_PARAM _param;
	fstream file;
	switch (paramData)
	{
	// プレイヤーの初期値に追加
	// 地形効果なし
	case 0:
		cell[y][x].map_str = "--";
		p_InitPos.push_back(x * CHIP_SIZE);
		p_InitPos.push_back(y * CHIP_SIZE);
		break;
	// 敵の初期値に追加
	// 地形効果なし
	case -10:
		cell[y][x].map_str = "--";
		e_InitPos.push_back(x * CHIP_SIZE);
		e_InitPos.push_back(y * CHIP_SIZE);
		break;
	// 門
	case -2:
		file.open(FileManager::Instance()->GetDataName(TERRAIN_GATE), ios::in | ios::binary);
		file.read((char*)&_param, sizeof(_param));
		terrain[y][x].DEF = _param.DEF;
		terrain[y][x].AVO = _param.AVO;

		// 門の前には必ず敵がいる
		e_InitPos.push_back(x * CHIP_SIZE);
		e_InitPos.push_back(y * CHIP_SIZE);

		// 名前を保存
		cell[y][x].map_str = FileManager::Instance()->GetFileName(TERRAIN_GATE);

		file.close();
		break;
	// 森
	case -3:
		file.open(FileManager::Instance()->GetDataName(TERRAIN_FOREST), ios::in | ios::binary);
		file.read((char*)&_param, sizeof(_param));
		terrain[y][x].DEF = _param.DEF;
		terrain[y][x].AVO = _param.AVO;

		// 名前を保存
		cell[y][x].map_str = FileManager::Instance()->GetFileName(TERRAIN_FOREST);

		file.close();
		break;
	// 家
	case -4:
		file.open(FileManager::Instance()->GetDataName(TERRAIN_HOUSE), ios::in | ios::binary);
		file.read((char*)&_param, sizeof(_param));
		terrain[y][x].DEF = _param.DEF;
		terrain[y][x].AVO = _param.AVO;

		// 名前を保存
		cell[y][x].map_str = FileManager::Instance()->GetFileName(TERRAIN_HOUSE);

		file.close();
		break;
	// 砦
	case -5:
		file.open(FileManager::Instance()->GetDataName(TERRAIN_FORT), ios::in | ios::binary);
		file.read((char*)&_param, sizeof(_param));
		terrain[y][x].DEF = _param.DEF;
		terrain[y][x].AVO = _param.AVO;

		// 名前を保存
		cell[y][x].map_str = FileManager::Instance()->GetFileName(TERRAIN_FORT);

		file.close();
		break;
	// 地形効果なし
	default:
		cell[y][x].map_str = "--";
		break;
	}
}

// 地形効果を取得する
int StageCreate::GetTerrainParam(int x, int y, string _param)
{
	int returnParam = 0;
	// 守備の地形効果をセット
	if (_param == "DEF") {
		returnParam = terrain[y][x].DEF;
	}
	// 回避の地形効果をセット
	else if (_param == "AVO") {
		returnParam = terrain[y][x].AVO;
	}

	// 地形効果の値を返す
	return returnParam;
}
