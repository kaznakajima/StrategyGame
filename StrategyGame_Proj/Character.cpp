#include "Character.h"
#include "KeyInput.h"
#include <time.h>

#define PI 3.141592654

// コンストラクタ
Character::Character()
{
	itemCount = 0;

	AddItem(IRON_SWORD);

	FileManager::Instance()->GetFileHandle(CAN_MOVE_AREA);
	FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA);
	FileManager::Instance()->GetFileHandle(ATTACK_DETAIL);
	LoadDivGraph(ARROW, 6, 6, 1, CHIP_SIZE, CHIP_SIZE, ArrowImage);

	for (size_t y = 0; y < StageCreate::Instance()->MAP_SIZEY; y++) {
		for (size_t x = 0; x < StageCreate::Instance()->MAP_SIZEX; x++) {
			moveToPos[y][x] = -1;
			moveArrow[y][x] = false;
			StageCreate::Instance()->StageUpdate((int)x, (int)y);
		}
	}
}

 void Character::Character_Initialize(string pass, string name, string team, int posX, int posY)
{
	 //if (status == nullptr) return;

	 GetCharacterParam(pass);

	 // ステータス設定
	 myStatus->myData = pass;
	 myStatus->NAME = name;
	 myStatus->PosX = posX;
	 myStatus->PosY = posY;
	 myStatus->_PosX = posX;
	 myStatus->_PosY = posY;
	 myStatus->AnimHandle = 0;
	 myStatus->AttackRange = 1;
	 myStatus->myTeam = team;
	 LoadDivGraph(CHARACTER_IMG, 20, 4, 5, CHIP_SIZE, CHIP_SIZE, myStatus->Image);
}

 // パラメータ取得
 void Character::GetCharacterParam(string pass)
 {
	 fstream file;
	 file.open(pass, ios::in | ios::binary);
	 file.read((char*)&myStatus->myParam, sizeof(myStatus->myParam));

	 file.close();

	 // MaxHPの設定
	 myStatus->myParam.MaxHP = myStatus->myParam.HP;

	 // 攻撃速度
	 int weight = Item[0]->myParam.WEIGHT - myStatus->myParam.PHYSIQUE;
	 if (weight < 0) weight = 0;
	 myStatus->myParam.ATTACK_SPEED = myStatus->myParam.SPEED - weight;
	 // 必殺率 (技パラメータ / 2)
	 myStatus->myParam.ATTACK_CLT = myStatus->myParam.TECHNIQUE / 2 + Item[0]->myParam.CLT;
	 // 回避率 (速さパラメータ * 2 + 幸運パラメータ)
	 myStatus->myParam.ATTACK_AVO = myStatus->myParam.ATTACK_SPEED * 2 + myStatus->myParam.LUCKY;
	 // 命中率 (武器命中 + 技パラメータ * 2 + 幸運パラメータ / 2)
	 myStatus->myParam.ATTACK_HIT = Item[0]->myParam.HIT + (int)(myStatus->myParam.TECHNIQUE * 2) + (int)(myStatus->myParam.LUCKY / 2);
 }

 // 移動再開(ターン開始時に呼び出す)
 void Character::TurnStart()
 {
	 myStatus->canMove = true;
	 myStatus->AnimHandle = 0.0f;
	 myStatus->canAttack = false;
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
	else {
		if (myStatus->canMove == false && myStatus->canAttack == false) myStatus->AnimHandle = 3.0f; 
	}

	// xPos, yPosの位置にキャラクターを描画
	DrawGraph(myStatus->PosX, myStatus->PosY, myStatus->Image[(int)myStatus->AnimHandle], true);

	// 選択中でないユニットの位置にチェック
	if (myStatus->isSelect == false) StageCreate::Instance()->onUnit[myStatus->PosY / CHIP_SIZE][myStatus->PosX / CHIP_SIZE] = myStatus->myTeam;
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
	if (StageCreate::Instance()->checkMove[moveY / CHIP_SIZE][moveX / CHIP_SIZE] == false
		|| StageCreate::Instance()->onUnit[moveY / CHIP_SIZE][moveX / CHIP_SIZE] != "NONE") {
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
	if (StageCreate::Instance()->stageList[_valueY][_valueX] > 0 && moveCost >= 0) {
		SpriteDraw(x, y, FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA));
		moveToPos[valueY][valueX] = 0;
		return;
	}
	else if (StageCreate::Instance()->stageList[_valueY][_valueX] > 0 || moveCost == 0) return;

	// 移動範囲表示していないなら表示
	if (StageCreate::Instance()->checkMove[valueY][valueX] != true) {
		StageCreate::Instance()->checkMove[valueY][valueX] = true;
		SpriteDraw(x, y, FileManager::Instance()->GetFileHandle(CAN_MOVE_AREA));
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
			if (moveToPos[y][x] == 0) SpriteDraw(x * CHIP_SIZE, y * CHIP_SIZE, FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA));
		}
	}
}

