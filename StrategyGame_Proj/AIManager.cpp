#include "AIManager.h"
#include "StageCreate.h"

AIManager::AIManager()
{
	isMove = false;
}

AIManager::~AIManager() { }

// 初期化
void AIManager::Initialize()
{
	if(_myCharacter != nullptr) _myCharacter.reset();
	// リストの初期化
	_playerList.clear(); _playerList = CharacterManager::Instance()->_playerList;
	_enemyList.clear(); _enemyList = CharacterManager::Instance()->_enemyList;
}

// 更新
void AIManager::Update()
{
	if (_myCharacter != nullptr) {
		if (_myCharacter->myStatus->canAttack) CharacterManager::Instance()->DrawCheck(xPos, yPos);
	}
}

// 初回起動
void AIManager::Play()
{
	isMove = false;
	int _minDistance = 100;

	// プレイヤーに一番近いユニットを行動させる
	for (size_t num = 0; num < _enemyList.size(); ++num) {
		// 一番近いキャラクターを選択
		if (_minDistance > GetDistancePlayer(_enemyList[num], _playerList) && _enemyList[num]->myStatus->canMove) {
			_minDistance = GetDistancePlayer(_enemyList[num], _playerList);
			_myCharacter = _enemyList[num];
		}
	}

	// 移動先の選択
	if (_myCharacter != nullptr) MoveSelect(_myCharacter);
}

// 移動先の選択
void AIManager::MoveSelect(shared_ptr<Character> const &character)
{
	// 敵の移動先の候補を検出
	x = character->myStatus->xPos, y = character->myStatus->yPos;
	CharacterManager::Instance()->DrawCheck(x, y);
	character->MoveAreaClear(CharacterManager::Instance()->_character);
	character->MoveRange(character->myStatus->xPos, character->myStatus->yPos, character->myStatus->myParam.MOVERANGE);

	// プレイヤーユニットの周囲を検索
	for (size_t num = 0; num < _playerList.size(); ++num) {
		int moveX = _playerList[num]->myStatus->xPos, moveY = _playerList[num]->myStatus->yPos;
		// ユニットの周囲が移動可能な場合なら移動先に登録
		// 下
		if (moveY / CHIP_SIZE < 9 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE + character->myStatus->AttackRange][moveX / CHIP_SIZE] == true) {
			isMove = ChoiseMovePoint(moveX, moveY + CHIP_SIZE);
			if (isMove) { xPos = moveX, yPos = moveY; break;} // 攻撃先の座標を取得
			else if(moveX == x && moveY + CHIP_SIZE == y) { xPos = moveX, yPos = moveY; break; } // その場で攻撃できるなら攻撃先を取得
		}
		// ユニットの周囲が移動可能な場合なら移動先に登録
		// 上
		if (moveY / CHIP_SIZE > 0 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE - character->myStatus->AttackRange][moveX / CHIP_SIZE] == true) {
			isMove = ChoiseMovePoint(moveX, moveY - CHIP_SIZE);
			if (isMove) { xPos = moveX, yPos = moveY; break; } // 攻撃先の座標を取得
			else if (moveX == x && moveY - CHIP_SIZE == y) { xPos = moveX, yPos = moveY; break; } // その場で攻撃できるなら攻撃先を取得
		}
		// ユニットの周囲が移動可能な場合なら移動先に登録
		// 右
		if (moveX / CHIP_SIZE < 14 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE][moveX / CHIP_SIZE + character->myStatus->AttackRange] == true) {
			isMove = ChoiseMovePoint(moveX + CHIP_SIZE, moveY);
			if (isMove) { xPos = moveX, yPos = moveY; break; } // 攻撃先の座標を取得
			else if (moveX + CHIP_SIZE == x && moveY == y) { xPos = moveX, yPos = moveY; break; } // その場で攻撃できるなら攻撃先を取得
		}
		// ユニットの周囲が移動可能な場合なら移動先に登録
		// 左
		if (moveX / CHIP_SIZE > 0 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE][moveX / CHIP_SIZE - character->myStatus->AttackRange] == true) {
			isMove = ChoiseMovePoint(moveX - CHIP_SIZE, moveY);
			if (isMove) { xPos = moveX, yPos = moveY; break; } // 攻撃先の座標を取得
			else if (moveX - CHIP_SIZE == x && moveY == y) { xPos = moveX, yPos = moveY; break; } // その場で攻撃できるなら攻撃先を取得
		}
	}

	// まだ移動先が決まっていないなら
	if (isMove == false) {
		minDistance = 100;
		moveCost = character->myStatus->myParam.MOVERANGE;
		// プレイヤー方向に向かって移動
		for (int stageY = 0; stageY < StageCreate::Instance()->MAP_SIZEY; stageY++) {
			for (int stageX = 0; stageX < StageCreate::Instance()->MAP_SIZEX; stageX++) {
				GetMovePoint(character, stageX, stageY, _playerList);
			}
		}
	}

	// 実際に移動させる
	CharacterManager::Instance()->GetMoveArrow(x, y);
	CharacterManager::Instance()->KeyCheck(x, y);
}

