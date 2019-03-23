#include "Character.h"
#include "KeyInput.h"
#include <time.h>

#define PI 3.141592654

// コンストラクタ
Character::Character()
{
	HpBar = LoadGraph(HP_BAR);
	HpBarBox = LoadGraph(HP_BARBOX);
	MoveArea = LoadGraph(CAN_MOVE_AREA);
	AttackArea = LoadGraph(CAN_ATTACK_AREA);
	AttackDetail = LoadGraph(ATTACK_DETAIL);
	DamageDetail = LoadGraph(DAMAGE_DETAIL);
	LoadDivGraph(ARROW, 6, 6, 1, CHIP_SIZE, CHIP_SIZE, ArrowImage);


	// インスタンス作成
	stage = StageCreate::Instance();

	for (int y = 0; y < StageCreate::Instance()->MAP_SIZEY; y++) {
		for (int x = 0; x < StageCreate::Instance()->MAP_SIZEX; x++) {
			moveToPos[y][x] = -1;
			moveArrow[y][x] = false;
			stage->StageUpdate(x, y);
		}
	}
}

 void Character::Character_Initialize(string pass, string name, string team, int posX, int posY)
{
	 //if (status == nullptr) return;

	 GetCharacterParam(pass);

	 // ステータス設定
	 myStatus->PosX = posX;
	 myStatus->PosY = posY;
	 myStatus->_PosX = posX;
	 myStatus->_PosY = posY;
	 myStatus->AnimHandle = 0;
	 myStatus->AttackRange = 1;
	 myStatus->myTeam = team;
	 LoadDivGraph(CHARACTER_IMG, 20, 4, 5, CHIP_SIZE, CHIP_SIZE, myStatus->Image);

	 // パラメータ設定
	 myStatus->myParam.NAME = name;
	 myStatus->myParam.HP = _param[(int)PLAYER_PARAM::HP];
	 myStatus->myParam.POWER = _param[(int)PLAYER_PARAM::POWER];
	 myStatus->myParam.TECHNIQUE = _param[(int)PLAYER_PARAM::TECHNIQUE];
	 myStatus->myParam.SPEED = _param[(int)PLAYER_PARAM::SPEED];
	 myStatus->myParam.LUCKY = _param[(int)PLAYER_PARAM::LUCKY];
	 myStatus->myParam.DEFENCE = _param[(int)PLAYER_PARAM::DEFENCE];
	 myStatus->myParam.MAGIC_DEFENCE = _param[(int)PLAYER_PARAM::MAGIC_DEFENCE];
	 myStatus->myParam.PHYSIQUE = _param[(int)PLAYER_PARAM::PHYSIQUE];
	 myStatus->myParam.MOVERANGE = _param[(int)PLAYER_PARAM::MOVERANGE];

	 // 必殺率 (技パラメータ / 2)
	 myStatus->myParam.ATTACK_CLT = myStatus->myParam.TECHNIQUE / 2;
	 // 回比率 (速さパラメータ * 2 + 幸運パラメータ)
	 myStatus->myParam.ATTACK_AVO = myStatus->myParam.SPEED * 2 + myStatus->myParam.LUCKY;
	 // 命中率 (武器命中 + 技パラメータ * 2.5)
	 myStatus->myParam.ATTACK_HIT = 85 + (int)(myStatus->myParam.TECHNIQUE * 2.5);
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

 // 移動再開(ターン開始時に呼び出す)
 void Character::TurnStart()
 {
	 myStatus->canMove = true;
 }

 // キャラクター周りの描画
 void Character::SpriteDraw(int x, int y, int img)
 {
	 DrawGraph(x, y, img, true);
 }

// キャラクターアニメーション
void Character::CharacterAnim()
{
	if (myStatus == nullptr) return;

	// 選択状態ならアニメーション更新
	if (myStatus->isSelect) {
		myStatus->AnimHandle += 0.1f;
		if (myStatus->AnimHandle > 7.0f && myStatus->canMove)
			myStatus->AnimHandle = 4.0f;
	}

	// xPos, yPosの位置にキャラクターを描画
	DrawGraph(myStatus->PosX, myStatus->PosY, myStatus->Image[(int)myStatus->AnimHandle], true);

	// 選択中でないユニットの位置にチェック
	if (myStatus->isSelect == false) stage->onUnit[myStatus->PosY / CHIP_SIZE][myStatus->PosX / CHIP_SIZE] = myStatus->myTeam;
}

// キャラクターの移動
bool Character::CharacterMove(int moveX, int moveY) 
{
	if (myStatus == nullptr) return false;

	// 自分の位置を選択したら移動終了
	if (myStatus->PosX == moveX && myStatus->PosY == moveY) {
		myStatus->isSelect = false;
		myStatus->canMove = false;
		myStatus->AnimHandle = 0;
		AttackCheck();
		return false;
	}

	// 移動不能エリアを選択したらリターン
	if (stage->checkMove[moveY / CHIP_SIZE][moveX / CHIP_SIZE] == false
		|| stage->onUnit[moveY / CHIP_SIZE][moveX / CHIP_SIZE] != "NONE") {
		myStatus->isSelect = false;
		myStatus->AnimHandle = 0;
		return false;
	}

	myStatus->canMove = false;

	// 現在の地点の二次元配列用データ
	int valueX = moveX - myStatus->_PosX;
	int valueY = moveY - myStatus->_PosY;
	// 次目指す地点の二次元配列用データ
	int nextX = myStatus->_PosX / CHIP_SIZE;
	int nextY = myStatus->_PosY / CHIP_SIZE;
	
	// 自分のいる地点はチェック
	moveArrow[nextY][nextX] = false;

	// 右移動
	if (nextX + 1 <= 14 && moveArrow[nextY][nextX + 1] == true) {
		// 画像の切り替え
		if (myStatus->AnimHandle < 8.0f || myStatus->AnimHandle > 11.0f)
			myStatus->AnimHandle = 8.0f;

		myStatus->PosX += 6;

		// 一マス分の移動
		if (myStatus->PosX - myStatus->_PosX == CHIP_SIZE) {
			myStatus->_PosX = myStatus->PosX;
			moveArrow[nextY][nextX + 1] = false;
		}

		// 最終地点に到達したら移動終了
		if (myStatus->PosX == moveX && myStatus->PosY == moveY) {
			myStatus->isSelect = false;
			AttackCheck();
			myStatus->AnimHandle = 0.0f;
			return false;
		}

		return true;
	}

	// 左移動
	if (nextX - 1 >= 0 && moveArrow[nextY][nextX - 1] == true) {
		// 画像の切り替え
		if (myStatus->AnimHandle < 12.0f || myStatus->AnimHandle > 15.0f)
			myStatus->AnimHandle = 12.0f;

		myStatus->PosX -= 6;

		// 一マス分の移動
		if (myStatus->PosX - myStatus->_PosX == -CHIP_SIZE) {
			myStatus->_PosX = myStatus->PosX;
			moveArrow[nextY][nextX - 1] = false;
		}

		// 最終地点に到達したら移動終了
		if (myStatus->PosX == moveX && myStatus->PosY == moveY) {
			myStatus->isSelect = false;
			AttackCheck();
			myStatus->AnimHandle = 0.0f;
			return false;
		}

		return true;
	}

	// 下移動
	if (nextY + 1 <= 9 && moveArrow[nextY + 1][nextX] == true) {
		// 画像の切り替え
		if (myStatus->AnimHandle < 4.0f || myStatus->AnimHandle > 7.0f)
			myStatus->AnimHandle = 4.0f;

		myStatus->PosY += 6;

		// 一マス分の移動
		if (myStatus->PosY - myStatus->_PosY == CHIP_SIZE) {
			myStatus->_PosY = myStatus->PosY;
			moveArrow[nextY + 1][nextX] = false;
		}

		// 最終地点に到達したら移動終了
		if (myStatus->PosX == moveX && myStatus ->PosY == moveY) {
			myStatus->isSelect = false;
			AttackCheck();
			myStatus->AnimHandle = 0.0f;
			return false;
		}

		return true;
	}

	// 上移動
	if (nextY - 1 >= 0 && moveArrow[nextY - 1][nextX] == true) {
		// 画像の切り替え
		if (myStatus->AnimHandle < 16.0f || myStatus->AnimHandle > 19.0f)
			myStatus->AnimHandle = 16.0f;

		myStatus->PosY -= 6;

		// 一マス分の移動
		if (myStatus->PosY - myStatus->_PosY == -CHIP_SIZE) {
			myStatus->_PosY = myStatus->PosY;
			moveArrow[nextY - 1][nextX] = false;
		}

		// 最終地点に到達したら移動終了
		if (myStatus->PosX == moveX && myStatus->PosY == moveY) {
			myStatus->isSelect = false;
			AttackCheck();
			myStatus->AnimHandle = 0.0f;
			return false;
		}

		return true;
	}

	return true;
}

// 移動範囲計算
void Character::MoveRange(int x, int y, int moveCost)
{
	if (myStatus == nullptr) return;

	DrawFormatString(48, 48, GetColor(0, 255, 0), "HP [%d]", myStatus->myParam.HP);

	// カメラとの差分
	float cameraX = KeyInput::Instance()->cameraPos.x, cameraY = KeyInput::Instance()->cameraPos.y;
	int offsetX = (int)cameraX / CHIP_SIZE, offsetY = (int)cameraY / CHIP_SIZE;

	// 現在の地点の二次元配列データアクセス用
	int valueX = x / CHIP_SIZE, valueY = y / CHIP_SIZE;
	// カメラの差分込みの二次元配列データアクセス用
	int _valueX = valueX + offsetX, _valueY = valueY + offsetY;

	// 画面外ならリターン
	if (valueX < 0 || valueX > 14 || valueY < 0 || valueY > 9)
		return;
	// 画面外ならリターン
	if (_valueX < 0 || _valueX > 14 || _valueY < 0 || _valueY > 9)
		return;

	// 移動先情報を格納
	if (moveToPos[valueY][valueX] < moveCost) moveToPos[valueY][valueX] = moveCost;

	// もう動けないもしくは進めない場所の場合はリターン
	if (stage->stageList[_valueY][_valueX] > 0 && moveCost >= 0) {
		SpriteDraw(x, y, AttackArea); 
		moveToPos[valueY][valueX] = 0;
		return;
	}
	else if (stage->stageList[_valueY][_valueX] > 0 || moveCost == 0) return;

	// 移動範囲表示していないなら表示
	if (stage->checkMove[valueY][valueX] != true) {
		stage->checkMove[valueY][valueX] = true;
		SpriteDraw(x, y, MoveArea);
	}

	// 上へ行けるならチェック
	if (valueY - 1 >= 0 && moveToPos[valueY - 1][valueX] < moveCost) MoveRange(x, y - CHIP_SIZE, moveCost - 1);
	// 右へ行けるならチェック
	if (valueX + 1 <= 14 && moveToPos[valueY][valueX + 1] < moveCost) MoveRange(x + CHIP_SIZE, y, moveCost - 1);
	// 左へ行けるならチェック
	if (valueX - 1 >= 0 && moveToPos[valueY][valueX - 1] < moveCost)MoveRange(x - CHIP_SIZE, y, moveCost - 1);
	// 下へ行けるならチェック
	if (valueY + 1 <= 9 && moveToPos[valueY + 1][valueX] < moveCost) MoveRange(x, y + CHIP_SIZE, moveCost - 1);
}

// 攻撃範囲表示
void Character::AttackRange()
{
	for (int y = 0; y < StageCreate::Instance()->MAP_SIZEY; y++) {
		for (int x = 0; x < StageCreate::Instance()->MAP_SIZEX; x++) {
			if (moveToPos[y][x] == 0) SpriteDraw(x * CHIP_SIZE, y * CHIP_SIZE, AttackArea);
		}
	}
}

// ガイドライン表示
void Character::DrawMoveArrow(int x, int y, int moveValue)
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

	// 敵の行動
	if (myStatus->myTeam == "Enemy") {
		// これまでの入力情報をクリア
		OldPosX.clear();
		OldPosY.clear();

		if (stage->checkMove[valueY][valueX] == true) {

			// ユニットの位置でないならここから逆探知
			if (myStatus->PosX != x || myStatus->PosY != y) {
				DrawGraph(x, y, ArrowImage[moveValue], true);
				moveArrow[valueY][valueX] = true;
			}

			// ユニットに向かってルートを逆探知していく
			if (valueY + 1 < 9 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY + 1][valueX] && stage->checkMove[valueY + 1][valueX] == true) {
				DrawMoveArrow(x, y + CHIP_SIZE, 5);
				return;
			}
			if (valueY - 1 > 0 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY - 1][valueX] && stage->checkMove[valueY - 1][valueX] == true) {
				DrawMoveArrow(x, y - CHIP_SIZE, 5);
				return;
			}
			if (valueX + 1 < 14 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX + 1] && stage->checkMove[valueY][valueX + 1] == true) {
				DrawMoveArrow(x + CHIP_SIZE, y, 5);
				return;
			}
			if (valueX - 1 > 0 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX - 1] && stage->checkMove[valueY][valueX - 1] == true) {
				DrawMoveArrow(x - CHIP_SIZE, y, 5);
				return;
			}
		}
	}

	// 入力順にガイドライン表示
	if (myStatus->myParam.MOVERANGE - moveToPos[valueY][valueX] >= moveCount || OldPosX.size() == moveCount) {
		for (unsigned int num = 0; num < OldPosX.size(); num++) {
			if (stage->checkMove[OldPosY[num] / CHIP_SIZE][OldPosX[num] / CHIP_SIZE] == true) {
				DrawGraph(OldPosX[num], OldPosY[num], ArrowImage[moveValue], true);
				moveArrow[OldPosY[num] / CHIP_SIZE][OldPosX[num] / CHIP_SIZE] = true;
			}
		}
	}
	// 最短で行けるルートを表示
	else if(myStatus->myParam.MOVERANGE - moveToPos[valueY][valueX] < moveCount || OldPosX.size() != moveCount) {

		// これまでの入力情報をクリア
		OldPosX.clear();
		OldPosY.clear();

		if (stage->checkMove[valueY][valueX] == true) {

			// ユニットの位置でないならここから逆探知
			if (myStatus->PosX != x || myStatus->PosY != y) {
				DrawGraph(x, y, ArrowImage[moveValue], true);
				moveArrow[y / CHIP_SIZE][x / CHIP_SIZE] = true;
			}

			// ユニットに向かってルートを逆探知していく
			if (valueY + 1 < 9 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY + 1][valueX] && stage->checkMove[valueY + 1][valueX] == true) {
				DrawMoveArrow(x, y + CHIP_SIZE, 5);
				return;
			}
			if (valueY - 1 > 0 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY - 1][valueX] && stage->checkMove[valueY - 1][valueX] == true) {
				DrawMoveArrow(x, y - CHIP_SIZE, 5);
				return;
			}
			if (valueX + 1 < 14 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX + 1] && stage->checkMove[valueY][valueX + 1] == true) {
				DrawMoveArrow(x + CHIP_SIZE, y, 5);
				return;
			}
			if (valueX - 1 > 0 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX - 1] && stage->checkMove[valueY][valueX - 1] == true) {
				DrawMoveArrow(x - CHIP_SIZE, y, 5);
				return;
			}
		}
	}
}

