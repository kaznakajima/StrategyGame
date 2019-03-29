#include "CharacterManager.h"
#include "AIManager.h"

// 生成するキャラクター数
const int PlayerNum = 4;

AIManager* AIMgr;

// 初期化
void CharacterManager::Initialize()
{
	HpBar = LoadGraph(HP_BAR);
	HpBarBox = LoadGraph(HP_BARBOX);
	DamageDetail = LoadGraph(DAMAGE_DETAIL);

	AIMgr = AIManager::Instance();

	// キャラクターの追加
	for (size_t num = 0; num < PlayerNum; num++) {
		character.push_back(new Character());
	}

	// キャラクターの初期化
	character[0]->Character_Initialize(CHARACTER_DATA_1, "Player", 240, 240);
	character[1]->Character_Initialize(CHARACTER_DATA_3, "Enemy", 144, 240);
	character[2]->Character_Initialize(CHARACTER_DATA_2, "Player", 96, 384);
	character[3]->Character_Initialize(CHARACTER_DATA_4, "Enemy", 336, 96);

	// 敵AIの初期化
	for (Character* _character : character) {
		AIMgr->CharacterCount(_character);
	}

	StartTurn();
}

// キャラクター更新
void CharacterManager::Update(int x, int y)
{
	// 攻撃範囲検索
	GetAttackArea(x, y);

	if (myCharacter != nullptr) {
		DrawFormatString(0, 96, GetColor(0, 0, 0), myCharacter->myStatus->myParam.NAME.c_str());
	}

	for (size_t i = 0; i < character.size(); i++) {
		// アニメーション
		character[i]->CharacterAnim();
	}

	// キャラクター移動
	if (isSelect == false) CharacterMove(x, y);

	// 攻撃
	if (attack) Attack();

	// 攻撃中のデータ表示
	if (myCharacter != nullptr && eCharacter != nullptr) {
		if (TimeCount::Instance()->GetTimer(500.0f) >= 500.0f) {
			myCharacter = nullptr;
			eCharacter = nullptr;
			if (moveableUnit != 0 && playerTurn == false) AIMgr->Play();
			if (moveableUnit <= 0) StartTurn();
			return;
		}
		DrawAttackParam(myCharacter, eCharacter);
	}
}

// ターン開始
void CharacterManager::StartTurn()
{
	playerTurn = !playerTurn;
	moveableUnit = 0;

	for (size_t num = 0; num < character.size(); num++) {
		character[num]->TurnStart();

		// 死亡したユニットを除外する
		if (character[num]->myStatus->isDeath) {
			character.erase(character.begin() + num);
			AIMgr->Initialize();
			// 敵AIの初期化
			for (Character* _character : character) {
				AIMgr->CharacterCount(_character);
			}
		}
		else {
			// 移動可能なユニットのカウント
		    // プレイヤー側ユニットの計算
			if (playerTurn && character[num]->myStatus->myTeam == "Player") moveableUnit++;
			// 敵側ユニットの計算
			else if (playerTurn == false && character[num]->myStatus->myTeam == "Enemy") moveableUnit++;
		}
	}

	turnAnim = true;
}

// 描画するかチェック
void CharacterManager::DrawCheck(int x, int y) 
{
	// 攻撃中はリターン
	if (attack == true) {
		ChoiseAttack(x, y);
		return;
	}

	for (size_t num = 0; num < character.size(); num++) {
		if (playerTurn && character[num]->myStatus->myTeam == "Player") {
			// カーソルが合っているユニットのみ表示
			if (character[num]->myStatus->PosX == x && character[num]->myStatus->PosY == y) {
				if (character[num]->myStatus->canMove) {
					character[num]->myStatus->isSelect = true;
					character[num]->myStatus->AnimHandle = 4.0f;
					isSelect = true;
					attack = false;
				}
			}
		}
		else if (playerTurn == false && character[num]->myStatus->myTeam == "Enemy") {
			// カーソルが合っているユニットのみ表示
			if (character[num]->myStatus->PosX == x && character[num]->myStatus->PosY == y) {
				if (character[num]->myStatus->canMove) {
					character[num]->myStatus->isSelect = true;
					character[num]->myStatus->AnimHandle = 4.0f;
					isSelect = true;
					attack = false;
				}
			}
		}
		
	}
}