// 移動先の選択
bool AIManager::ChoiseMovePoint(int _x, int _y)
{
	// 他のユニットがその場にいるならリターン
	if (StageCreate::Instance()->onUnit[_y / CHIP_SIZE][_x / CHIP_SIZE] != "NONE") return false;

	// 移動先の代入
	x = _x, y = _y;

	return true;
}

// プレイヤー側のユニットを取得
int AIManager::GetDistancePlayer(shared_ptr<Character> const &character, vector<shared_ptr<Character>> const &playerList)
{
	// プレイヤー側のキャラクターとの距離
	int offsetX = 0, offsetY = 0, offsetTotal = 0;
	// 最短距離
	int _minDistance = 100;

	// プレイヤーからの距離の計算
	for (size_t num = 0; num < _playerList.size(); ++num) {
		offsetTotal = 0;
		offsetX = abs(character->myStatus->xPos - _playerList[num]->myStatus->xPos) / CHIP_SIZE;
		offsetY = abs(character->myStatus->yPos - _playerList[num]->myStatus->yPos) / CHIP_SIZE;
		offsetTotal = offsetX + offsetY;

		// 最短距離の更新
		if (_minDistance > offsetTotal) {
			_minDistance = offsetTotal;
		}
	}

	// 最短距離を返す
	return _minDistance;
}

// 移動地点を検索
void AIManager::GetMovePoint(shared_ptr<Character> const &character, int _x, int _y, vector<shared_ptr<Character>> const &playerList)
{
	// 自身の移動範囲外、他のユニットがいる位置ならリターン
	if (StageCreate::Instance()->checkMove[_y][_x] == false || StageCreate::Instance()->onUnit[_y][_x] != "NONE") return;

	// プレイヤー側のキャラクターとの距離
	int offsetX = 0, offsetY = 0, offsetTotal = 0;
	// プレイヤー側のキャラクターとの距離
	int _offsetX = 0, _offsetY = 0, _offsetTotal = 0;

	// 移動先候補までの最も遠い位置なら実行
	if (character->moveToPos[_y][_x] > 0 && moveCost >= character->moveToPos[_y][_x]) {
		moveCost = character->moveToPos[_y][_x];
		// プレイヤーからの距離の計算
		for (size_t num = 0; num < _playerList.size(); ++num) {
			offsetTotal = 0, _offsetTotal = 0;
			// 距離の検出
			offsetX = abs(_x * CHIP_SIZE - _playerList[num]->myStatus->xPos) / CHIP_SIZE;
			offsetY = abs(_y * CHIP_SIZE - _playerList[num]->myStatus->yPos) / CHIP_SIZE;
			offsetTotal = offsetX + offsetY;
			// 現在の最短距離の検出
			_offsetX = abs(x - _playerList[num]->myStatus->xPos) / CHIP_SIZE;
			_offsetY = abs(y - _playerList[num]->myStatus->yPos) / CHIP_SIZE;
			_offsetTotal = _offsetX + _offsetY;

			// 最短距離の更新
			if (offsetTotal < _offsetTotal) {
				CheckCanMove(character, _x, _y, _playerList[num]);
			}
		}
	}
}

// 移動可能な地点かチェックする
void AIManager::CheckCanMove(shared_ptr<Character> const &character, int _x, int _y, shared_ptr<Character> const &playerSt)
{
	// 移動不可能な地点ならリターン
	if (StageCreate::Instance()->stageList[_y][_x] > 0) return;

	// 移動先を格納
	isMove = true;
	x = _x * CHIP_SIZE;
	y = _y * CHIP_SIZE;
}

// 終了処理
void AIManager::Finalize()
{
	if (_playerList.empty() == false) _playerList.clear();
	if (_enemyList.empty() == false) _enemyList.clear();
	if (_myCharacter != nullptr) _myCharacter.reset();
}