// ガイドライン表示
void Character::DrawMoveArrow(int x, int y, int moveValue)
{
	// 現在の地点の二次元配列用データ
	int valueX = x / CHIP_SIZE, valueY = y / CHIP_SIZE;

	// 移動範囲でないなら表示しない
	if (StageCreate::Instance()->checkMove[valueY][valueX] == false) {
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

		if (StageCreate::Instance()->checkMove[valueY][valueX] == true) {

			// ユニットの位置でないならここから逆探知
			if (myStatus->PosX != x || myStatus->PosY != y) {
				DrawGraph(x, y, ArrowImage[moveValue], true);
				moveArrow[valueY][valueX] = true;
			}

			// ユニットに向かってルートを逆探知していく
			if (valueY + 1 < 9 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY + 1][valueX] && StageCreate::Instance()->checkMove[valueY + 1][valueX] == true) {
				DrawMoveArrow(x, y + CHIP_SIZE, 5);
				return;
			}
			if (valueY - 1 > 0 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY - 1][valueX] && StageCreate::Instance()->checkMove[valueY - 1][valueX] == true) {
				DrawMoveArrow(x, y - CHIP_SIZE, 5);
				return;
			}
			if (valueX + 1 < 14 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX + 1] && StageCreate::Instance()->checkMove[valueY][valueX + 1] == true) {
				DrawMoveArrow(x + CHIP_SIZE, y, 5);
				return;
			}
			if (valueX - 1 > 0 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX - 1] && StageCreate::Instance()->checkMove[valueY][valueX - 1] == true) {
				DrawMoveArrow(x - CHIP_SIZE, y, 5);
				return;
			}
		}
	}

	// 入力順にガイドライン表示
	if (myStatus->myParam.MOVERANGE - moveToPos[valueY][valueX] >= moveCount || OldPosX.size() == moveCount) {
		for (unsigned int num = 0; num < OldPosX.size(); num++) {
			if (StageCreate::Instance()->checkMove[OldPosY[num] / CHIP_SIZE][OldPosX[num] / CHIP_SIZE] == true) {
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

		if (StageCreate::Instance()->checkMove[valueY][valueX] == true) {

			// ユニットの位置でないならここから逆探知
			if (myStatus->PosX != x || myStatus->PosY != y) {
				DrawGraph(x, y, ArrowImage[moveValue], true);
				moveArrow[y / CHIP_SIZE][x / CHIP_SIZE] = true;
			}

			// ユニットに向かってルートを逆探知していく
			if (valueY + 1 < 9 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY + 1][valueX] && StageCreate::Instance()->checkMove[valueY + 1][valueX] == true) {
				DrawMoveArrow(x, y + CHIP_SIZE, 5);
				return;
			}
			if (valueY - 1 > 0 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY - 1][valueX] && StageCreate::Instance()->checkMove[valueY - 1][valueX] == true) {
				DrawMoveArrow(x, y - CHIP_SIZE, 5);
				return;
			}
			if (valueX + 1 < 14 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX + 1] && StageCreate::Instance()->checkMove[valueY][valueX + 1] == true) {
				DrawMoveArrow(x + CHIP_SIZE, y, 5);
				return;
			}
			if (valueX - 1 > 0 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX - 1] && StageCreate::Instance()->checkMove[valueY][valueX - 1] == true) {
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
	if (valueX + 1 <= 14 && StageCreate::Instance()->onUnit[valueY][valueX + 1] != "NONE" && StageCreate::Instance()->onUnit[valueY][valueX + 1] != myStatus->myTeam) myStatus->canAttack = true;
	if (valueX - 1 >= 0 && StageCreate::Instance()->onUnit[valueY][valueX - 1] != "NONE" && StageCreate::Instance()->onUnit[valueY][valueX - 1] != myStatus->myTeam) myStatus->canAttack = true;
	if (valueY + 1 <= 9 && StageCreate::Instance()->onUnit[valueY + 1][valueX] != "NONE" && StageCreate::Instance()->onUnit[valueY + 1][valueX] != myStatus->myTeam) myStatus->canAttack = true;
	if (valueY - 1 >= 0 && StageCreate::Instance()->onUnit[valueY - 1][valueX] != "NONE" && StageCreate::Instance()->onUnit[valueY - 1][valueX] != myStatus->myTeam) myStatus->canAttack = true;
}

