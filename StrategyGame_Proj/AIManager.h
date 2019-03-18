#pragma once
#include "CharacterManager.h"
#include "Character.h"

// 敵AIを管理するクラス
class AIManager : public Singleton<AIManager>
{
public:
	AIManager();

	// 移動先の座標
	int x = 0, y = 0; 
	int xPos = 0, yPos = 0;

	// AIを動かすかどうか
	bool AIMove;

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 現在の敵(AI)、プレイヤーのカウント
	void CharacterCount(Character* character);

	// 初回起動
	void Play();

	// 動かすキャラクターの選択
	void MoveSelect(Character* character);

	// 移動先を選択する
	void ChoiseMovePoint(int xPos, int yPos);

	// プレイヤー側キャラクターとの距離を取得
	int GetDistance(Character* character, vector<Character*> playerList);
private:

	// CharacterManagerのインスタンス
	CharacterManager* characterMgr;

	// 敵(AI)キャラクターのリスト
	vector<Character*> enemyList;
	// プレイヤー側のキャラクターのリスト
	vector<Character*> playerList;

	// 操作するキャラクターのインスタンス
	Character* myCharacter;

	// 敵キャラクターのロスト(死亡処理)
	void CharacterLost(Character* character);
};