// 描画処理
void CharacterManager::Draw()
{
	for (size_t num = 0; num < character.size(); num++) {
		// 移動順路を記録しつつ移動範囲と攻撃範囲の描画
		if (character[num]->myStatus->isSelect) {
			character[num]->MoveAreaClear(character);
			character[num]->OldPosX.push_back(character[num]->myStatus->PosX);
			character[num]->OldPosY.push_back(character[num]->myStatus->PosY);
			character[num]->MoveRange(character[num]->myStatus->PosX, character[num]->myStatus->PosY, character[num]->myStatus->myParam.MOVERANGE);
			character[num]->AttackRange();
			return;
		}
	}
}

void CharacterManager::CharacterMove(int x, int y) 
{
	bool moveEnd = false;

	// キャラクターの移動
	for (size_t num = 0; num < character.size(); num++) {
		if (character[num]->myStatus->isSelect) {
			isMove = character[num]->CharacterMove(x, y);

			// 移動が完了し、攻撃しないなら行動終了
			if (isMove == false && character[num]->myStatus->canMove == false
				&& character[num]->myStatus->canAttack == false) {
				moveableUnit--;
				moveEnd = true;
			}
		}
	}

	// 行動可能なユニットがいないならターン終了
	if (moveEnd && playerTurn == false && moveableUnit != 0) AIMgr->Play();
	if (moveableUnit <= 0) StartTurn();
}

// 移動値の取得
void CharacterManager::GetMoveCount(int x, int y) {
	for (size_t num = 0; num < character.size(); num++) {
		character[num]->OldPosX.push_back(x);
		character[num]->OldPosY.push_back(y);
		character[num]->moveCount++;
	}
}

void CharacterManager::GetMoveArrow(int x, int y)
{
	// 移動順路を描画
	for (size_t num = 0; num < character.size(); num++) {
		if (character[num]->myStatus->isSelect) {
			character[num]->DrawMoveArrow(x, y, 5);

			// ユニットの位置に戻ったら順路をクリア
			if (character[num]->myStatus->PosX == x && character[num]->myStatus->PosY == y) {
				character[num]->OldPosX.clear();
				character[num]->OldPosY.clear();
				character[num]->moveCount = 0;
			}
		}
	}
}

// 攻撃範囲表示
void CharacterManager::GetAttackArea(int x, int y)
{
	// 攻撃可能なユニットの取得
	for (size_t num = 0; num < character.size(); num++) {
		if (character[num]->myStatus->canAttack) {
			myCharacter = character[num];
			myCharacter->AttackableDraw();
			attack = true;
		}
	}

	// 攻撃可能な位置のユニットとの攻撃した際の詳細情報の表示
	for (size_t num = 0; num < character.size(); num++) {
		if (myCharacter != nullptr && myCharacter->myStatus->canAttack) {
			if (character[num]->myStatus->PosX == x && character[num]->myStatus->PosY == y
				&& StageCreate::Instance()->onUnit[y / CHIP_SIZE][x / CHIP_SIZE] == "Enemy") {
				if (myCharacter->myStatus->myTeam != character[num]->myStatus->myTeam) myCharacter->GetAttackDetail(character[num]);
			}
		}
	}
}

// 攻撃の対象を選択
void CharacterManager::ChoiseAttack(int x, int y)
{
	// 選択した位置に敵がいたら攻撃対象のリファレンスを作成
	for (size_t num = 0; num < character.size(); num++) {
		if (myCharacter != character[num] && character[num]->myStatus->PosX == x && character[num]->myStatus->PosY == y) {
			eCharacter = character[num];
			myCharacter->myStatus->isAttack = true;
		}

		// 攻撃可能なユニットがいなかったら終了
		if (myCharacter == character[num]) character[num]->myStatus->canAttack = false;
	}
	// 選択した位置に敵がいないなら終了
	if (eCharacter == nullptr) {
		attack = false;
		moveableUnit--;
		if (moveableUnit != 0 && playerTurn == false) AIMgr->Play();
		if (moveableUnit == 0) StartTurn();
	}
}

