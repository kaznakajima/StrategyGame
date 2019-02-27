#pragma once

// キャラクターのデータ
#define CHARACTER_DATA_1 "Resources\\CharacterData\\PlayerData.dat"
#define CHARACTER_DATA_2 "Resources\\CharacterData\\PlayerData_1.dat"
#define CHARACTER_DATA_3 "Resources\\CharacterData\\PlayerData_2.dat"

class CharacterManager
{
public:
	// プレイヤーのターンかどうか
	bool playerTurn = false;
	// 選択判定
	bool isSelect;
	// 移動判定
	bool isMove;
	// 攻撃判定
	bool attack;
	// 全体の攻撃回数
	int attackCount = 0;
	// 行動可能なユニット数
	int moveableUnit = 0;

	// 初期化
	void Initialize();
	// 更新
	void Update(int x, int y);

	// ターン開始
	void StartTurn();

	// 描画するかチェック
	void DrawCheck(int x, int y);
	// 描画
	void Draw();

	// 入力検知
	void KeyCheck(int x, int y);

	// 移動値の取得
	void GetMoveCount(int x, int y);
	// 移動方向取得
	void GetMoveArrow(int x, int y);
	// ユニットの移動
	void CharacterMove(int x, int y);

	// 攻撃範囲表示
	void GetAttackArea(int x, int y);
	// 攻撃の対象を選択
	void ChoiseAttack(int x, int y);
	// 攻撃の処理
	void Attack();

	// カメラとのオフセット計算
	void SetCameraOffset(int dir, bool horizontal);

	// 終了
	void Finalize();
};