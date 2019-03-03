#pragma once
#include "CharacterManager.h"
#include "Character.h"

// 敵AIを管理するクラス
class AIManager
{
public:
	AIManager();

	// AIを動かすかどうか
	bool AIMove;

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 現在の敵(AI)、プレイヤーのカウント
	void CharacterCount(Character::STATUS status);

	// 初回起動
	void Play();

	// 動かすキャラクターの選択
	void MoveSelect(Character::STATUS* status);

	// 移動先を選択する
	void ChoiseMovePoint();

	// プレイヤー側キャラクターとの距離を取得
	int GetDistance(Character::STATUS* status, vector<Character::STATUS> playerList);
private:
	// 移動先の座標
	int x = 0, y = 0;

	// CharacterManagerのインスタンス
	CharacterManager* characterMgr;

	// 敵(AI)キャラクターのリスト
	vector<Character::STATUS> enemyList;
	// プレイヤー側のキャラクターのリスト
	vector<Character::STATUS> playerList;

	// 操作するキャラクターのインスタンス
	Character::STATUS* myStatus;

	// 敵キャラクターのロスト(死亡処理)
	void CharacterLost(Character::STATUS* status);
};