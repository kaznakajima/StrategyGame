#pragma once
#include "Character.h"

// キャラクターのデータ
#define CHARACTER_DATA_1 "Resources\\CharacterData\\PlayerData.dat"
#define CHARACTER_DATA_2 "Resources\\CharacterData\\PlayerData_1.dat"
#define CHARACTER_DATA_3 "Resources\\CharacterData\\PlayerData_2.dat"
#define DAMAGE_DETAIL "Resources\\image\\DamageDetail.png"
#define HP_BAR "Resources\\image\\HP_Bar.png"
#define HP_BARBOX "Resources\\image\\HP_BarBox.png"

class CharacterManager : public Singleton<CharacterManager>
{
public:
	// 生成するキャラクター
	vector<Character*> character;

	// 攻撃するユニット
	int attackUnitNum = 0;
	Character* myCharacter = nullptr;
	// 攻撃されるユニット
	int blockUnitNum = 0;
	Character* eCharacter = nullptr;

	// ターン変更アニメーション
	bool turnAnim;
	// プレイヤーのターンかどうか
	bool playerTurn = false;
	// 選択判定
	bool isSelect;
	// 移動判定
	bool isMove;
	// 攻撃判定
	bool attack;

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
	// 攻撃中のデータ表示
	void DrawAttackParam(Character* attackChara, Character* defenceChara);

	// カメラとのオフセット計算
	void SetCameraOffset(int dir, bool horizontal);

private:
	// HP用画像
	int HpBar;
	int HpBarBox;
	int DamageDetail;
	// 全体の攻撃回数
	int attackCount = 0;
	// 行動可能なユニット数
	int moveableUnit = 0;

	// 終了
	void Finalize();
};