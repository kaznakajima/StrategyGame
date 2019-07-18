#pragma once
#include "Character.h"

// キャラクターのデータ
#define UNIT_LOAD "Load.dat"
#define UNIT_SUPPORT "Support.dat"
#define UNIT_ENEMY1 "Enemy1.dat"
#define UNIT_ENEMY2 "Enemy2.dat"
#define DAMAGE_DETAIL "DamageDetail.png"
#define HP_BAR "HP_Bar.png"
#define HP_BARBOX "HP_BarBox.png"

class CharacterManager : public Singleton<CharacterManager>
{
public:
	~CharacterManager();

	// 生成するキャラクター
	vector<shared_ptr<Character>> _character;

	// 敵(AI)キャラクターのリスト
	vector<shared_ptr<Character>> _enemyList;
	// プレイヤー側のキャラクターのリスト
	vector<shared_ptr<Character>> _playerList;

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
	bool isMove = false;
	// 攻撃判定
	bool attack;
	// ゲームが終了したかどうか
	bool isGame = true;

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
	// ユニットの詳細情報検索
	bool isDetail;
	void CheckDetail(int x, int y);
	// 現在チェック中のユニット
	shared_ptr<Character> checkCharacter = nullptr;
	// チェックするユニットの変更
	void ChangeDetailCharacter(shared_ptr<Character> const &character, int _index);

	// 移動値の取得
	void GetMoveCount(int x, int y);
	// 移動方向取得
	void GetMoveArrow(int x, int y);
	// 移動エリアのリセット
	void MoveAreaClear();

	// カメラとのオフセット計算
	void SetCameraOffset(int dir, bool horizontal);

	// 終了処理
	void Finalize();

private:

	// プレイヤーデータのパス
	vector<string> playerDataPass;
	// 敵データのパス
	vector<string> enemyDataPass;
	// ユニットを選択状態にする
	void CanSelectCharacter(shared_ptr<Character> const &character, int x, int y);
	// プレイヤーと敵のデータのパスを格納
	void SetCharacterData();
	// ユニットリストのリセット
	void ResetCharacterList();
	// 現在の敵(AI)、プレイヤーのカウント
	void CharacterCount(shared_ptr<Character> const &character);

	// ユニットの移動
	void CharacterMove(int x, int y);

	// 攻撃範囲表示
	void GetAttackArea(int x, int y);
	// 攻撃詳細情報表示
	void DrawAttackDetail(shared_ptr<Character> const &character, int x, int y);
	// 攻撃の対象を選択
	void ChoiseAttack(int x, int y);
	// 攻撃の処理
	void Attack();
	// 攻撃中のデータ表示
	void DrawDamageDetail(shared_ptr<Character> const &attackChara, shared_ptr<Character> const &defenceChara);

	// HP用画像
	int HpBar;
	int HpBarBox;
	int DamageDetail;
	// 行動可能なユニット数
	int moveableUnit = 0;
};