// 攻撃できるかチェック
void Character::AttackCheck()
{
	if (myStatus == nullptr) return;

	myStatus->canAttack = false;

	int valueX = myStatus->PosX / CHIP_SIZE, valueY = myStatus->PosY / CHIP_SIZE;
	if (valueX + 1 <= 14 && stage->onUnit[valueY][valueX + 1] != "NONE" && stage->onUnit[valueY][valueX + 1] != myStatus->myTeam) myStatus->canAttack = true;
	if (valueX - 1 >= 0 && stage->onUnit[valueY][valueX - 1] != "NONE" && stage->onUnit[valueY][valueX - 1] != myStatus->myTeam) myStatus->canAttack = true;
	if (valueY + 1 <= 9 && stage->onUnit[valueY + 1][valueX] != "NONE" && stage->onUnit[valueY + 1][valueX] != myStatus->myTeam) myStatus->canAttack = true;
	if (valueY - 1 >= 0 && stage->onUnit[valueY - 1][valueX] != "NONE" && stage->onUnit[valueY - 1][valueX] != myStatus->myTeam) myStatus->canAttack = true;
}

// 攻撃範囲描画
void Character::AttackableDraw()
{
	int valueX = myStatus->PosX / CHIP_SIZE, valueY = myStatus->PosY / CHIP_SIZE;
	if (valueX + 1 <= 14 && stage->onUnit[valueY][valueX + 1] != "NONE" && stage->onUnit[valueY][valueX + 1] != myStatus->myTeam) SpriteDraw(myStatus->PosX + CHIP_SIZE, myStatus->PosY, AttackArea);
	if (valueX - 1 >= 0 && stage->onUnit[valueY][valueX - 1] != "NONE" && stage->onUnit[valueY][valueX - 1] != myStatus->myTeam) SpriteDraw(myStatus->PosX - CHIP_SIZE, myStatus->PosY, AttackArea);
	if (valueY + 1 <= 9 && stage->onUnit[valueY + 1][valueX] != "NONE" && stage->onUnit[valueY + 1][valueX] != myStatus->myTeam) SpriteDraw(myStatus->PosX, myStatus->PosY + CHIP_SIZE, AttackArea);
	if (valueY - 1 >= 0 && stage->onUnit[valueY - 1][valueX] != "NONE" && stage->onUnit[valueY - 1][valueX] != myStatus->myTeam) SpriteDraw(myStatus->PosX, myStatus->PosY - CHIP_SIZE, AttackArea);
}

