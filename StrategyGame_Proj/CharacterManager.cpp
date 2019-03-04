#include "CharacterManager.h"
#include "AIManager.h"
#include "Character.h"

// 生成するキャラクター数
const int PlayerNum = 3;

// AIManagerインスタンス
AIManager* AIMgr;

// キャラクターの情報
vector<Character::STATUS> statusList;
Character::STATUS status[PlayerNum];

// キャラクターのインスタンス
Character* character;
// 攻撃するユニット
Character::STATUS* myStatus = nullptr;
// 攻撃されるユニット
Character::STATUS* eStatus = nullptr;

// デストラクタ
CharacterManager::~CharacterManager()
{
	delete AIMgr;
	delete character;
}

// 初期化
void CharacterManager::Initialize()
{
	AIMgr = new AIManager();
	character = new Character();

	// キャラクターの追加
	for (unsigned int num = 0; num < PlayerNum; num ++) {
		statusList.push_back(status[num]);
	}

	// キャラクターの初期化
	character->Character_Initialize(&statusList[0], CHARACTER_DATA_1, "Player", 240, 240);
	character->Character_Initialize(&statusList[1], CHARACTER_DATA_2, "Enemy", 144, 336);
	character->Character_Initialize(&statusList[2], CHARACTER_DATA_3, "Player", 96, 384);

	// 敵AIの初期化
	for (Character::STATUS &status : statusList) {
		AIMgr->CharacterCount(status);
	}

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
	for (Character::STATUS &status : statusList) {
		// プレイヤーターン
		if (playerTurn && status.myTeam == "Player") moveableUnit++;
		// 敵ターン
		else if (playerTurn == false && status.myTeam == "Enemy") moveableUnit++;
	}

	// 敵ターンの開始
	if (playerTurn == false) AIMgr->Play();
}

// 描画するかチェック
void CharacterManager::DrawCheck(int x, int y) 
{
	// 攻撃中はリターン
	if (attack == true) {
		ChoiseAttack(x, y);
		return;
	}

	for (Character::STATUS &status : statusList) {
		// カーソルが合っているユニットのみ表示
		if (status.PosX == x && status.PosY == y) {
			if (status.canMove) {
				status.isSelect = true;
				status.AnimHandle = 4.0f;
				isSelect = true;
				attack = false;
			}
		}
	}
}

// 描画処理
void CharacterManager::Draw()
{
	for (Character::STATUS &status : statusList) {
		character->MoveAreaClear();

		// 移動順路を記録しつつ移動範囲と攻撃範囲の描画
		if (status.isSelect) {
			character->OldPosX.push_back(status.PosX);
			character->OldPosY.push_back(status.PosY);
			character->MoveRange(&status, status.PosX, status.PosY, status.myParam.MOVERANGE);
			character->AttackRange();
			return;
		}
	}
}

void CharacterManager::CharacterMove(int x, int y) 
{
	// キャラクターの移動
	for (Character::STATUS &status : statusList) {
		if (status.isSelect) {
			isMove = character->CharacterMove(&status, x, y);

			// 移動が完了し、攻撃しないなら行動終了
			if (isMove == false && status.canMove == false && status.canAttack == false) moveableUnit--;
		}
	}

	// 行動可能なユニットがいないならターン終了
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
	// 移動順路を描画
	for (Character::STATUS &status : statusList) {
		if (status.isSelect) {
			character->DrawMoveArrow(status, x, y, 5);

			// ユニットの位置に戻ったら順路をクリア
			if (status.PosX == x && status.PosY == y) {
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
	// キャラクター情報観賞用
	Character::STATUS _status;

	// 攻撃可能なユニットの取得
	for (Character::STATUS &status : statusList) {
		if (status.canAttack) {
			myStatus = &status;
			_status = status;
			character->AttackableDraw(status);
			attack = true;
		}
	}

	// 攻撃可能なユニットがいるなら攻撃した際の詳細情報を描画
	for (Character::STATUS &status : statusList) {
		if (myStatus != nullptr && myStatus != &status) {
			if (status.PosX == x && status.PosY == y && status.myTeam != myStatus->myTeam) 
				character->GetAttackDetail(_status, status);
		}
	}
}

// 攻撃の対象を選択
void CharacterManager::ChoiseAttack(int x, int y)
{
	// 選択した位置に敵がいたら攻撃対象のリファレンスを作成
	for (Character::STATUS &status : statusList) {
		if (myStatus != &status) {
			if (status.PosX == x && status.PosY == y) {
				eStatus = &status;
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
	for (Character::STATUS &status : statusList) {
		character->SetCameraOffset(&status, dir, horizontal);
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