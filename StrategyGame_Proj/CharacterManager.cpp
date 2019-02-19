#include "CharacterManager.h"
#include "Character.h"

const int PlayerNum = 3;

// キャラクターの情報
vector<Character::STATUS> statusList;
Character::STATUS status[PlayerNum];

// キャラクターのインスタンス
Character* character;

// 初期化
void CharacterManager::Initialize()
{
	character = new Character();

	for (int num = 0; num < PlayerNum; num++) {
		statusList.push_back(status[num]);
	}

	character->Character_Initialize(&statusList[0], CHARACTER_DATA_1, 240, 240);
	character->Character_Initialize(&statusList[1], CHARACTER_DATA_2, 144, 336);
	character->Character_Initialize(&statusList[2], CHARACTER_DATA_3, 96, 384);
}

// アニメーション更新
void CharacterManager::Update()
{
	for (unsigned int i = 0; i < statusList.size(); i++) {
		// 死亡したユニットを除外する
		if (statusList[i].isDeath) {
			statusList.erase(statusList.begin() + i);
			return;
		}

		character->CharacterAnim(&statusList[i]);
	}
}

// 描画するかチェック
void CharacterManager::DrawCheck(int x, int y) 
{
	// 攻撃中はリターン
	if (attack == true) {
		Attack(x, y);
		return;
	}

	for (unsigned int i = 0; i < statusList.size(); i++)
	{
		// カーソルが合っているユニットのみ表示
		if (statusList[i].PosX == x && statusList[i].PosY == y) {
			statusList[i].isSelect = true;
			statusList[i].AnimHandle = 4.0f;
			isSelect = true;
			attack = false;
		}
	}
}

// 描画処理
void CharacterManager::Draw()
{
	for (unsigned int i = 0; i < statusList.size(); i++) {

		character->MoveAreaClear();

		if (statusList[i].isSelect) {
			character->OldPosX.push_back(statusList[i].PosX);
			character->OldPosY.push_back(statusList[i].PosY);
			character->MoveRange(&statusList[i], statusList[i].PosX, statusList[i].PosY, statusList[i].myParam.MOVERANGE);
			character->AttackRange();
			return;
		}
	}
}

void CharacterManager::CharacterMove(int x, int y) 
{
	for (unsigned int i = 0; i < statusList.size(); i++) {
		if (statusList[i].isSelect) {
			statusList[i].isMove = true;
			isMove = character->CharacterMove(&statusList[i], x, y);
		}
	}
}

// 移動値の取得
void CharacterManager::GetMoveCount(int x, int y) {
	character->OldPosX.push_back(x);
	character->OldPosY.push_back(y);
	character->moveCount++;
}

void CharacterManager::GetMoveArrow(int x, int y)
{
	for (unsigned int i = 0; i < statusList.size(); i++) {
		if (statusList[i].isSelect) {
			character->DrawMoveArrow(statusList[i], x, y, 5);

			if (x == statusList[i].PosX && y == statusList[i].PosY) {
				character->OldPosX.clear();
				character->OldPosY.clear();
				character->moveCount = 0;
			}
		}
	}
}

// 攻撃範囲表示
void CharacterManager::GetAttackArea(int x, int y)
{
	// 攻撃するユニット
	Character::STATUS* myStatus = nullptr;

	for (unsigned int num = 0; num < statusList.size(); num++) {
		if (statusList[num].canAttack) {
			myStatus = &statusList[num];
			character->AttackableDraw(&statusList[num]);
			attack = true;
		}
	}

	for (unsigned int num = 0; num < statusList.size(); num++) {
		if (myStatus != nullptr && myStatus != &statusList[num]) {
			if (statusList[num].PosX == x && statusList[num].PosY == y)
				character->GetAttackDetail(myStatus, &statusList[num]);
		}
	}
}

// 攻撃
void CharacterManager::Attack(int x, int y)
{
	// 攻撃するユニット
	Character::STATUS* myStatus = nullptr;

	for (unsigned int num = 0; num < statusList.size(); num++) {
		if(statusList[num].canAttack)
			myStatus = &statusList[num];
	}

	for (unsigned int num = 0; num < statusList.size(); num++) {
		if (myStatus != &statusList[num]) {
			if (statusList[num].PosX == x && statusList[num].PosY == y)
				character->CharacterAttack(myStatus, &statusList[num], 1);
		}
	}

	if(myStatus != nullptr) myStatus->canAttack = false;
	attack = false;
}

// カメラとのオフセットの計算
void CharacterManager::SetCameraOffset(int dir, bool horizontal)
{
	for (unsigned int num = 0; num < statusList.size(); num++) {
		character->SetCameraOffset(&statusList[num], dir, horizontal);
	}
}

void CharacterManager::KeyCheck(int x, int y)
{
	for (unsigned int i = 0; i < statusList.size(); i++) {
		// カーソルが合っているユニットのみ表示
		if (statusList[i].PosX != x || statusList[i].PosY != y) {
			character->OldPosX.clear();
			character->OldPosY.clear();
			character->moveCount = 0;
			isSelect = false;
		}
	}
}

void CharacterManager::Finalize() {

}