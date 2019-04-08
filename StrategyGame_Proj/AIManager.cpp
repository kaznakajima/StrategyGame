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
	_myCharacter = nullptr;
	// リストの初期化
	_playerList.clear();
	_enemyList.clear();
}

// 更新
void AIManager::Update()
{
	if (_myCharacter != nullptr) {
		if (_myCharacter->myStatus->canAttack) CharacterManager::Instance()->DrawCheck(xPos, yPos);
	}
}

// 現在の敵(AI)、プレイヤーのカウント
void AIManager::CharacterCount(shared_ptr<Character> const &character)
{
	if (character->myStatus->myTeam == "Player") _playerList.push_back(character);
	else if (character->myStatus->myTeam == "Enemy") _enemyList.push_back(character);
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
	x = character->myStatus->PosX, y = character->myStatus->PosY;

	CharacterManager::Instance()->DrawCheck(x, y);

	character->MoveAreaClear(CharacterManager::Instance()->_character);

	character->MoveRange(character->myStatus->PosX, character->myStatus->PosY, character->myStatus->myParam.MOVERANGE);

	// プレイヤーユニットの周囲を検索
	for (size_t num = 0; num < _playerList.size(); ++num) {
		int moveX = _playerList[num]->myStatus->PosX, moveY = _playerList[num]->myStatus->PosY;
		// ユニットの周囲が移動可能な場合なら移動先に登録
		// 右
		if (moveY / CHIP_SIZE < 9 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE + character->myStatus->AttackRange][moveX / CHIP_SIZE] == true) {
			ChoiseMovePoint(moveX, moveY + CHIP_SIZE);
			if (isMove) {
				xPos = moveX, yPos = moveY;
				break;
			}
		}
		// ユニットの周囲が移動可能な場合なら移動先に登録
		// 左
		if (moveY / CHIP_SIZE > 0 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE - character->myStatus->AttackRange][moveX / CHIP_SIZE] == true) {
			ChoiseMovePoint(moveX, moveY - CHIP_SIZE);
			if (isMove) {
				xPos = moveX, yPos = moveY;
				break;
			}
		}
		// ユニットの周囲が移動可能な場合なら移動先に登録
		// 下
		if (moveX / CHIP_SIZE < 14 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE][moveX / CHIP_SIZE + character->myStatus->AttackRange] == true) {
			ChoiseMovePoint(moveX + CHIP_SIZE, moveY);
			if (isMove) {
				xPos = moveX, yPos = moveY;
				break;
			}
		}
		// ユニットの周囲が移動可能な場合なら移動先に登録
		// 上
		if (moveX / CHIP_SIZE > 0 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE][moveX / CHIP_SIZE - character->myStatus->AttackRange] == true) {
			ChoiseMovePoint(moveX - CHIP_SIZE, moveY);
			if (isMove) {
				xPos = moveX, yPos = moveY;
				break;
			}
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
	CharacterManager::Instance()->GetMoveArrow(x, y);
	CharacterManager::Instance()->KeyCheck(x, y);
}

// 移動先の選択
void AIManager::ChoiseMovePoint(int _x, int _y)
{
	if (StageCreate::Instance()->onUnit[_y / CHIP_SIZE][_x / CHIP_SIZE] != "NONE") {
		isMove = false; 
		return;
	}

	x = _x, y = _y;

	isMove = true;
}

// プレイヤー側のキャラクターの取得
int AIManager::GetDistancePlayer(shared_ptr<Character> const &character, vector<shared_ptr<Character>> const &playerList)
{
	// プレイヤー側のキャラクターとの距離
	int offsetX = 0, offsetY = 0, offsetTotal = 0;
	// 最短距離
	int _minDistance = 100;

	// プレイヤーからの距離の計算
	for (size_t num = 0; num < _playerList.size(); ++num) {
		offsetTotal = 0;
		offsetX = abs(character->myStatus->PosX - _playerList[num]->myStatus->PosX) / CHIP_SIZE;
		offsetY = abs(character->myStatus->PosY - _playerList[num]->myStatus->PosY) / CHIP_SIZE;
		offsetTotal = offsetX + offsetY;

		// 最短距離の更新
		if (_minDistance > offsetTotal) {
			_minDistance = offsetTotal;
		}
	}

	return _minDistance;
}

