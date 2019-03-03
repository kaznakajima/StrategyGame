#include "AIManager.h"
#include "StageCreate.h"

AIManager::AIManager()
{
	characterMgr = new CharacterManager();
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
	if (myStatus != nullptr && myStatus->isSelect) characterMgr->Draw();

	characterMgr->Update(x, y);
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

// 初回起動
void AIManager::Play()
{
	int minDistance = 100;

	// プレイヤーに一番近いユニットを行動させる
	for (Character::STATUS &status : enemyList) {
		// 一番近いキャラクターを選択
		if (minDistance > GetDistance(&status, playerList) && status.canMove) {
			minDistance = GetDistance(&status, playerList);
			myStatus = &status;
		}
	}

	MoveSelect(myStatus);
}

// 動かすキャラクターの選択
void AIManager::MoveSelect(Character::STATUS* status)
{
	x = status->PosX, y = status->PosY;

	ChoiseMovePoint();
}

// 移動先の選択
void AIManager::ChoiseMovePoint()
{
	characterMgr->DrawCheck(x, y);
}

// プレイヤー側のキャラクターの取得
int AIManager::GetDistance(Character::STATUS* status, vector<Character::STATUS> playerList)
{
	// プレイヤー側のキャラクターとの距離
	int offsetX = 0, offsetY = 0, offsetTotal = 0;
	// 最短距離
	int minDistance = 100;

	// プレイヤーからの距離の計算
	for (Character::STATUS &playerSt : playerList) {
		offsetX = abs(playerSt.PosX - status->PosX);
		offsetY = abs(playerSt.PosY - status->PosY);
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