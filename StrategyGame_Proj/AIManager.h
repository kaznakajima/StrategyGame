#pragma once
#include "Character.h"

// 敵AIを管理するクラス
class AIManager
{
public:
	// 初期化
	void Initialize();

	// 現在の敵(AI)、プレイヤーのカウント
	void CharacterCount(Character::STATUS status);

	// 動かすキャラクターの選択
	void MoveSelect(Character::STATUS* status);

	// 移動先を選択する
	void ChoiseMovePoint();

	// プレイヤー側のキャラクターの取得
	int SerchCharacter(Character::STATUS* status, vector<Character::STATUS> playerList);
private:
	// 敵(AI)キャラクターのリスト
	vector<Character::STATUS> enemyList;
	// プレイヤー側のキャラクターのリスト
	vector<Character::STATUS> playerList;

	// 敵キャラクターのロスト(死亡処理)
	void CharacterLost(Character::STATUS* status);
};