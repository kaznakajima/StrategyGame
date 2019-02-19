#include "Character.h"
#include "KeyInput.h"
#include <time.h>

#define PI 3.141592654

// 移動範囲
int MoveArea;
// 攻撃範囲
int AttackArea;
// 攻撃情報
int AttackDetail;
// 移動経路
int ArrowImage[6];

// 移動できるエリア
int moveToPos[MAP_SIZEY][MAP_SIZEX];

// 移動先の道筋かどうか
bool moveArrow[MAP_SIZEY][MAP_SIZEX];

// ステージデータを読み込むクラスのインスタンス
StageCreate* stage;

// パラメータ
int _param[9];

KeyInput* keyInput;

Character::Character()
{
	MoveArea = LoadGraph("CanMove.png");
	AttackArea = LoadGraph("CanAttack.png");
	AttackDetail = LoadGraph("AttackGraph.png");
	LoadDivGraph("Arrow.png", 6, 6, 1, CHIP_SIZE, CHIP_SIZE, ArrowImage);

	// インスタンス作成
	stage = StageCreate::Instance();
	keyInput = KeyInput::Instance();

	for (int y = 0; y < MAP_SIZEY; y++) {
		for (int x = 0; x < MAP_SIZEX; x++) {
			moveToPos[y][x] = -1;
			moveArrow[y][x] = false;
			stage->StageUpdate(x, y);
		}
	}
}

 void Character::Character_Initialize(STATUS* status, string pass, int posX, int posY)
{
	 GetCharacterParam(pass);

	 // ステータス設定
	 status->PosX = posX;
	 status->PosY = posY;
	 status->_PosX = posX;
	 status->_PosY = posY;
	 status->AnimHandle = 0;
	 status->AttackRange = 1;
	 LoadDivGraph("Character_40×40.png", 20, 4, 5, CHIP_SIZE, CHIP_SIZE, status->Image);

	 // パラメータ設定
	 status->myParam.HP = _param[(int)PLAYER_PARAM::HP];
	 status->myParam.POWER = _param[(int)PLAYER_PARAM::POWER];
	 status->myParam.TECHNIQUE = _param[(int)PLAYER_PARAM::TECHNIQUE];
	 status->myParam.SPEED = _param[(int)PLAYER_PARAM::SPEED];
	 status->myParam.LUCKY = _param[(int)PLAYER_PARAM::LUCKY];
	 status->myParam.DEFENCE = _param[(int)PLAYER_PARAM::DEFENCE];
	 status->myParam.MAGIC_DEFENCE = _param[(int)PLAYER_PARAM::MAGIC_DEFENCE];
	 status->myParam.PHYSIQUE = _param[(int)PLAYER_PARAM::PHYSIQUE];
	 status->myParam.MOVERANGE = _param[(int)PLAYER_PARAM::MOVERANGE];

	 // 必殺率 (技パラメータ / 2)
	 status->myParam.ATTACK_CLT = status->myParam.TECHNIQUE / 2;
	 // 回比率 (速さパラメータ * 2 + 幸運パラメータ)
	 status->myParam.ATTACK_AVO = status->myParam.SPEED * 2 + status->myParam.LUCKY;
	 // 命中率 (武器命中 + 技パラメータ * 2.5)
	 status->myParam.ATTACK_HIT = 85 + (int)(status->myParam.TECHNIQUE * 2.5);
}

 // パラメータ取得
 void Character::GetCharacterParam(string pass)
 {
	 int num = 0;

	 ifstream ifs(pass);
	 string str = "";
	 // ファイルの中身を一行ずつ読み取る
	while (getline(ifs, str)) {
		string tmp = "";
		istringstream stream(str);
		while (getline(stream, tmp, ',')) {
			_param[num] = atoi(tmp.c_str());
			num++;
		}
	}
 }

 // キャラクター周りの描画
 void Character::SpriteDraw(int x, int y, int img)
 {
	 DrawGraph(x, y, img, true);
 }