// 攻撃範囲描画
void Character::AttackableDraw()
{
	if (myStatus->canMove) return;

	int valueX = myStatus->PosX / CHIP_SIZE, valueY = myStatus->PosY / CHIP_SIZE;
	if (valueX + 1 <= 14 && StageCreate::Instance()->onUnit[valueY][valueX + 1] != "NONE" && StageCreate::Instance()->onUnit[valueY][valueX + 1] != myStatus->myTeam) SpriteDraw(myStatus->PosX + CHIP_SIZE, myStatus->PosY, FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA));
	if (valueX - 1 >= 0 && StageCreate::Instance()->onUnit[valueY][valueX - 1] != "NONE" && StageCreate::Instance()->onUnit[valueY][valueX - 1] != myStatus->myTeam) SpriteDraw(myStatus->PosX - CHIP_SIZE, myStatus->PosY, FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA));
	if (valueY + 1 <= 9 && StageCreate::Instance()->onUnit[valueY + 1][valueX] != "NONE" && StageCreate::Instance()->onUnit[valueY + 1][valueX] != myStatus->myTeam) SpriteDraw(myStatus->PosX, myStatus->PosY + CHIP_SIZE, FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA));
	if (valueY - 1 >= 0 && StageCreate::Instance()->onUnit[valueY - 1][valueX] != "NONE" && StageCreate::Instance()->onUnit[valueY - 1][valueX] != myStatus->myTeam) SpriteDraw(myStatus->PosX, myStatus->PosY - CHIP_SIZE, FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA));
}

// 攻撃の詳細表示
void Character::GetAttackDetail(shared_ptr<Character> const &eCharacter)
{
	// 威力 (攻撃側の力 - 守備側の守備力)
	if (Item.empty() == false) myStatus->myParam.ATTACK_STR = myStatus->myParam.POWER + Item[0]->myParam.POWER;
	else myStatus->myParam.ATTACK_STR = 0;

	if (eCharacter->Item.empty() == false) eCharacter->myStatus->myParam.ATTACK_STR = eCharacter->myStatus->myParam.POWER + eCharacter->Item[0]->myParam.POWER;
	else eCharacter->myStatus->myParam.ATTACK_STR = 0;

	int mySTR = myStatus->myParam.ATTACK_STR - eCharacter->myStatus->myParam.DEFENCE;
	if (mySTR < 0) mySTR = 0;
	int eSTR = eCharacter->myStatus->myParam.ATTACK_STR - myStatus->myParam.DEFENCE;
	if (eSTR < 0) eSTR = 0;

	// 速さ判定(4以上大きければ2回攻撃)
	int mySPD = myStatus->myParam.SPEED - eCharacter->myStatus->myParam.SPEED;

	// 命中力 (攻撃側の命中率 - 守備側の回避率)
	int myHitness = myStatus->myParam.ATTACK_HIT - eCharacter->myStatus->myParam.ATTACK_AVO;
	if (myHitness > 100) myHitness = 100;
	int eHitness = eCharacter->myStatus->myParam.ATTACK_HIT - eCharacter->myStatus->myParam.ATTACK_AVO;
	if (eHitness > 100) eHitness = 100;

	int drawOffset = 0;

	if (myStatus->PosX > STAGE1_WIDTH / 2) {
		SpriteDraw(drawOffset, 0, FileManager::Instance()->GetFileHandle(ATTACK_DETAIL));
	}
	else if (myStatus->PosX < STAGE1_WIDTH / 2) {
		drawOffset = 480;
		SpriteDraw(drawOffset, 0, FileManager::Instance()->GetFileHandle(ATTACK_DETAIL));
	}

	DrawFormatString(80 + drawOffset, 100, GetColor(255, 255, 0), "HP");
	DrawFormatString(80 + drawOffset, 125, GetColor(255, 255, 0), "威力");
	DrawFormatString(80 + drawOffset, 150, GetColor(255, 255, 0), "命中");

	DrawFormatString(120 + drawOffset, 100, GetColor(0, 0, 255), "%d", myStatus->myParam.HP);
	DrawFormatString(120 + drawOffset, 175, GetColor(0, 0, 255), "%d", myStatus->myParam.MOVERANGE);
	DrawFormatString(45 + drawOffset, 65, GetColor(0, 0, 0), myStatus->NAME.c_str());
	DrawFormatString(120 + drawOffset, 125, GetColor(0, 0, 255), "%d", mySTR);
	if (mySPD >= 4) DrawFormatString(130 + drawOffset, 130, GetColor(255, 255, 255), "×2");
	DrawFormatString(120 + drawOffset, 150, GetColor(0, 0, 255), "%d", myHitness);

	DrawFormatString(45 + drawOffset, 100, GetColor(0, 0, 255), "%d", eCharacter->myStatus->myParam.HP);
	DrawFormatString(100 + drawOffset, 330, GetColor(255, 255, 255), eCharacter->myStatus->NAME.c_str());
	DrawFormatString(45 + drawOffset, 125, GetColor(0, 0, 255), "%d", eSTR);
	if (mySPD <= -4) DrawFormatString(55 + drawOffset, 130, GetColor(255, 255, 255), "×2");
	DrawFormatString(45 + drawOffset, 150, GetColor(0, 0, 255), "%d", eHitness);
}

