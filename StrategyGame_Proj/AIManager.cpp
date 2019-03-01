#include "AIManager.h"

// 初期化
void AIManager::Initialize()
{
	// リストの初期化
	enemyList.clear();
}

// 現在の敵(AI)、プレイヤーのカウント
void AIManager::CharacterCount(Character::STATUS status)
{
	if (status.myTeam == "Player") {
		playerList.push_back(status);
	}
	else if (status.myTeam == "Enemy") {
		enemyList.push_back(status);
	} 
}

// 動かすキャラクターの選択
void AIManager::MoveSelect(Character::STATUS* status)
{
	status->isSelect = true;

	SerchCharacter(status, playerList);
}

// 移動先の選択
void AIManager::ChoiseMovePoint()
{
	// 移動先の座標
	int x = 0, y = 0;
}

// プレイヤー側のキャラクターの取得
int AIManager::SerchCharacter(Character::STATUS* status, vector<Character::STATUS> playerList)
{
	// プレイヤー側のキャラクターとの距離
	int offsetX = 0, offsetY = 0, offsetTotal = 0;
	// 最短距離
	int minDistance = 100;

	// プレイヤーからの距離の計算
	for (unsigned int num = 0; num < playerList.size(); num++) {
		offsetX = playerList[num].PosX - status->PosX;
		offsetY = playerList[num].PosY - status->PosY;
		offsetTotal = offsetX + offsetY;

		// 最短距離の更新
		if (minDistance > offsetTotal) {
			minDistance = offsetTotal;
		}
	}

	return minDistance;
}

// 敵キャラクターのロスト(死亡処理)
void AIManager::CharacterLost(Character::STATUS* status)
{

}