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
	if (myCharacter != nullptr && myCharacter->myStatus->isSelect) characterMgr->Draw();

	characterMgr->Update(x, y);
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

	MoveSelect(myCharacter);
}

// 動かすキャラクターの選択
void AIManager::MoveSelect(Character* character)
{
	x = character->myStatus->PosX, y = character->myStatus->PosY;

	ChoiseMovePoint();
}

// 移動先の選択
void AIManager::ChoiseMovePoint()
{
	characterMgr->DrawCheck(x, y);
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
		offsetX = abs(playerSt->myStatus->PosX - character->myStatus->PosX);
		offsetY = abs(playerSt->myStatus->PosY - character->myStatus->PosY);
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