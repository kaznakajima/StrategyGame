#include "AIManager.h"
#include "StageCreate.h"

AIManager::AIManager()
{
	characterMgr = CharacterManager::Instance();
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
	if (myCharacter != nullptr) {
		if (characterMgr->isSelect) {
			characterMgr->Draw();
			characterMgr->GetMoveArrow(x, y);
			characterMgr->KeyCheck(x, y);
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
	int minDistance = 100;

	// プレイヤーに一番近いユニットを行動させる
	for (Character* character : enemyList) {
		// 一番近いキャラクターを選択
		if (minDistance > GetDistance(character, playerList) && character->myStatus->canMove) {
			minDistance = GetDistance(character, playerList);
			myCharacter = character;
		}
	}

	if(myCharacter != nullptr) MoveSelect(myCharacter);
}

// 動かすキャラクターの選択
void AIManager::MoveSelect(Character* character)
{
	x = character->myStatus->PosX, y = character->myStatus->PosY;

	characterMgr->DrawCheck(x, y);

	for (int stageY = 0; stageY < StageCreate::Instance()->MAP_SIZEY; stageY++) {
		for (int stageX = 0; stageX < StageCreate::Instance()->MAP_SIZEX; stageX++) {
			if (StageCreate::Instance()->checkMove[stageY][stageX] == true) {
				x = stageX * CHIP_SIZE;
				y = stageY * CHIP_SIZE;
			}
		}
	}

	for (Character* _character : playerList) {
		xPos = _character->myStatus->PosX, yPos = _character->myStatus->PosY;
		if (yPos / CHIP_SIZE > 9 && StageCreate::Instance()->checkMove[yPos / CHIP_SIZE + 1][xPos / CHIP_SIZE] == true) ChoiseMovePoint(xPos, yPos + CHIP_SIZE);
		if (yPos / CHIP_SIZE < 0 && StageCreate::Instance()->checkMove[yPos / CHIP_SIZE - 1][xPos / CHIP_SIZE] == true) ChoiseMovePoint(xPos, yPos - CHIP_SIZE);
		if (xPos / CHIP_SIZE < 14 && StageCreate::Instance()->checkMove[yPos / CHIP_SIZE][xPos / CHIP_SIZE + 1] == true) ChoiseMovePoint(xPos + CHIP_SIZE, yPos);
		if (xPos / CHIP_SIZE > 0 && StageCreate::Instance()->checkMove[yPos / CHIP_SIZE][xPos / CHIP_SIZE - 1] == true) ChoiseMovePoint(xPos - CHIP_SIZE, yPos);
	}
}

// 移動先の選択
void AIManager::ChoiseMovePoint(int xPos, int yPos)
{
	if (StageCreate::Instance()->onUnit[yPos / CHIP_SIZE][xPos / CHIP_SIZE] != "NONE") return;

	x = xPos, y = yPos;
}

// プレイヤー側のキャラクターの取得
int AIManager::GetDistance(Character* character, vector<Character*> playerList)
{
	// プレイヤー側のキャラクターとの距離
	int offsetX = 0, offsetY = 0, offsetTotal = 0;
	// 最短距離
	int minDistance = 100;

	// プレイヤーからの距離の計算
	for (Character* playerSt : playerList) {
		offsetTotal = 0;
		offsetX = abs(character->myStatus->PosX - playerSt->myStatus->PosX) / CHIP_SIZE;
		offsetY = abs(character->myStatus->PosY - playerSt->myStatus->PosY) / CHIP_SIZE;
		offsetTotal = offsetX + offsetY;

		// 最短距離の更新
		if (minDistance > offsetTotal) {
			minDistance = offsetTotal;
		}
	}

	return minDistance;
}

// 敵キャラクターのロスト(死亡処理)
void AIManager::CharacterLost(Character* character)
{

}