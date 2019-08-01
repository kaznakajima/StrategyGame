#include "CharacterManager.h"
#include "AIManager.h"

CharacterManager::~CharacterManager() {}

// 初期化
void CharacterManager::Initialize()
{
	SetCharacterData();

	FileManager::Instance()->GetFileHandle(HP_BAR);
	FileManager::Instance()->GetFileHandle(HP_BARBOX);
	FileManager::Instance()->GetFileHandle(DAMAGE_DETAIL);

	int allCharacter = StageCreate::Instance()->playerCount + StageCreate::Instance()->enemyCount;
	// キャラクターの追加
	for (size_t num = 0; num < allCharacter; num++) {
		_character.push_back(make_shared<Character>());
	}

	for (int num = 0; num < StageCreate::Instance()->playerCount; num++) {
		_character[num]->Character_Initialize(playerDataPass[num], "Player", StageCreate::Instance()->p_InitPos[num * 2], StageCreate::Instance()->p_InitPos[num * 2 + 1]);
	}
	for (int num = 0; num < StageCreate::Instance()->enemyCount; num++) {
		_character[StageCreate::Instance()->playerCount + num]->Character_Initialize(enemyDataPass[num], "Enemy", StageCreate::Instance()->e_InitPos[num * 2], StageCreate::Instance()->e_InitPos[num * 2 + 1]);
	}

	// 敵AIの初期化
	for (size_t num = 0; num < _character.size(); ++num) {
		CharacterCount(_character[num]);
	}
	AIManager::Instance()->Initialize();

	for (int y = 0; y < StageCreate::Instance()->MAP_SIZEY; y++) {
		for (int x = 0; x < StageCreate::Instance()->MAP_SIZEX; x++) {
			shared_ptr<Renderer> ren = make_shared<Renderer>(FileManager::Instance()->GetFileHandle(CAN_MOVE_AREA), 2, 0);
			ren->SetPosition(x * CHIP_SIZE, y * CHIP_SIZE);
			ren->SetVisible(false);
			DrawManager::Instance()->AddDrawList(ren);
			ren = make_shared<Renderer>(FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA), 2, 0);
			ren->SetPosition(x * CHIP_SIZE, y * CHIP_SIZE);
			ren->SetVisible(false);
			DrawManager::Instance()->AddDrawList(ren);
		}
	}

	playerTurn = false;
	isGame = true;
	StartTurn();
}

// ユニットを選択状態にする
void CharacterManager::CanSelectCharacter(shared_ptr<Character> const & character, int x, int y)
{
	// 敵ターンは除外
	if (playerTurn == false && character->myStatus->myTeam == "Player") return;

	// カーソル位置と同じユニットを選択状態にする
	if (character->myStatus->xPos == x && character->myStatus->yPos == y) {
		if (character->myStatus->canMove) {
			character->myStatus->isSelect = true;
			character->myStatus->AnimHandle = 4.0f;
			isSelect = true;
			attack = false;
			Draw();
		}
	}
}

