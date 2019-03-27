#include "AIManager.h"
#include "StageCreate.h"

AIManager::AIManager()
{
	characterMgr = CharacterManager::Instance();

	isMove = false;
}

// 初期化
void AIManager::Initialize()
{
	// リストの初期化
	playerList.clear();
	enemyList.clear();
}

// 更新
void AIManager::Update()
{
	DrawFormatString(0, 48, GetColor(0, 0, 0), "%d", playerList.size());
	if (myCharacter != nullptr) {
		if (characterMgr->isSelect) {
			//characterMgr->Draw();
		}
		if (myCharacter->myStatus->canAttack) characterMgr->DrawCheck(xPos, yPos);
	}
}

// 現在の敵(AI)、プレイヤーのカウント
void AIManager::CharacterCount(Character* character)
{
	if (character->myStatus->myTeam == "Player") {
		playerList.push_back(character);
	}
	else if (character->myStatus->myTeam == "Enemy") {
		enemyList.push_back(character);
	} 
}

// 初回起動
void AIManager::Play()
{
	isMove = false;
	myCharacter = nullptr;
	int _minDistance = 100;

	// プレイヤーに一番近いユニットを行動させる
	for (Character* character : enemyList) {
		// 一番近いキャラクターを選択
		if (_minDistance > GetDistancePlayer(character, playerList) && character->myStatus->canMove) {
			_minDistance = GetDistancePlayer(character, playerList);
			myCharacter = character;
		}
	}

	if (myCharacter != nullptr) MoveSelect(myCharacter);
}

// 動かすキャラクターの選択
void AIManager::MoveSelect(Character* character)
{
	x = character->myStatus->PosX, y = character->myStatus->PosY;

	characterMgr->DrawCheck(x, y);

	character->MoveAreaClear(characterMgr->character);

	character->MoveRange(character->myStatus->PosX, character->myStatus->PosY, character->myStatus->myParam.MOVERANGE);

	for (Character* _character : playerList) {
		int moveX  = _character->myStatus->PosX, moveY = _character->myStatus->PosY;
		if (moveY / CHIP_SIZE < 9 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE + 1][moveX / CHIP_SIZE] == true) {
			ChoiseMovePoint(moveX, moveY + CHIP_SIZE);
			if (isMove) {
				xPos = moveX, yPos = moveY;
				break;
			}
		}
		if (moveY / CHIP_SIZE > 0 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE - 1][moveX / CHIP_SIZE] == true) {
			ChoiseMovePoint(moveX, moveY - CHIP_SIZE);
			if (isMove) {
				xPos = moveX, yPos = moveY;
				break;
			}
		}
		if (moveX / CHIP_SIZE < 14 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE][moveX / CHIP_SIZE + 1] == true) {
			ChoiseMovePoint(moveX + CHIP_SIZE, moveY);
			if (isMove) {
				xPos = moveX, yPos = moveY;
				break;
			}
		}
		if (moveX / CHIP_SIZE > 0 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE][moveX / CHIP_SIZE - 1] == true) {
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
				GetMovePoint(character, stageX, stageY, playerList);
			}
		}
	}
	characterMgr->GetMoveArrow(x, y);
	characterMgr->KeyCheck(x, y);
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
int AIManager::GetDistancePlayer(Character* character, vector<Character*> playerList)
{
	// プレイヤー側のキャラクターとの距離
	int offsetX = 0, offsetY = 0, offsetTotal = 0;
	// 最短距離
	int _minDistance = 100;

	// プレイヤーからの距離の計算
	for (Character* playerSt : playerList) {
		offsetTotal = 0;
		offsetX = abs(character->myStatus->PosX - playerSt->myStatus->PosX) / CHIP_SIZE;
		offsetY = abs(character->myStatus->PosY - playerSt->myStatus->PosY) / CHIP_SIZE;
		offsetTotal = offsetX + offsetY;

		// 最短距離の更新
		if (_minDistance > offsetTotal) {
			_minDistance = offsetTotal;
		}
	}

	return _minDistance;
}

// 移動地点を検索
void AIManager::GetMovePoint(Character* character, int _x, int _y, vector<Character*> playerList)
{
	if (StageCreate::Instance()->checkMove[_y][_x] == false || StageCreate::Instance()->onUnit[_y][_x] != "NONE") return;

	// プレイヤー側のキャラクターとの距離
	int offsetX = 0, offsetY = 0, offsetTotal = 0;
	// プレイヤー側のキャラクターとの距離
	int _offsetX = 0, _offsetY = 0, _offsetTotal = 0;

	if (character->moveToPos[_y][_x] > 0 && moveCost >= character->moveToPos[_y][_x]) {
		moveCost = character->moveToPos[_y][_x];
		// プレイヤーからの距離の計算
		for (Character* playerSt : playerList) {
			offsetTotal = 0, _offsetTotal = 0;
			offsetX = abs(_x * CHIP_SIZE - playerSt->myStatus->PosX) / CHIP_SIZE;
			offsetY = abs(_y * CHIP_SIZE - playerSt->myStatus->PosY) / CHIP_SIZE;
			offsetTotal = offsetX + offsetY;
			_offsetX = abs(x - playerSt->myStatus->PosX) / CHIP_SIZE;
			_offsetY = abs(y - playerSt->myStatus->PosY) / CHIP_SIZE;
			_offsetTotal = _offsetX + _offsetY;

			// 最短距離の更新
			if (offsetTotal <= _offsetTotal) {
				CheckCanMove(character, playerSt->myStatus->PosX / CHIP_SIZE, playerSt->myStatus->PosY / CHIP_SIZE, playerSt);
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

void AIManager::CheckCanMove(Character* character, int _x, int _y, Character* playerSt)
{
	if (StageCreate::Instance()->stageList[_y][_x] > 0) return;

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

	int moveX = (character->myStatus->PosX - _x * CHIP_SIZE) / CHIP_SIZE, moveY = (character->myStatus->PosY - _y * CHIP_SIZE) / CHIP_SIZE;
	if (moveX > character->myStatus->PosX / CHIP_SIZE || moveY > character->myStatus->PosY / CHIP_SIZE) {
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