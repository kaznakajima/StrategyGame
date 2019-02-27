#include "CharacterManager.h"
#include "Character.h"

const int PlayerNum = 3;

// キャラクターの情報
vector<Character::STATUS> statusList;
Character::STATUS status[PlayerNum];

// キャラクターのインスタンス
Character* character;
// 攻撃するユニット
Character::STATUS* myStatus = nullptr;
// 攻撃されるユニット
Character::STATUS* eStatus = nullptr;

// 初期化
void CharacterManager::Initialize()
{
	character = new Character();

	for (int num = 0; num < PlayerNum; num++) {
		statusList.push_back(status[num]);
	}

	character->Character_Initialize(&statusList[0], CHARACTER_DATA_1, "Player", 240, 240);
	character->Character_Initialize(&statusList[1], CHARACTER_DATA_2, "Enemy", 144, 336);
	character->Character_Initialize(&statusList[2], CHARACTER_DATA_3, "Player", 96, 384);

	StartTurn();
}

// キャラクター更新
void CharacterManager::Update(int x, int y)
{
	// 攻撃範囲検索
	GetAttackArea(x, y);

	for (unsigned int i = 0; i < statusList.size(); i++) {
		// 死亡したユニットを除外する
		if (statusList[i].isDeath) {
			statusList.erase(statusList.begin() + i);
			return;
		}

		character->CharacterAnim(&statusList[i]);
	}

	if (isSelect == false) CharacterMove(x, y);

	if (attack) Attack();
}

// ターン開始
void CharacterManager::StartTurn()
{
	playerTurn = !playerTurn;
	moveableUnit = 0;

	// 移動可能なユニットのカウント
	for (int num = 0; num < statusList.size(); num++) {
		// プレイヤーターン
		if (playerTurn && statusList[num].myTeam == "Player") moveableUnit++;
		// 敵ターン
		else if (playerTurn == false && statusList[num].myTeam == "Enemy") moveableUnit++;
	}
}

// 描画するかチェック
void CharacterManager::DrawCheck(int x, int y) 
{
	// 攻撃中はリターン
	if (attack == true) {
		ChoiseAttack(x, y);
		return;
	}

	for (unsigned int i = 0; i < statusList.size(); i++)
	{
		// カーソルが合っているユニットのみ表示
		if (statusList[i].PosX == x && statusList[i].PosY == y) {
			if (statusList[i].myTeam == "Player" && statusList[i].canMove) {
				statusList[i].isSelect = true;
				statusList[i].AnimHandle = 4.0f;
				isSelect = true;
				attack = false;
			}
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
			isMove = character->CharacterMove(&statusList[i], x, y);
			if (isMove == false && statusList[i].canMove == false && statusList[i].canAttack == false) moveableUnit--;
		}
	}

	if (moveableUnit <= 0) StartTurn();
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

// 攻撃の対象を選択
void CharacterManager::ChoiseAttack(int x, int y)
{
	// 現在攻撃可能なユニットのリファレンス
	for (unsigned int num = 0; num < statusList.size(); num++) {
		if(statusList[num].canAttack) myStatus = &statusList[num];
	}

	// 選択した位置に敵がいたら攻撃対象のリファレンスを作成
	for (unsigned int num = 0; num < statusList.size(); num++) {
		if (myStatus != &statusList[num]) {
			if (statusList[num].PosX == x && statusList[num].PosY == y) {
				eStatus = &statusList[num];
				myStatus->isAttack = true;
			}
		}
	}

	// 攻撃可能なユニットがいなかったら終了
	if(myStatus != nullptr) myStatus->canAttack = false;
	// 選択した位置に敵がいないなら終了
	if (eStatus == nullptr) {
		attack = false;
		moveableUnit--;
		if (moveableUnit == 0) StartTurn();
	}
}

// 攻撃のアニメーション
void CharacterManager::Attack()
{
	// 1回目の攻撃
	if (myStatus != nullptr && eStatus != nullptr && attackCount < 2) {
		if (myStatus->isAttack) attack = character->AttackAnimation(myStatus, eStatus, 1);
		if (eStatus->isAttack) attack = character->AttackAnimation(eStatus, myStatus, 2);

		// アニメーションが終わっていないならリターン
		if (attack) return;
	}

	// アニメーションが終わっているなら攻撃の回数を記録
	if(attack == false) attackCount = 3;

	// 2回目の攻撃
	if (myStatus != nullptr && eStatus != nullptr && attackCount > 2) {
		if (myStatus->isAttack) attack = character->AttackAnimation(myStatus, eStatus, 3);
		if (eStatus->isAttack) attack = character->AttackAnimation(eStatus, myStatus, 4);
		
		// アニメーションが終わっていないならリターン
		if (attack) return;
	}

	// アニメーションが終わったなら攻撃終了
	if (attack == false) {
		attackCount = 0;
		myStatus = nullptr;
		eStatus = nullptr;
		moveableUnit--;
		if (moveableUnit <= 0) StartTurn();
	}
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