// 攻撃の詳細表示
void Character::GetAttackDetail(Character* eStatus)
{
	// 威力 (攻撃側の力 - 守備側の守備力)
	int mySTR = myStatus->myParam.POWER - eStatus->myStatus->myParam.DEFENCE;
	if (mySTR < 0) mySTR = 0;
	int eSTR = eStatus->myStatus->myParam.POWER - myStatus->myParam.DEFENCE;
	if (eSTR < 0) eSTR = 0;

	// 速さ判定(4以上大きければ2回攻撃)
	int mySPD = myStatus->myParam.SPEED - eStatus->myStatus->myParam.SPEED;

	// 命中力 (攻撃側の命中率 - 守備側の回避率)
	int myHitness = myStatus->myParam.ATTACK_HIT - eStatus->myStatus->myParam.ATTACK_AVO;
	int eHitness = eStatus->myStatus->myParam.ATTACK_HIT - eStatus->myStatus->myParam.ATTACK_AVO;

	SpriteDraw(0, 0, AttackDetail);
	DrawFormatString(80, 100, GetColor(255, 255, 0), "HP");
	DrawFormatString(80, 125, GetColor(255, 255, 0), "威力");
	DrawFormatString(80, 150, GetColor(255, 255, 0), "命中");

	DrawFormatString(120, 100, GetColor(0, 0, 255), "%d", myStatus->myParam.HP);
	DrawFormatString(120, 125, GetColor(0, 0, 255), "%d", mySTR);
	if (mySPD >= 4) DrawFormatString(130, 130, GetColor(255, 255, 255), "×2");
	DrawFormatString(120, 150, GetColor(0, 0, 255), "%d", myHitness);

	DrawFormatString(45, 100, GetColor(0, 0, 255), "%d", eStatus->myStatus->myParam.HP);
	DrawFormatString(45, 125, GetColor(0, 0, 255), "%d", eSTR);
	if (mySPD <= -4) DrawFormatString(55, 130, GetColor(255, 255, 255), "×2");
	DrawFormatString(45, 150, GetColor(0, 0, 255), "%d", eHitness);
}