// 攻撃アニメーション
bool Character::AttackAnimation(shared_ptr<Character> const &eCharacter, int count)
{

	// 攻撃方向を取得
	int moveX = eCharacter->myStatus->PosX - myStatus->_PosX;
	int moveY = eCharacter->myStatus->PosY - myStatus->_PosY;

	if (moveX > 0) {
		// 攻撃アニメーション
		if (myStatus->animReset) myStatus->PosX -= 6;
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
void Character::CharacterAttack(shared_ptr<Character> const &eCharacter, int count)
{
	// 威力 (攻撃側の力 - 守備側の守備力)
	if (Item.empty() == false) myStatus->myParam.ATTACK_STR = myStatus->myParam.POWER + Item[0]->myParam.POWER;
	else myStatus->myParam.ATTACK_STR = myStatus->myParam.POWER;

	if (eCharacter->Item.empty() == false) eCharacter->myStatus->myParam.ATTACK_STR = eCharacter->myStatus->myParam.POWER + eCharacter->Item[0]->myParam.POWER;
	else eCharacter->myStatus->myParam.ATTACK_STR = eCharacter->myStatus->myParam.POWER;

	// ダメージ計算
	int damage = myStatus->myParam.ATTACK_STR - eCharacter->myStatus->myParam.DEFENCE;
	// マイナスは0
	if (damage < 0) damage = 0;

	// 速さ判定(4以上大きければ2回攻撃)
	myStatus->myParam.ATTACK_SPEED = myStatus->myParam.SPEED - eCharacter->myStatus->myParam.SPEED;

	// 命中力 (攻撃側の命中率 - 守備側の回避率)
	int myHitness = myStatus->myParam.ATTACK_HIT - eCharacter->myStatus->myParam.ATTACK_AVO;
	if (myHitness > 100) myHitness = 100;

	// 確率（命中力）
	int probability = myHitness;

	// 乱数の初期化
	srand((unsigned)time(NULL));
	if (GetRand(100) <= probability) {
		int _damage = eCharacter->myStatus->myParam.HP - damage;
		CharacterDamage(eCharacter, _damage);
		AudioManager::Instance()->playSE(SE_DAMAGE);
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
}

// 攻撃の処理
void Character::CharacterDamage(shared_ptr<Character> const &eCharacter, int damage)
{
	if (eCharacter->myStatus->myParam.HP == damage && damage > 0)  return;

	eCharacter->myStatus->myParam.HP--;

	if (eCharacter->myStatus->myParam.HP <= 0) {
		eCharacter->myStatus->isDeath = true;
		return;
	}

	CharacterDamage(eCharacter, damage);
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

void Character::MoveAreaClear(vector<shared_ptr<Character>> const &_character)
{
	for (int y = 0; y < StageCreate::Instance()->MAP_SIZEY; y++) {
		for (int x = 0; x < StageCreate::Instance()->MAP_SIZEX; x++) {
			moveToPos[y][x] = -1;
			moveArrow[y][x] = false;
			StageCreate::Instance()->StageUpdate(x, y);
			for (size_t num = 0; num < _character.size();++num) {
				if (_character[num]->myStatus->PosX == x * CHIP_SIZE && _character[num]->myStatus->PosY == y * CHIP_SIZE) StageCreate::Instance()->CheckOnUnit(x, y, _character[num]->myStatus->myTeam);
			}
		}
	}
}

// アイテムの追加
void Character::AddItem(string itemName)
{
	if (itemCount == 5) return;

	Item.push_back(make_unique<Weapon>());
	Item[0]->ParamInitialize(itemName);
	itemCount++;
}

// ユニットのレベルアップ
void Character::LevelUp()
{
	// ファイルを開く
	fstream file;
	file.open(myStatus->myData, ios::binary | ios::out);
	file.write((char*)&myStatus->myParam, sizeof(myStatus->myParam));

	file.close();
}

// 終了処理
void Character::Finalize()
{

}