// データパスの設定
void CharacterManager::SetCharacterData()
{
	// プレイヤーパスの設定
	playerDataPass = vector<string>(StageCreate::Instance()->playerCount, "");
	for (int pass = 0; pass < playerDataPass.size(); ++pass) {
		switch (pass) {
		// ロード
		case 0:
			playerDataPass[pass] = UNIT_LOAD;
			break;
		// サポート
		case 1:
			playerDataPass[pass] = UNIT_SUPPORT;
			break;
		}
	}

	// 敵のパスの設定
	enemyDataPass = vector<string>(StageCreate::Instance()->enemyCount, "");
	for (int pass = 0; pass < enemyDataPass.size(); ++pass) {
		switch (pass) {
		// 敵1
		case 0:
			enemyDataPass[pass] = UNIT_ENEMY1;
			break;
		// 敵2
		case 1:
			enemyDataPass[pass] = UNIT_ENEMY2;
			break;
		default:
			enemyDataPass[pass] = UNIT_ENEMY1;
			break;
		}
	}
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
	// 攻撃可能かチェック
	GetAttackArea(x, y);

	for (size_t num = 0; num < _character.size(); ++num) {
		// アニメーション
		_character[num]->CharacterAnim();

		DrawAttackDetail(_character[num], x, y);
	}

	// 詳細情報描画
	if (checkCharacter != nullptr) checkCharacter->DrawCharacterDetail();

	// キャラクター移動
	if (isSelect == false) CharacterMove(x, y);

	// 攻撃
	if (attack) Attack();

	// 攻撃中でないならリターン
	if (_myCharacter == nullptr || _eCharacter == nullptr) return;

	// 攻撃中なら以下の処理が実行される

	// 攻撃から約0.5秒たったら攻撃終了
	if (TimeCount::Instance()->GetTimer(500.0f) >= 500.0f) {
		_myCharacter.reset();
		_eCharacter.reset();
		moveableUnit--;
		for (size_t num = 0; num < _character.size(); num++) {
			// 死亡したユニットを除外する
			if (_character[num]->myStatus->isDeath) {
				// プレイヤー指揮官がやられたらゲーム終了
				if (_character[num]->myStatus->NAME == "Load") {
					_character[num].reset();
					_character.erase(_character.begin() + num);
					_playerList.clear(); 
					isGame = false;
					return;
				}
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
		// プレイヤーまたは敵が1体も残っていないならゲーム終了
		if (_playerList.empty() || _enemyList.empty()) { isGame = false; return; }
		if (moveableUnit <= 0) { StartTurn(); return; }
		if (moveableUnit != 0 && playerTurn == false) AIManager::Instance()->Play();
		return;
	}
	DrawDamageDetail(_myCharacter, _eCharacter);
}

// ターン開始
void CharacterManager::StartTurn()
{
	playerTurn = !playerTurn;
	moveableUnit = 0;

	for (size_t num = 0; num < _character.size(); num++) {
		_character[num]->TurnStart();

		// 移動可能なユニットのカウント
		// プレイヤー側ユニットの計算
		if (playerTurn && _character[num]->myStatus->myTeam == "Player") moveableUnit++;
		// 敵側ユニットの計算
		else if (playerTurn == false && _character[num]->myStatus->myTeam == "Enemy") moveableUnit++;
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

	for (size_t num = 0; num < _character.size(); num++) {
		// ユニットが選択可能かチェック
		CanSelectCharacter(_character[num], x, y);
	}
}

// 描画処理
void CharacterManager::Draw()
{
	for (size_t num = 0; num < _character.size(); num++) {
		// 移動順路を記録しつつ移動範囲と攻撃範囲の描画
		if (_character[num]->myStatus->isSelect) {
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
			if (playerTurn && _character[num]->myStatus->myTeam == "Enemy") {
				_character[num]->myStatus->isSelect = false;
				_character[num]->myStatus->AnimHandle = 0.0f;
				return;
			}

			isMove = _character[num]->CharacterMove(x, y);

			// 移動が完了し、攻撃しないなら行動終了
			if (isMove == false && _character[num]->myStatus->canMove == false
				&& _character[num]->myStatus->canAttack == false) {
				moveableUnit--;
				moveEnd = true;
				MoveAreaClear();
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
			if (playerTurn && _character[num]->myStatus->myTeam == "Enemy") return;

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

void CharacterManager::MoveAreaClear()
{
	for (int y = 0; y < StageCreate::Instance()->MAP_SIZEY; y++) {
		for (int x = 0; x < StageCreate::Instance()->MAP_SIZEX; x++) {
			int posX = x * CHIP_SIZE, posY = y * CHIP_SIZE;
			DrawManager::Instance()->GetDrawParts(FileManager::Instance()->GetFileHandle(CAN_MOVE_AREA), posX, posY)->SetVisible(false);
			DrawManager::Instance()->GetDrawParts(FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA), posX, posY)->SetVisible(false);
			for (size_t num = 0; num < _character.size(); ++num) {
				_character[num]->ResetArea(x, y);
				StageCreate::Instance()->StageUpdate(x, y);
				if (_character[num]->myStatus->xPos == x * CHIP_SIZE
					&& _character[num]->myStatus->yPos == y * CHIP_SIZE) StageCreate::Instance()->CheckOnUnit(x, y, _character[num]->myStatus->myTeam);
			}
		}
	}
}

// 攻撃可能エリア取得
void CharacterManager::GetAttackArea(int x, int y)
{
	// 攻撃可能なユニットの取得
	for (size_t num = 0; num < _character.size(); num++) {
		if (_character[num]->myStatus->canAttack) {
			_myCharacter = _character[num];
			_myCharacter->AttackableDraw();
			attack = true;
		}
	}
}

// 攻撃の詳細情報表示
void CharacterManager::DrawAttackDetail(shared_ptr<Character> const &character, int x, int y)
{
	// 攻撃できるユニットがいない、敵ターン、攻撃中はリターン、
	if (_myCharacter == nullptr || playerTurn == false || attackCount != 0) return;

	// 攻撃可能な位置のユニットとの攻撃した際の詳細情報の表示
	if (_myCharacter->myStatus->canAttack) {
		if (character->myStatus->xPos == x && character->myStatus->yPos == y
			&& StageCreate::Instance()->onUnit[y / CHIP_SIZE][x / CHIP_SIZE] == "Enemy") {
			if (_myCharacter->myStatus->myTeam != character->myStatus->myTeam) _myCharacter->GetAttackDetail(character);
		}
	}
}

// 攻撃の対象を選択
void CharacterManager::ChoiseAttack(int x, int y)
{
	// 選択した位置に敵がいたら攻撃対象のリファレンスを作成
	for (size_t num = 0; num < _character.size(); num++) {
		if (_myCharacter != _character[num] && _character[num]->myStatus->xPos == x && _character[num]->myStatus->yPos == y) {
			if (_myCharacter->myStatus->myTeam != _character[num]->myStatus->myTeam) {
				_eCharacter = _character[num];
				_myCharacter->myStatus->isAttack = true;
			}
		}

		// 攻撃可能なユニットがいなかったら終了
		//if (myCharacter == character[num]) character[num]->myStatus->canAttack = false;
	}
	// 選択した位置に敵がいないなら終了
	if (_eCharacter == nullptr) {
		_myCharacter->myStatus->canAttack = false;
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
	if (_myCharacter != nullptr && _eCharacter != nullptr && attackCount < 2) {
		if (_myCharacter->myStatus->isAttack) attack = _myCharacter->AttackAnimation(_eCharacter, 1);
		if (_eCharacter->myStatus->isAttack && _myCharacter->myStatus->isAttack == false) attack = _eCharacter->AttackAnimation(_myCharacter, 2);

		// アニメーションが終わっていないならリターン
		if (attack) { attackCount = 1; return; }
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
void CharacterManager::DrawDamageDetail(shared_ptr<Character> const &attackChara, shared_ptr<Character> const &defenceChara)
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
	DrawFormatString((int)A_drawPosX - 30, (int)A_drawPosY, GetColor(0, 0, 0), "%d", attackChara->myStatus->HP);
	DrawExtendGraphF(A_drawPosX, A_drawPosY,
		A_drawPosX + 100, A_drawPosY + 15, FileManager::Instance()->GetFileHandle(HP_BARBOX), true);
	DrawExtendGraphF(A_drawPosX, A_drawPosY,
		A_drawPosX + (100 * ((float)attackChara->myStatus->HP / (float)attackChara->myStatus->myParam.MaxHP)), A_drawPosY + 15, FileManager::Instance()->GetFileHandle(HP_BAR), true);

	// 防御側の情報の描画
	DrawFormatString((int)D_drawPosX, (int)D_drawPosY - 50, GetColor(0, 0, 0), defenceChara->myStatus->NAME.c_str());
	DrawFormatString((int)D_drawPosX - 30, (int)D_drawPosY, GetColor(0, 0, 0), "%d", defenceChara->myStatus->HP);
	DrawExtendGraphF(D_drawPosX, D_drawPosY,
		D_drawPosX + 100, D_drawPosY + 15, FileManager::Instance()->GetFileHandle(HP_BARBOX), true);
	DrawExtendGraphF(D_drawPosX, D_drawPosY,
		D_drawPosX + (100 * ((float)defenceChara->myStatus->HP / (float)defenceChara->myStatus->myParam.MaxHP)), D_drawPosY + 15, FileManager::Instance()->GetFileHandle(HP_BAR), true);
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
		else if (index < 0) index = (int)_playerList.size() - 1;
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
		else if (index < 0) index = (int)_enemyList.size() - 1;
		checkCharacter = _enemyList[index];
	}
}

// 終了処理
void CharacterManager::Finalize() {
	if (_character.empty() == false) _character.clear();
	if (_myCharacter != nullptr) _myCharacter.reset();
	if (_eCharacter != nullptr) _eCharacter.reset();
	if (checkCharacter != nullptr) checkCharacter.reset();
}