// 攻撃のアニメーション
void CharacterManager::Attack()
{
	if (myCharacter == nullptr || eCharacter == nullptr) return;

	// 1回目の攻撃
	if (myCharacter != nullptr && eCharacter != nullptr && attackCount < 1) {
		if (myCharacter->myStatus->isAttack) attack = myCharacter->AttackAnimation(eCharacter, 1);
		if (eCharacter->myStatus->isAttack) attack = eCharacter->AttackAnimation(myCharacter, 2);

		// アニメーションが終わっていないならリターン
		if (attack) return;
	}

	// アニメーションが終わっているなら攻撃の回数を記録
	if(attack == false) attackCount = 3;

	// 2回目の攻撃
	if (myCharacter != nullptr && eCharacter != nullptr && attackCount > 2) {
		if (myCharacter->myStatus->isAttack) attack = myCharacter->AttackAnimation(eCharacter, 3);
		if (eCharacter->myStatus->isAttack) attack = eCharacter->AttackAnimation(myCharacter, 4);
		
		// アニメーションが終わっていないならリターン
		if (attack) return;
	}

	// 攻撃終了
	myCharacter->myStatus->isAttack = false;
	eCharacter->myStatus->isAttack = false;

	// アニメーションが終わったなら攻撃終了
	if (attack == false) {
		TimeCount::Instance()->SetCount();
		attackCount = 0;
		moveableUnit--;
		for (size_t num = 0; num < character.size(); num++) {
			// 死亡したユニットを除外する
			if (character[num]->myStatus->isDeath) {
				character.erase(character.begin() + num);
				AIMgr->Initialize();
				// 敵AIの初期化
				for (Character* _character : character) {
					AIMgr->CharacterCount(_character);
				}
			}
		}
	}
}

// 攻撃中のデータ表示
void CharacterManager::DrawAttackParam(Character* attackChara, Character* defenceChara)
{
	float drawOffset = 150;

	if (attackChara->myStatus->PosY >= STAGE1_HEIGHT / 2) {
		drawOffset = -100;
		DrawGraph(0, (int)drawOffset, DamageDetail, true);
	}
	else if (attackChara->myStatus->PosY < STAGE1_HEIGHT / 2) {
		DrawGraph(0, (int)drawOffset, DamageDetail, true);
	}

	float A_drawPosX = 390.0f, A_drawPosY = 250 + drawOffset;
	float D_drawPosX = 190.0f, D_drawPosY = 250 + drawOffset;

	// 攻撃側の情報の描画
	DrawFormatString((int)A_drawPosX, (int)A_drawPosY - 50, GetColor(0, 0, 0), attackChara->myStatus->myParam.NAME.c_str());
	DrawFormatString((int)A_drawPosX - 30, (int)A_drawPosY, GetColor(0, 0, 0), "%d", attackChara->myStatus->myParam.HP);
	DrawExtendGraphF(A_drawPosX, A_drawPosY,
		A_drawPosX + 100, A_drawPosY + 15, HpBarBox, true);
	DrawExtendGraphF(A_drawPosX, A_drawPosY,
		A_drawPosX + (100 * ((float)attackChara->myStatus->myParam.HP / (float)attackChara->myStatus->myParam.MaxHP)), A_drawPosY + 15, HpBar, true);

	// 防御側の情報の描画
	DrawFormatString((int)D_drawPosX, (int)D_drawPosY - 50, GetColor(0, 0, 0), defenceChara->myStatus->myParam.NAME.c_str());
	DrawFormatString((int)D_drawPosX - 30, (int)D_drawPosY, GetColor(0, 0, 0), "%d", defenceChara->myStatus->myParam.HP);
	DrawExtendGraphF(D_drawPosX, D_drawPosY,
		D_drawPosX + 100, D_drawPosY + 15, HpBarBox, true);
	DrawExtendGraphF(D_drawPosX, D_drawPosY,
		D_drawPosX + (100 * ((float)defenceChara->myStatus->myParam.HP / (float)defenceChara->myStatus->myParam.MaxHP)), D_drawPosY + 15, HpBar, true);
}

// カメラとのオフセットの計算
void CharacterManager::SetCameraOffset(int dir, bool horizontal)
{
	for (size_t num = 0; num < character.size(); num++) {
		character[num]->SetCameraOffset(dir, horizontal);
	}
}

void CharacterManager::KeyCheck(int x, int y)
{
	for (size_t i = 0; i < character.size(); i++) {
		// カーソルが合っているユニットのみ表示
		if (character[i]->myStatus->PosX != x || character[i]->myStatus->PosY != y) {
			character[i]->OldPosX.clear();
			character[i]->OldPosY.clear();
			character[i]->moveCount = 0;
			isSelect = false;
		}
	}
}

void CharacterManager::Finalize() {

}