// 攻撃アニメーション
bool Character::AttackAnimation(Character* eCharacter, int count)
{
	DrawGraph(0, 0, DamageDetail, true);

	// 攻撃方向を取得
	int moveX = eCharacter->myStatus->PosX - myStatus->_PosX;
	int moveY = eCharacter->myStatus->PosY - myStatus->_PosY;

	if (moveX > 0) {
		// 攻撃アニメーション
		if(myStatus->animReset) myStatus->PosX -= 6;
		else myStatus->PosX += 6;
		
		if (myStatus->PosX - myStatus->_PosX == CHIP_SIZE) myStatus->animReset = true;
		if (myStatus->PosX == myStatus->_PosX) {
			CharacterAttack(eCharacter, count);
			myStatus->animReset = false;
			return false;
		}
	}
	else if (moveX < 0) {
		// 攻撃アニメーション
		if (myStatus->animReset) myStatus->PosX += 6;
		else myStatus->PosX -= 6;

		if (myStatus->PosX - myStatus->_PosX == -CHIP_SIZE) myStatus->animReset = true;
		if (myStatus->PosX == myStatus->_PosX) {
			CharacterAttack(eCharacter, count);
			myStatus->animReset = false;
			return false;
		}
	}
	if (moveY > 0) {
		// 攻撃アニメーション
		if (myStatus->animReset) myStatus->PosY -= 6;
		else myStatus->PosY += 6;

		if (myStatus->PosY - myStatus->_PosY == CHIP_SIZE) myStatus->animReset = true;
		if (myStatus->PosY == myStatus->_PosY) {
			CharacterAttack(eCharacter, count);
			myStatus->animReset = false;
			return false;
		}
	}
	else if (moveY < 0) {
		// 攻撃アニメーション
		if (myStatus->animReset) myStatus->PosY += 6;
		else myStatus->PosY -= 6;

		if (myStatus->PosY - myStatus->_PosY == -CHIP_SIZE) myStatus->animReset = true;
		if (myStatus->PosY == myStatus->_PosY) {
			CharacterAttack(eCharacter, count);
			myStatus->animReset = false;
			return false;
		}
	}

	return true;
}

