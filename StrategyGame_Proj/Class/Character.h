#pragma once
#include "DxLib.h"
#include "StageCreate.h"
#include "AudioManager.h"
#include "Weapon.h"
#include "TimeCount.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define CAN_MOVE_AREA "CanMove.png"
#define CAN_ATTACK_AREA "CanAttack.png"
#define ATTACK_DETAIL "AttackGraph.png"
#define CHARACTER_DETAIL "CharacterParam.png"
#define ARROW "Resources\\Image\\Arrow.png"
#define CHARACTER_IMG "Resources\\Image\\Character_48×48.png"

class Character
{
public:
	Character();

	// キャラクターのパラメータ
	typedef struct {
		int LEVEL;
		int MaxHP;
		int POWER;
		int TECHNIQUE;
		int SPEED;
		int LUCKY;
		int DEFENCE;
		int MAGIC_DEFENCE;
		int PHYSIQUE;
		int MOVERANGE;

		// 攻撃に使うパラメータ
		int ATTACK_STR;  // 攻撃力
		int ATTACK_CLT;  // 必殺
		int ATTACK_AVO; // 回避力
		int ATTACK_HIT;  // 命中力
		int ATTACK_SPEED;
	} PARAM;

	// キャラクターのステータス
	typedef struct {
		PARAM myParam;               // 自身のパラメータ
		string NAME;                      // ユニット名
		string myTeam;                  // 自身の陣営
		int HP;                              // 現在のHP
		int xPos, yPos;                   // 現在のx座標, 現在のy座標
		int _xPos, _yPos;               // 移動前のx座標, 移動前のy座標                           
		int Image[20];                   // キャラクター用画像
		float AnimHandle;              // アニメーション用変数
		int AttackRange;                // 攻撃範囲
		bool animReset;                // アニメーションのリセット 
		bool isSelect;                    // 選択されているか
		bool canMove = true;        // 移動可能か
		bool isAttack;                   // 攻撃中か
		bool canAttack = false;     // 攻撃可能か
		bool isDeath = false;        // 死亡判定
	}STATUS ;

	// 道筋
	vector<int> OldPosX;
	vector<int> OldPosY;

	// 自身のステータス
	unique_ptr<STATUS> myStatus = make_unique<STATUS>();
	// 自身の使用する武器
	vector<unique_ptr<Weapon>> Item;

	// 移動できるエリア
	vector<vector<int>> moveToPos = vector<vector<int>>(StageCreate::Instance()->MAP_SIZEY, vector<int>(StageCreate::Instance()->MAP_SIZEX, -1));

	// 移動先の道筋かどうか
	vector<vector<bool>> moveArrow = vector<vector<bool>>(StageCreate::Instance()->MAP_SIZEY, vector<bool>(StageCreate::Instance()->MAP_SIZEX, 0));

	// 移動値
	int moveCount;

	// 自身のパラメータデータ
	string myData;

	// 初期化
	void Character_Initialize(string pass, string team, int posX, int posY);
	// パラメータ取得
	void GetCharacterParam(string pass);
	// ユニットの詳細情報表示
	void DrawCharacterDetail();

	// ユニットの位置を取得
	VECTOR GetCharacterPosition();

	// 描画
	void SpriteDraw(int x, int y, int img);
	// キャラクターのアニメーション
	void CharacterAnim();

	// 移動再開(ターン開始時に呼び出す)
	void TurnStart();
	// 移動範囲計算
    void MoveRange(int x, int y, int moveRange);
	// 攻撃範囲描画
	void AttackRange();
	// 移動方向描画
	void DrawMoveArrow(int x, int y, int moveValue);
	// キャラクターの移動
	bool CharacterMove(int moveX, int moveY);

	// 攻撃チェック
	void AttackCheck();
	// 攻撃範囲描画
	void AttackableDraw();
	// 攻撃の詳細情報表示
	void GetAttackDetail(shared_ptr<Character> const &eCharacter);
	// 攻撃アニメーション
	bool AttackAnimation(shared_ptr<Character> const &eCharacter, int count);
	// 攻撃の計算
	void CharacterAttack(shared_ptr<Character> const &eCharacter, int count);
	// 攻撃の処理
	void ApplyDamage(shared_ptr<Character> const &eCharacter, int damage);

	// カメラとのオフセット計算
	void SetCameraOffset(int dir, bool horizontal);

	// 移動エリアのリセット
	void MoveAreaClear(vector<shared_ptr<Character>> const &_character);

	// 終了処理
	void Finalize();

private:
	// 移動経路
	int ArrowImage[6];

	// アイテム追加
	void AddItem(string itemName);

	// レベルアップ
	void LevelUp();

	// アイテムの所持数
	int itemCount = 0;

	// パラメータ
	int _param[10];
};