// キャラクターアニメーション
void Character::CharacterAnim(STATUS* status)
{
	// 選択状態ならアニメーション更新
	if (status->isSelect) {
		status->AnimHandle += 0.1f;
		if (status->AnimHandle > 7.0f && status->isMove == false)
			status->AnimHandle = 4.0f;
	}

	// xPos, yPosの位置にキャラクターを描画
	if (status->isAttack) {
		DrawGraph(status->PosX + (sin(PI * 2 / 120 * Count) + 1) / 2 * CHIP_SIZE, status->PosY, status->Image[(int)status->AnimHandle], true);
		Count++;
		if (Count == 60) {
			Count = 0;
			status->isAttack = false;
		}
	}
	else {
		DrawGraph(status->PosX, status->PosY, status->Image[(int)status->AnimHandle], true);
	}

	// 選択中でないユニットの位置にチェック
	if (status->isSelect == false) stage->onUnit[status->PosY / CHIP_SIZE][status->PosX / CHIP_SIZE] = true;
}

// キャラクターの移動
bool Character::CharacterMove(STATUS* status, int moveX, int moveY) 
{
	// 移動不能エリアを選択したらリターン
	if (stage->checkMove[moveY / CHIP_SIZE][moveX / CHIP_SIZE] == false 
		|| stage->onUnit[moveY / CHIP_SIZE][moveX / CHIP_SIZE] == true 
		|| status->PosX == moveX && status->PosY == moveY) {
		status->isSelect = false;
		status->isMove = false;
		status->AnimHandle = 0;
		AttackCheck(status);
		return false;
	}

	// 現在の地点の二次元配列用データ
	int valueX = moveX - status->_PosX;
	int valueY = moveY - status->_PosY;
	// 次目指す地点の二次元配列用データ
	int nextX = status->_PosX / CHIP_SIZE;
	int nextY = status->_PosY / CHIP_SIZE;
	
	// 自分のいる地点はチェック
	moveArrow[nextY][nextX] = false;

	// 右移動
	if (moveArrow[nextY][nextX + 1] == true) {
		// 画像の切り替え
		if (status->AnimHandle < 8.0f || status->AnimHandle > 11.0f)
			status->AnimHandle = 8.0f;

		status->PosX += 6;

		// 一マス分の移動
		if (status->PosX - status->_PosX == CHIP_SIZE) {
			status->_PosX = status->PosX;
			moveArrow[nextY][nextX + 1] = false;
		}

		// 最終地点に到達したら移動終了
		if (status->PosX == moveX && status->PosY == moveY) {
			status->isSelect = false;
			status->isMove = false;
			AttackCheck(status);
			status->AnimHandle = 0.0f;
			return false;
		}

		return true;
	}

	// 左移動
	if (moveArrow[nextY][nextX - 1] == true) {
		// 画像の切り替え
		if (status->AnimHandle < 12.0f || status->AnimHandle > 15.0f)
			status->AnimHandle = 12.0f;

		status->PosX -= 6;

		// 一マス分の移動
		if (status->PosX - status->_PosX == -CHIP_SIZE) {
			status->_PosX = status->PosX;
			moveArrow[nextY][nextX - 1] = false;
		}

		// 最終地点に到達したら移動終了
		if (status->PosX == moveX && status->PosY == moveY) {
			status->isSelect = false;
			status->isMove = false;
			AttackCheck(status);
			status->AnimHandle = 0.0f;
			return false;
		}

		return true;
	}

	// 下移動
	if (moveArrow[nextY + 1][nextX] == true) {
		// 画像の切り替え
		if (status->AnimHandle < 4.0f || status->AnimHandle > 7.0f)
			status->AnimHandle = 4.0f;

		status->PosY += 6;

		// 一マス分の移動
		if (status->PosY - status->_PosY == CHIP_SIZE) {
			status->_PosY = status->PosY;
			moveArrow[nextY + 1][nextX] = false;
		}

		// 最終地点に到達したら移動終了
		if (status->PosX == moveX && status->PosY == moveY) {
			status->isSelect = false;
			status->isMove = false;
			AttackCheck(status);
			status->AnimHandle = 0.0f;
			return false;
		}

		return true;
	}

	// 上移動
	if (moveArrow[nextY - 1][nextX] == true) {
		// 画像の切り替え
		if (status->AnimHandle < 16.0f || status->AnimHandle > 19.0f)
			status->AnimHandle = 16.0f;

		status->PosY -= 6;

		// 一マス分の移動
		if (status->PosY - status->_PosY == -CHIP_SIZE) {
			status->_PosY = status->PosY;
			moveArrow[nextY - 1][nextX] = false;
		}

		// 最終地点に到達したら移動終了
		if (status->PosX == moveX && status->PosY == moveY) {
			status->isSelect = false;
			status->isMove = false;
			AttackCheck(status);
			status->AnimHandle = 0.0f;
			return false;
		}

		return true;
	}

	return true;
}

