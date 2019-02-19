#pragma once

// キャラクターのデータ
#define CHARACTER_DATA_1 "Resources\\CharacterData\\PlayerData.dat"
#define CHARACTER_DATA_2 "Resources\\CharacterData\\PlayerData_1.dat"
#define CHARACTER_DATA_3 "Resources\\CharacterData\\PlayerData_2.dat"

class CharacterManager
{
public:
	// 選択判定
	bool isSelect;
	// 移動判定
	bool isMove;
	// 攻撃判定
	bool attack;

	// 初期化
	void Initialize();
	// 更新
	void Update();

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
	// 攻撃
	void Attack(int x, int y);

	// カメラとのオフセット計算
	void SetCameraOffset(int dir, bool horizontal);

	// 終了
	void Finalize();
};