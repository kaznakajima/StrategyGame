#include "CharacterManager.h"
#include "AIManager.h"

// 生成するキャラクター数
const int PlayerNum = 5;

CharacterManager::~CharacterManager() {
	//Finalize();
}

// 初期化
void CharacterManager::Initialize()
{
	FileManager::Instance()->GetFileHandle(HP_BAR);
	FileManager::Instance()->GetFileHandle(HP_BARBOX);
	FileManager::Instance()->GetFileHandle(DAMAGE_DETAIL);

	// キャラクターの追加
	for (size_t num = 0; num < PlayerNum; num++) {
		_character.push_back(make_shared<Character>());
	}

	// キャラクターの初期化
	_character[0]->Character_Initialize(CHARACTER_DATA_1, "Player", 240, 240);
	_character[1]->Character_Initialize(CHARACTER_DATA_3, "Enemy", 144, 240);
	_character[2]->Character_Initialize(CHARACTER_DATA_2, "Player", 96, 384);
	_character[3]->Character_Initialize(CHARACTER_DATA_4, "Enemy", 336, 96);

	// 敵AIの初期化
	for (size_t num = 0; num < _character.size(); ++num) {
		CharacterCount(_character[num]);
	}
	AIManager::Instance()->Initialize();

	StartTurn();
}

// ユニットリストのリセット
void CharacterManager::ResetCharacterList()
{
	_playerList.clear();
	_enemyList.clear();
}

// // 現在の敵(AI)、プレイヤーのカウント
void CharacterManager::CharacterCount(shared_ptr<Character> const & character)
{
	if (character->myStatus->myTeam == "Player") _playerList.push_back(character);
	else if (character->myStatus->myTeam == "Enemy") _enemyList.push_back(character);
}

// キャラクター更新
void CharacterManager::Update(int x, int y)
{
	// 攻撃範囲検索
	GetAttackArea(x, y);

	for (size_t num = 0; num < _character.size(); ++num) {
		// アニメーション
		_character[num]->CharacterAnim();
	}

	// 詳細情報描画
	if (checkCharacter != nullptr) checkCharacter->DrawCharacterDetail();

	// キャラクター移動
	if (isSelect == false) CharacterMove(x, y);

	// 攻撃
	if (attack) Attack();

	// 攻撃中のデータ表示
	if (_myCharacter != nullptr && _eCharacter != nullptr) {
		// 攻撃から約0.5秒たったら攻撃終了
		if (TimeCount::Instance()->GetTimer(500.0f) >= 500.0f) {
			_myCharacter.reset();
			_eCharacter.reset();
			moveableUnit--;
			for (size_t num = 0; num < _character.size(); num++) {
				// 死亡したユニットを除外する
				if (_character[num]->myStatus->isDeath) {
					_character[num].reset();
					_character.erase(_character.begin() + num);
					// ユニットリストの初期化
					ResetCharacterList();
					for (size_t num = 0; num < _character.size(); ++num) {
						CharacterCount(_character[num]);
					}
					AIManager::Instance()->Initialize();
				}
			}
			if (moveableUnit <= 0) { StartTurn(); return; }
			if (moveableUnit != 0 && playerTurn == false) AIManager::Instance()->Play();
			return;
		}
		DrawAttackParam(_myCharacter, _eCharacter);
	}
}