// キャラクターの攻撃
void Character::CharacterAttack(Character* eCharacter, int count)
{
	// ダメージ計算
	int damage = myStatus->myParam.POWER - eCharacter->myStatus->myParam.DEFENCE;
	// マイナスは0
	if (damage < 0) damage = 0;

	// 速さ判定(4以上大きければ2回攻撃)
	myStatus->myParam.ATTACK_SPEED = myStatus->myParam.SPEED - eCharacter->myStatus->myParam.SPEED;

	// 命中力 (攻撃側の命中率 - 守備側の回避率)
	int myHitness = myStatus->myParam.ATTACK_HIT - eCharacter->myStatus->myParam.ATTACK_AVO;

	// 確率（命中力）
	double probability = myHitness;

	// 乱数の初期化
	srand((unsigned)time(NULL));
	if (GetRand(100) <= probability) {
		CharacterDamage(eCharacter, damage);
		// 敵が倒せたらその時点で戦闘終了
		if (eCharacter->myStatus->isDeath) {
			myStatus->isAttack = false;
			return;
		}
	}

	// 攻撃終了
	myStatus->isAttack = false;

	// 敵の反撃
	if (count < 2 && myStatus->AttackRange == eCharacter->myStatus->AttackRange) {
		eCharacter->myStatus->isAttack = true;
		return;
	}
	else if (count >= 2) {
		//	自分からの追撃
		if (myStatus->myParam.ATTACK_SPEED >= 4) {
			myStatus->isAttack = true;
			return;
		}
		// 敵からの追撃
		else if (eCharacter->myStatus->myParam.ATTACK_SPEED >= 4 && myStatus->AttackRange == eCharacter->myStatus->AttackRange) {
			eCharacter->myStatus->isAttack = true;
			return;
		}
	}

	// 追撃が出ないなら戦闘終了
	if (myStatus->myParam.ATTACK_SPEED < 4 || eCharacter->myStatus->myParam.ATTACK_SPEED < 4 || count < 2) myStatus->canAttack = false;
}

// 攻撃の処理
void Character::CharacterDamage(Character* eCharacter, int damage)
{
	eCharacter->myStatus->myParam.HP -= damage;

	if (eCharacter->myStatus->myParam.HP < 0) {
		eCharacter->myStatus->isDeath = true;
	}
	myStatus->canAttack = false;
}

// 疑似的カメラとのオフセットの計算
void Character::SetCameraOffset(int dir, bool horizontal)
{
	// 左右計算
	if (horizontal) {
		myStatus->PosX += CHIP_SIZE * dir;
		myStatus->_PosX = myStatus->PosX;
	}
	// 上下計算
	else {
		myStatus->PosY += CHIP_SIZE * dir;
		myStatus->_PosY = myStatus->PosY;
	}
}

void Character::MoveAreaClear(vector<Character*> _character)
{
	for (int y = 0; y < StageCreate::Instance()->MAP_SIZEY; y++) {
		for (int x = 0; x < StageCreate::Instance()->MAP_SIZEX; x++) {
			for (Character* character : _character) {
				moveToPos[y][x] = -1;
				moveArrow[y][x] = false;
				if(character->myStatus->PosX != x, character->myStatus->PosY != y) stage->StageUpdate(x, y);
			}
		}
	}
}

