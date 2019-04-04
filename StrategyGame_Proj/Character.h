#pragma once
#include "DxLib.h"
#include "StageCreate.h"
#include "AttackPhase.h"
#include "TimeCount.h"
#include "FileManager.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define CAN_MOVE_AREA "CanMove.png"
#define CAN_ATTACK_AREA "CanAttack.png"
#define ATTACK_DETAIL "AttackGraph.png"
#define ARROW "Resources\\image\\Arrow.png"
#define CHARACTER_IMG "Resources\\image\\_Character_40×40.png"

class Character
{
public:
	Character();

	// プレイヤーステータスの列挙
	enum PLAYER_PARAM
	{
		LEVEL = 0,
		HP,
		POWER,
		TECHNIQUE,
		SPEED,
		LUCKY,
		DEFENCE,
		MAGIC_DEFENCE,
		PHYSIQUE,
		MOVERANGE,
		TEAM
	};

	// キャラクターのパラメータ
	typedef struct {
		int LEVEL;
		int MaxHP;
		int HP;
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
		string myData;                    // 自身のデータ
		string myTeam;                   // 自身の陣営
		int PosX;                             // 現在のx座標
		int PosY;                             // 現在のy座標
		int _PosX;                           // 移動前のx座標
		int _PosY;                           // 移動前のy座標
		int Image[20];                   // キャラクター用画像
		float AnimHandle;              // アニメーション用変数
		int AttackRange;                // 攻撃範囲
		bool animReset;                // アニメーションのリセット 
		bool isSelect;                    // 選択されているか
		bool canMove = true;       // 移動可能か
		bool isAttack;                  // 攻撃中か
		bool canAttack = false;   // 攻撃可能か
		bool isDeath = false;      // 死亡判定
	}STATUS ;

	// 道筋
	vector<int> OldPosX;
	vector<int> OldPosY;

	unique_ptr<STATUS> myStatus = make_unique<STATUS>();

	// 移動できるエリア
	vector<vector<int>> moveToPos = vector<vector<int>>(StageCreate::Instance()->MAP_SIZEY, vector<int>(StageCreate::Instance()->MAP_SIZEX, -1));

	// 移動先の道筋かどうか
	vector<vector<bool>> moveArrow = vector<vector<bool>>(StageCreate::Instance()->MAP_SIZEY, vector<bool>(StageCreate::Instance()->MAP_SIZEX, 0));

	// 移動値
	int moveCount;

	// 初期化
	void Character_Initialize(string pass, string name, string team, int posX, int posY);
	// パラメータ取得
	void GetCharacterParam(string pass);
	void SetParam(string name);

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
	void GetAttackDetail(Character* eCharacter);
	// 攻撃アニメーション
	bool AttackAnimation(Character* eCharacter, int count);
	// 攻撃の計算
	void CharacterAttack(Character* eCharacter, int count);
	// 攻撃の処理
	void CharacterDamage(Character* eCharacter, int damage);

	// カメラとのオフセット計算
	void SetCameraOffset(int dir, bool horizontal);

	// 移動エリアのリセット
	void MoveAreaClear(vector<Character*> _character);

	// 終了処理
	void Finalize();

private:
	// 移動経路
	int ArrowImage[6];

	// レベルアップ
	void LevelUp();

	// パラメータ
	string name;
	int _param[10];
};