// ターン開始
void CharacterManager::StartTurn()
{
	playerTurn = !playerTurn;
	moveableUnit = 0;

	for (size_t num = 0; num < _character.size(); num++) {
		_character[num]->TurnStart();

		// 死亡したユニットを除外する
		if (_character[num]->myStatus->isDeath) {
			_character[num].reset();
			_character.erase(_character.begin() + num);
			// ユニットリストの初期化
			ResetCharacterList();
			for (size_t num = 0; num < _character.size(); ++num) {
				CharacterCount(_character[num]);
			}
			AIManager::Instance()->Initialize();
		}
		else {
			// 移動可能なユニットのカウント
		    // プレイヤー側ユニットの計算
			if (playerTurn && _character[num]->myStatus->myTeam == "Player") moveableUnit++;
			// 敵側ユニットの計算
			else if (playerTurn == false && _character[num]->myStatus->myTeam == "Enemy") moveableUnit++;
		}
	}

	AudioManager::Instance()->playSE(SE_TURNSTART);
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

	for (size_t num = 0; num < _character.size(); num++) {
		// プレイヤーターン
		if (playerTurn && _character[num]->myStatus->myTeam == "Player") {
			// カーソルが合っているユニットのみ表示
			if (_character[num]->myStatus->xPos == x && _character[num]->myStatus->yPos == y) {
				if (_character[num]->myStatus->canMove) {
					_character[num]->myStatus->isSelect = true;
					_character[num]->myStatus->AnimHandle = 4.0f;
					isSelect = true;
					attack = false;
				}
			}
		}
		// 敵ターン
		else if (playerTurn == false && _character[num]->myStatus->myTeam == "Enemy") {
			// カーソルが合っているユニットのみ表示
			if (_character[num]->myStatus->xPos == x && _character[num]->myStatus->yPos == y) {
				if (_character[num]->myStatus->canMove) {
					_character[num]->myStatus->isSelect = true;
					_character[num]->myStatus->AnimHandle = 4.0f;
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
	for (size_t num = 0; num < _character.size(); num++) {
		// 移動順路を記録しつつ移動範囲と攻撃範囲の描画
		if (_character[num]->myStatus->isSelect) {
			_character[num]->MoveAreaClear(_character);
			_character[num]->OldPosX.push_back(_character[num]->myStatus->xPos);
			_character[num]->OldPosY.push_back(_character[num]->myStatus->yPos);
			_character[num]->MoveRange(_character[num]->myStatus->xPos, _character[num]->myStatus->yPos, _character[num]->myStatus->myParam.MOVERANGE);
			_character[num]->AttackRange();
			return;
		}
	}
}

void CharacterManager::CharacterMove(int x, int y) 
{
	// 移動完了したかどうか
	bool moveEnd = false;

	// キャラクターの移動
	for (size_t num = 0; num < _character.size(); num++) {
		if (_character[num]->myStatus->isSelect) {
			isMove = _character[num]->CharacterMove(x, y);

			// 移動が完了し、攻撃しないなら行動終了
			if (isMove == false && _character[num]->myStatus->canMove == false
				&& _character[num]->myStatus->canAttack == false) {
				moveableUnit--;
				moveEnd = true;
			}
		}
	}

	// 行動可能なユニットがいないならターン終了
	if (moveEnd && playerTurn == false && moveableUnit != 0) AIManager::Instance()->Play();
	if (moveableUnit <= 0) StartTurn();
}

// 移動値の取得
void CharacterManager::GetMoveCount(int x, int y) {
	for (size_t num = 0; num < _character.size(); num++) {
		_character[num]->OldPosX.push_back(x);
		_character[num]->OldPosY.push_back(y);
		_character[num]->moveCount++;
	}
}

void CharacterManager::GetMoveArrow(int x, int y)
{
	// 移動順路を描画
	for (size_t num = 0; num < _character.size(); num++) {
		if (_character[num]->myStatus->isSelect) {
			_character[num]->DrawMoveArrow(x, y, 5);

			// ユニットの位置に戻ったら順路をクリア
			if (_character[num]->myStatus->xPos == x && _character[num]->myStatus->yPos == y) {
				_character[num]->OldPosX.clear();
				_character[num]->OldPosY.clear();
				_character[num]->moveCount = 0;
			}
		}
	}
}

// 攻撃範囲表示
void CharacterManager::GetAttackArea(int x, int y)
{
	if (attackCount != 0) return;

	// 攻撃可能なユニットの取得
	for (size_t num = 0; num < _character.size(); num++) {
		if (_character[num]->myStatus->canAttack) {
			_myCharacter = _character[num];
			_myCharacter->AttackableDraw();
			attack = true;
		}
	}

	if (playerTurn == false) return;

	// 攻撃可能な位置のユニットとの攻撃した際の詳細情報の表示
	for (size_t num = 0; num < _character.size(); num++) {
		if (_myCharacter != nullptr && _myCharacter->myStatus->canAttack) {
			if (_character[num]->myStatus->xPos == x && _character[num]->myStatus->yPos == y
				&& StageCreate::Instance()->onUnit[y / CHIP_SIZE][x / CHIP_SIZE] == "Enemy") {
				if (_myCharacter->myStatus->myTeam != _character[num]->myStatus->myTeam) _myCharacter->GetAttackDetail(_character[num]);
			}
		}
	}
}

// 攻撃の対象を選択
void CharacterManager::ChoiseAttack(int x, int y)
{
	// 選択した位置に敵がいたら攻撃対象のリファレンスを作成
	for (size_t num = 0; num < _character.size(); num++) {
		if (_myCharacter != _character[num] && _character[num]->myStatus->xPos == x && _character[num]->myStatus->yPos == y) {
			_eCharacter = _character[num];
			_myCharacter->myStatus->isAttack = true;
		}

		// 攻撃可能なユニットがいなかったら終了
		//if (myCharacter == character[num]) character[num]->myStatus->canAttack = false;
	}
	// 選択した位置に敵がいないなら終了
	if (_eCharacter == nullptr) {
		attack = false;
		moveableUnit--;
		if (moveableUnit != 0 && playerTurn == false) AIManager::Instance()->Play();
		if (moveableUnit == 0) StartTurn();
	}
}

// 攻撃のアニメーション
void CharacterManager::Attack()
{
	if (_myCharacter == nullptr || _eCharacter == nullptr) return;

	// 1回目の攻撃
	if (_myCharacter != nullptr && _eCharacter != nullptr && attackCount < 1) {
		if (_myCharacter->myStatus->isAttack) attack = _myCharacter->AttackAnimation(_eCharacter, 1);
		if (_eCharacter->myStatus->isAttack && _myCharacter->myStatus->isAttack == false) attack = _eCharacter->AttackAnimation(_myCharacter, 2);

		// アニメーションが終わっていないならリターン
		if (attack) return;
	}

	// アニメーションが終わっているなら攻撃の回数を記録
	if(attack == false) attackCount = 3;

	// 2回目の攻撃
	if (_myCharacter != nullptr && _eCharacter != nullptr && attackCount > 2) {
		if (_myCharacter->myStatus->isAttack) attack = _myCharacter->AttackAnimation(_eCharacter, 3);
		if (_eCharacter->myStatus->isAttack&& _myCharacter->myStatus->isAttack == false) attack = _eCharacter->AttackAnimation(_myCharacter, 4);
		
		// アニメーションが終わっていないならリターン
		if (attack) return;
	}

	// アニメーションが終わったなら攻撃終了
	if (attack == false) {
		TimeCount::Instance()->SetCount();
		attackCount = 0;
		_myCharacter->myStatus->canAttack = false;
		_eCharacter->myStatus->canAttack = false;
	}
}

// 攻撃中のデータ表示
void CharacterManager::DrawAttackParam(shared_ptr<Character> const &attackChara, shared_ptr<Character> const &defenceChara)
{
	float drawOffset = 150;

	// 攻撃側ユニットの位置に応じて表示位置を変更
	if (attackChara->myStatus->_yPos >= STAGE1_HEIGHT / 2) {
		drawOffset = -100;
		DrawRotaGraph(336, 240 + (int)drawOffset, 1.0f, 0.0f, FileManager::Instance()->GetFileHandle(DAMAGE_DETAIL), true);
	}
	else if (attackChara->myStatus->_yPos < STAGE1_HEIGHT / 2) {
		DrawRotaGraph(336, 240 + (int)drawOffset, 1.0f, 0.0f, FileManager::Instance()->GetFileHandle(DAMAGE_DETAIL), true);
	}

	// 情報(体力、名前)の表示位置の定義
	float A_drawPosX = 390.0f, A_drawPosY = 260 + drawOffset;
	float D_drawPosX = 190.0f, D_drawPosY = 260 + drawOffset;

	// 攻撃側の情報の描画
	DrawFormatString((int)A_drawPosX, (int)A_drawPosY - 50, GetColor(0, 0, 0), attackChara->myStatus->NAME.c_str());
	DrawFormatString((int)A_drawPosX - 30, (int)A_drawPosY, GetColor(0, 0, 0), "%d", attackChara->myStatus->myParam.HP);
	DrawExtendGraphF(A_drawPosX, A_drawPosY,
		A_drawPosX + 100, A_drawPosY + 15, FileManager::Instance()->GetFileHandle(HP_BARBOX), true);
	DrawExtendGraphF(A_drawPosX, A_drawPosY,
		A_drawPosX + (100 * ((float)attackChara->myStatus->myParam.HP / (float)attackChara->myStatus->myParam.MaxHP)), A_drawPosY + 15, FileManager::Instance()->GetFileHandle(HP_BAR), true);

	// 防御側の情報の描画
	DrawFormatString((int)D_drawPosX, (int)D_drawPosY - 50, GetColor(0, 0, 0), defenceChara->myStatus->NAME.c_str());
	DrawFormatString((int)D_drawPosX - 30, (int)D_drawPosY, GetColor(0, 0, 0), "%d", defenceChara->myStatus->myParam.HP);
	DrawExtendGraphF(D_drawPosX, D_drawPosY,
		D_drawPosX + 100, D_drawPosY + 15, FileManager::Instance()->GetFileHandle(HP_BARBOX), true);
	DrawExtendGraphF(D_drawPosX, D_drawPosY,
		D_drawPosX + (100 * ((float)defenceChara->myStatus->myParam.HP / (float)defenceChara->myStatus->myParam.MaxHP)), D_drawPosY + 15, FileManager::Instance()->GetFileHandle(HP_BAR), true);
}

// カメラとのオフセットの計算
void CharacterManager::SetCameraOffset(int dir, bool horizontal)
{
	for (size_t num = 0; num < _character.size(); num++) {
		_character[num]->SetCameraOffset(dir, horizontal);
	}
}

// 入力した地点のチェック
void CharacterManager::KeyCheck(int x, int y)
{
	for (size_t num = 0; num < _character.size(); num++) {
		// カーソルが合っていなければ選択状態を解除
		if (_character[num]->myStatus->xPos != x || _character[num]->myStatus->yPos != y) {
			_character[num]->OldPosX.clear();
			_character[num]->OldPosY.clear();
			_character[num]->moveCount = 0;
			isSelect = false;
		}
	}
}

void CharacterManager::CheckDetail(int x, int y)
{
	// 現在詳細情報を描画しているなら閉じる
	if (isDetail) { 
		isDetail = false; 
		checkCharacter = nullptr; 
		return; 
	}

	// カーソルが合っていれば詳細情報描画
	for (size_t num = 0; num < _character.size(); num++) {
		if (_character[num]->myStatus->xPos == x && _character[num]->myStatus->yPos == y) {
			isDetail = true;
			checkCharacter = _character[num];
		}
	}
}

// チェックするユニットの変更
void CharacterManager::ChangeDetailCharacter(shared_ptr<Character> const & character, int _index)
{
	// インデックス番号
	int index = 0;
	// 最初にチェックしたユニットの陣営側だけをチェック
	if (character->myStatus->myTeam == "Player") {
		// インデックス番号の取得
		for (int num = 0; num < _playerList.size(); ++num) {
			if (_playerList[num] == character) index = num;
		}
		// 次の要素へ
		index += _index;
		// 要素数を超えないように調整
		if (index == _playerList.size()) index = 0;
		else if (index < 0) index = _playerList.size() - 1;
		checkCharacter = _playerList[index];
	}
	else if (character->myStatus->myTeam == "Enemy") {
		// インデックス番号の取得
		for (int num = 0; num < _enemyList.size(); ++num) {
			if (_enemyList[num] == character) index = num;
		}
		// 次の要素へ
		index += _index;
		// 要素数を超えないように調整
		if (index == _enemyList.size()) index = 0;
		else if (index < 0) index = _enemyList.size() - 1;
		checkCharacter = _enemyList[index];
	}
}

void CharacterManager::Finalize() {
	if (_character.empty() == false) _character.clear();
	if (_myCharacter != nullptr) _myCharacter.reset();
	if (_eCharacter != nullptr) _eCharacter.reset();
}