// 移動範囲計算
void Character::MoveRange(STATUS* status, int x, int y, int moveCost)
{

	DrawFormatString(48, 48, GetColor(0, 255, 0), "HP [%d]", status->myParam.HP);

	// カメラとの差分
	float cameraX = keyInput->cameraPos.x, cameraY = keyInput->cameraPos.y;
	int offsetX = (int)cameraX / CHIP_SIZE, offsetY = (int)cameraY / CHIP_SIZE;

	// 現在の地点の二次元配列データアクセス用
	int valueX = x / CHIP_SIZE, valueY = y / CHIP_SIZE;
	// カメラの差分込みの二次元配列データアクセス用
	int _valueX = valueX + offsetX, _valueY = valueY + offsetY;

	// 画面外ならリターン
	if (valueX < 0 || valueX > 14 || valueY < 0 || valueY > 9)
		return;

	// 移動先情報を格納
	moveToPos[valueY][valueX] = moveCost;

	// もう動けないもしくは進めない場所の場合はリターン
	if (stage->stageList[_valueY][_valueX] > 0 && moveCost >= 0) {
		SpriteDraw(x, y, AttackArea);
		return;
	}
	else if (stage->stageList[_valueY][_valueX] > 0 || moveCost == 0) return;

	// 移動範囲表示していないなら表示
	if (stage->checkMove[valueY][valueX] != true) {
		stage->checkMove[valueY][valueX] = true;
		SpriteDraw(x, y, MoveArea);
	}

	// 上へ行けるならチェック
	if (moveToPos[valueY - 1][valueX] < moveCost) MoveRange(status, x, y - CHIP_SIZE, moveCost - 1);
	// 右へ行けるならチェック
	if (moveToPos[valueY][valueX + 1] < moveCost) MoveRange(status, x + CHIP_SIZE, y, moveCost - 1);
	// 左へ行けるならチェック
	if (moveToPos[valueY][valueX - 1] < moveCost)MoveRange(status, x - CHIP_SIZE, y, moveCost - 1);
	// 下へ行けるならチェック
	if (moveToPos[valueY + 1][valueX] < moveCost) MoveRange(status, x, y + CHIP_SIZE, moveCost - 1);
}

// 攻撃範囲表示
void Character::AttackRange()
{
	for (int y = 0; y < MAP_SIZEY; y++) {
		for (int x = 0; x < MAP_SIZEX; x++) {
			if (moveToPos[y][x] == 0) SpriteDraw(x * CHIP_SIZE, y * CHIP_SIZE, AttackArea);
		}
	}
}

