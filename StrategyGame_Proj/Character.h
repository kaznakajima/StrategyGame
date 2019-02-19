#pragma once
#include "DxLib.h"
#include "StageCreate.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

#define CAN_MOVE_AREA "Resources\\image\\CanMove.png"
#define CHARACTER_IMG "Resources\\image\\Character_40×40.png"

class Character
{
public:
	Character();

	// プレイヤーステータスの列挙
	enum PLAYER_PARAM
	{
		HP = 0,
		POWER,
		TECHNIQUE,
		SPEED,
		LUCKY,
		DEFENCE,
		MAGIC_DEFENCE,
		PHYSIQUE,
		MOVERANGE
	};

	// キャラクターのパラメータ
	typedef struct {
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
	} PARAM;

	// キャラクターのステータス
	typedef struct {
		PARAM myParam;  // 自身のパラメータ
		int PosX;             // 現在のx座標
		int PosY;             // 現在のy座標
		int _PosX;           // 移動前のx座標
		int _PosY;           // 移動前のy座標
		int Image[20];    // キャラクター用画像
		float AnimHandle;  // アニメーション用変数
		int AttackRange;    // 攻撃範囲
		bool isSelect;      // 選択されているか
		bool isMove;       // 移動中か
		bool isAttack;     // 攻撃中か
		bool canAttack;   // 攻撃可能か
		bool isDeath;      // 死亡判定
	}STATUS ;

	// 道筋
	vector<int> OldPosX;
	vector<int> OldPosY;

	// カウント
	int Count;

	// 移動値
	int moveCount;

	// 初期化
	void Character_Initialize(STATUS* status, string pass, int posX, int posY);
	// パラメータ取得
	void GetCharacterParam(string pass);

	// 描画
	void SpriteDraw(int x, int y, int img);
	// キャラクターのアニメーション
	void CharacterAnim(STATUS* status);

	// 移動範囲計算
    void MoveRange(STATUS* status, int x, int y, int moveRange);
	// 攻撃範囲描画
	void AttackRange();
	// 移動方向描画
	void DrawMoveArrow(STATUS status, int x, int y, int moveValue);
	// キャラクターの移動
	bool CharacterMove(STATUS* status, int moveX, int moveY);

	// 攻撃チェック
	void AttackCheck(STATUS* status);
	// 攻撃範囲描画
	void AttackableDraw(STATUS* status);
	// 攻撃の詳細情報表示
	void GetAttackDetail(STATUS* myStatus, STATUS* eStatus);
	// 攻撃の計算
	void CharacterAttack(STATUS* myStatus, STATUS* eStatus, int count);
	// 攻撃の処理
	void CharacterDamage(STATUS* myStatus, STATUS* eStatus, double probability, int damage);

	// カメラとのオフセット計算
	void SetCameraOffset(STATUS* status, int dir, bool horizontal);

	// 移動エリアのリセット
	void MoveAreaClear();
};