// 移動地点を検索
void AIManager::GetMovePoint(shared_ptr<Character> const &character, int _x, int _y, vector<shared_ptr<Character>> const &playerList)
{
	if (StageCreate::Instance()->checkMove[_y][_x] == false || StageCreate::Instance()->onUnit[_y][_x] != "NONE") return;

	// プレイヤー側のキャラクターとの距離
	int offsetX = 0, offsetY = 0, offsetTotal = 0;
	// プレイヤー側のキャラクターとの距離
	int _offsetX = 0, _offsetY = 0, _offsetTotal = 0;

	if (character->moveToPos[_y][_x] > 0 && moveCost >= character->moveToPos[_y][_x]) {
		moveCost = character->moveToPos[_y][_x];
		// プレイヤーからの距離の計算
		for (size_t num = 0; num < _playerList.size(); ++num) {
			offsetTotal = 0, _offsetTotal = 0;
			// 距離の検出
			offsetX = abs(_x * CHIP_SIZE - _playerList[num]->myStatus->PosX) / CHIP_SIZE;
			offsetY = abs(_y * CHIP_SIZE - _playerList[num]->myStatus->PosY) / CHIP_SIZE;
			offsetTotal = offsetX + offsetY;
			// 現在の最短距離の検出
			_offsetX = abs(x - _playerList[num]->myStatus->PosX) / CHIP_SIZE;
			_offsetY = abs(y - _playerList[num]->myStatus->PosY) / CHIP_SIZE;
			_offsetTotal = _offsetX + _offsetY;

			// 最短距離の更新
			if (offsetTotal <= _offsetTotal) {
				// 移動可能化チェック
				CheckCanMove(character, _playerList[num]->myStatus->PosX / CHIP_SIZE, _playerList[num]->myStatus->PosY / CHIP_SIZE, _playerList[num]);
				if (isMove == false) {
					x = moveToX;
					y = moveToY;
				}
				else {
					moveToX = _x * CHIP_SIZE;
					moveToY = _y * CHIP_SIZE;
				}
			}
		}
	}
}

// 移動可能な地点かチェックする
void AIManager::CheckCanMove(shared_ptr<Character> const &character, int _x, int _y, shared_ptr<Character> const &playerSt)
{
	// 移動不可能な地点ならリターン
	if (StageCreate::Instance()->stageList[_y][_x] > 0) return;

	// 最短距離の地点を格納しリターン
	if (character->moveToPos[_y][_x] == moveCost && StageCreate::Instance()->onUnit[_y][_x] == "NONE") {
		isMove = true;
		int disX = abs(_x * CHIP_SIZE - playerSt->myStatus->PosX) / CHIP_SIZE, disY = abs(_y * CHIP_SIZE - playerSt->myStatus->PosY) / CHIP_SIZE;
		int offset = disX + disY;
		if (minDistance > offset) {
			minDistance = offset;
			x = _x * CHIP_SIZE;
			y = _y * CHIP_SIZE;
		}
		return;
	}

	// 最短距離のユニットとの距離を測る
	int moveX = (character->myStatus->PosX - _x * CHIP_SIZE) / CHIP_SIZE, moveY = (character->myStatus->PosY - _y * CHIP_SIZE) / CHIP_SIZE;

	if (moveX > character->myStatus->PosX / CHIP_SIZE || moveY > character->myStatus->PosY / CHIP_SIZE) {
		// ユニットに向かってルートを逆探知していく
		if (_x + 1 < 14 && StageCreate::Instance()->stageList[_y][_x + 1] <= 0) {
			CheckCanMove(character, _x + 1, _y, playerSt);
		}
		if (_y + 1 < 9 && StageCreate::Instance()->stageList[_y + 1][_x] <= 0) {
			CheckCanMove(character, _x, _y + 1, playerSt);
		}
		return;
	}
	else if (moveX > character->myStatus->PosX / CHIP_SIZE || moveY < character->myStatus->PosY / CHIP_SIZE) {
		// ユニットに向かってルートを逆探知していく
		if (_x + 1 < 14 && StageCreate::Instance()->stageList[_y][_x + 1] <= 0) {
			CheckCanMove(character, _x + 1, _y, playerSt);
		}
		if (_y - 1 > 0 && StageCreate::Instance()->stageList[_y - 1][_x] <= 0) {
			CheckCanMove(character, _x, _y - 1, playerSt);
		}
		return;
	}
	else if (moveX <  character->myStatus->PosX / CHIP_SIZE || moveY >  character->myStatus->PosY / CHIP_SIZE) {
		// ユニットに向かってルートを逆探知していく
		if (_x - 1 > 0 && StageCreate::Instance()->stageList[_y][_x - 1] <= 0) {
			CheckCanMove(character, _x - 1, _y, playerSt);
		}
		if (_y + 1 < 9 && StageCreate::Instance()->stageList[_y + 1][_x] <= 0) {
			CheckCanMove(character, _x, _y + 1, playerSt);
		}
		return;
	}
	else if (moveX < character->myStatus->PosX / CHIP_SIZE || moveY < character->myStatus->PosY / CHIP_SIZE) {
		// ユニットに向かってルートを逆探知していく
		if (_x - 1 > 0 && StageCreate::Instance()->stageList[_y][_x - 1] <= 0) {
			CheckCanMove(character, _x - 1, _y, playerSt);
		}
		if (_y - 1 > 0 && StageCreate::Instance()->stageList[_y - 1][_x] <= 0) {
			CheckCanMove(character, _x, _y - 1, playerSt);
		}
		return;
	}
}

// 敵キャラクターのロスト(死亡処理)
void AIManager::CharacterLost(Character* character)
{

}

void AIManager::Finalize()
{
	if (_playerList.empty() == false) _playerList.clear();
	if (_enemyList.empty() == false) _enemyList.clear();
}