// ガイドライン表示
void Character::DrawMoveArrow(STATUS status, int x, int y, int moveValue)
{
	// 現在の地点の二次元配列用データ
	int valueX = x / CHIP_SIZE, valueY = y / CHIP_SIZE;

	// 移動範囲でないなら表示しない
	if (stage->checkMove[valueY][valueX] == false) {
		OldPosX.clear();
		OldPosY.clear();
		moveCount++;
		moveArrow[valueY][valueX] = false;
	}

	// 入力順にガイドライン表示
	if (status.myParam.MOVERANGE - moveToPos[valueY][valueX] >= moveCount || OldPosX.size() == moveCount) {
		for (unsigned int num = 0; num < OldPosX.size(); num++) {
			if (stage->checkMove[OldPosY[num] / CHIP_SIZE][OldPosX[num] / CHIP_SIZE] == true) {
				DrawGraph(OldPosX[num], OldPosY[num], ArrowImage[moveValue], true);
				moveArrow[OldPosY[num] / CHIP_SIZE][OldPosX[num] / CHIP_SIZE] = true;
			}
		}
	}
	// 最短で行けるルートを表示
	else if(status.myParam.MOVERANGE - moveToPos[valueY][valueX] < moveCount || OldPosX.size() != moveCount) {

		// これまでの入力情報をクリア
		OldPosX.clear();
		OldPosY.clear();

		if (stage->checkMove[valueY][valueX] == true) {

			// ユニットの位置でないならここから逆探知
			if (status.PosX != x || status.PosY != y) {
				DrawGraph(x, y, ArrowImage[moveValue], true);
				moveArrow[y / CHIP_SIZE][x / CHIP_SIZE] = true;
			}

			// ユニットに向かってルートを逆探知していく
			if (moveToPos[valueY][valueX] + 1 == moveToPos[valueY + 1][valueX] && stage->checkMove[valueY + 1][valueX] == true) {
				DrawMoveArrow(status, x, y + CHIP_SIZE, 5);
				return;
			}
			if (moveToPos[valueY][valueX] + 1 == moveToPos[valueY - 1][valueX] && stage->checkMove[valueY - 1][valueX] == true) {
				DrawMoveArrow(status, x, y - CHIP_SIZE, 5);
				return;
			}
			if (moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX + 1] && stage->checkMove[valueY][valueX + 1] == true) {
				DrawMoveArrow(status, x + CHIP_SIZE, y, 5);
				return;
			}
			if (moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX - 1] && stage->checkMove[valueY][valueX - 1] == true) {
				DrawMoveArrow(status, x - CHIP_SIZE, y, 5);
				return;
			}
		}
	}
}

// 攻撃できるかチェック
void Character::AttackCheck(STATUS* status)
{
	status->canAttack = false;

	int valueX = status->PosX / CHIP_SIZE, valueY = status->PosY / CHIP_SIZE;
	if (stage->onUnit[valueY][valueX + 1] == true) status->canAttack = true;
	if (stage->onUnit[valueY][valueX - 1] == true) status->canAttack = true;
	if (stage->onUnit[valueY + 1][valueX] == true) status->canAttack = true;
	if (stage->onUnit[valueY - 1][valueX] == true) status->canAttack = true;
}

// 攻撃範囲描画
void Character::AttackableDraw(STATUS* status)
{
	int valueX = status->PosX / CHIP_SIZE, valueY = status->PosY / CHIP_SIZE;
	if (stage->onUnit[valueY][valueX + 1] == true) SpriteDraw(status->PosX + CHIP_SIZE, status->PosY, AttackArea);
	if (stage->onUnit[valueY][valueX - 1] == true) SpriteDraw(status->PosX - CHIP_SIZE, status->PosY, AttackArea);
	if (stage->onUnit[valueY + 1][valueX] == true) SpriteDraw(status->PosX, status->PosY + CHIP_SIZE, AttackArea);
	if (stage->onUnit[valueY - 1][valueX] == true) SpriteDraw(status->PosX, status->PosY - CHIP_SIZE, AttackArea);
}

