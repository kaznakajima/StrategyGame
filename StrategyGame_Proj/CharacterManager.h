#pragma once
#include "Character.h"

// キャラクターのデータ
#define CHARACTER_DATA_1 "ロード.dat"
#define CHARACTER_DATA_2 "サポート.dat"
#define CHARACTER_DATA_3 "敵1.dat"
#define CHARACTER_DATA_4 "敵2.dat"
#define DAMAGE_DETAIL "DamageDetail.png"
#define HP_BAR "HP_Bar.png"
#define HP_BARBOX "HP_BarBox.png"

class CharacterManager : public Singleton<CharacterManager>
{
public:
	~CharacterManager();

	// 生成するキャラクター
	vector<shared_ptr<Character>> _character;

	// 攻撃するユニット
	int attackUnitNum = 0;
	shared_ptr<Character> _myCharacter = nullptr;
	// 攻撃されるユニット
	int blockUnitNum = 0;
	shared_ptr<Character> _eCharacter = nullptr;
	// 全体の攻撃回数
	int attackCount = 0;

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
	void DrawAttackParam(shared_ptr<Character> const &attackChara, shared_ptr<Character> const &defenceChara);

	// カメラとのオフセット計算
	void SetCameraOffset(int dir, bool horizontal);

	// 終了処理
	void Finalize();

private:
	// HP用画像
	int HpBar;
	int HpBarBox;
	int DamageDetail;
	// 行動可能なユニット数
	int moveableUnit = 0;
};