// 攻撃の詳細表示
void Character::GetAttackDetail(STATUS* myStatus, STATUS* eStatus)
{
	// 威力 (攻撃側の力 - 守備側の守備力)
	int mySTR = myStatus->myParam.POWER - eStatus->myParam.DEFENCE;
	if (mySTR < 0) mySTR = 0;
	int eSTR = eStatus->myParam.POWER - myStatus->myParam.DEFENCE;
	if (eSTR < 0) eSTR = 0;

	// 速さ判定(4以上大きければ2回攻撃)
	int mySPD = myStatus->myParam.SPEED - eStatus->myParam.SPEED;

	// 命中力 (攻撃側の命中率 - 守備側の回避率)
	int myHitness = myStatus->myParam.ATTACK_HIT - eStatus->myParam.ATTACK_AVO;
	int eHitness = eStatus->myParam.ATTACK_HIT - myStatus->myParam.ATTACK_AVO;

	SpriteDraw(0, 0, AttackDetail);
	DrawFormatString(80, 100, GetColor(255, 255, 0), "HP");
	DrawFormatString(80, 125, GetColor(255, 255, 0), "威力");
	DrawFormatString(80, 150, GetColor(255, 255, 0), "命中");

	DrawFormatString(120, 100, GetColor(0, 0, 255), "%d", myStatus->myParam.HP);
	DrawFormatString(120, 125, GetColor(0, 0, 255), "%d", mySTR);
	if (mySPD >= 4) DrawFormatString(130, 130, GetColor(255, 255, 255), "×2");
	DrawFormatString(120, 150, GetColor(0, 0, 255), "%d", myHitness);

	DrawFormatString(45, 100, GetColor(0, 0, 255), "%d", eStatus->myParam.HP);
	DrawFormatString(45, 125, GetColor(0, 0, 255), "%d", eSTR);
	if (mySPD <= -4) DrawFormatString(55, 130, GetColor(255, 255, 255), "×2");
	DrawFormatString(45, 150, GetColor(0, 0, 255), "%d", eHitness);
}

// キャラクターの攻撃
void Character::CharacterAttack(STATUS* myStatus, STATUS* eStatus, int count) 
{
	myStatus->isAttack = true;

	// ダメージ計算
	int damage = myStatus->myParam.POWER - eStatus->myParam.DEFENCE;
	// マイナスは0
	if (damage < 0) damage = 0;

	// 速さ判定(4以上大きければ2回攻撃)
	int mySPD = myStatus->myParam.SPEED - eStatus->myParam.SPEED;

	// 命中力 (攻撃側の命中率 - 守備側の回避率)
	int myHitness = myStatus->myParam.ATTACK_HIT - eStatus->myParam.ATTACK_AVO;

	// 確率（命中力）
	double probability = myHitness;

	// 乱数の初期化
	srand((unsigned)time(NULL));
	if (GetRand(100) <= probability) {
		CharacterDamage(myStatus, eStatus, probability, damage);
		// 攻撃した相手を倒せたら戦闘終了
		if (eStatus->isDeath == true) {
			myStatus->canAttack = false;
			myStatus->isAttack = false;
			return;
		}
	}

	// 反撃
	if(count < 2 && myStatus->AttackRange == eStatus->AttackRange) CharacterAttack(eStatus, myStatus, 2);
	else if (count <= 2) {
		//	自分からの追撃
		if (mySPD >= 4) {
			CharacterAttack(myStatus, eStatus, 3);
			return;
		}
		// 敵からの追撃
		else if (mySPD <= -4 && myStatus->AttackRange == eStatus->AttackRange) {
			CharacterAttack(eStatus, myStatus, 3);
			return;
		}
	}

	// 追撃が出ないなら戦闘終了
	if (mySPD < 4 || mySPD > -4 || count < 2) myStatus->canAttack = false;
}

// 攻撃の処理
void Character::CharacterDamage(STATUS* myStatus, STATUS* eStatus, double probability, int damage)
{
	eStatus->myParam.HP -= damage;

	if (eStatus->myParam.HP < 0) {
		eStatus->isDeath = true;
	}
}

// 疑似的カメラとのオフセットの計算
void Character::SetCameraOffset(STATUS* status, int dir, bool horizontal)
{
	// 左右計算
	if (horizontal) {
		status->PosX += CHIP_SIZE * dir;
		status->_PosX = status->PosX;
	}
	// 上下計算
	else {
		status->PosY += CHIP_SIZE * dir;
		status->_PosY = status->PosY;
	}
}

void Character::MoveAreaClear()
{
	for (int y = 0; y < MAP_SIZEY; y++) {
		for (int x = 0; x < MAP_SIZEX; x++) {
			moveToPos[y][x] = -1;
			moveArrow[y][x] = false;
			stage->StageUpdate(x, y);
		}
	}
}

