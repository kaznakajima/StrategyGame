#pragma once
#include "CharacterManager.h"
#include "Character.h"

// 敵AIを管理するクラス
class AIManager : public Singleton<AIManager>
{
public:
	AIManager();
	~AIManager();

	// 移動先の座標
	int x = 0, y = 0; 
	int xPos = 0, yPos = 0;

	// AIを動かすかどうか
	bool AIMove;

	bool isDebug = false;

	// 初期化
	void Initialize();
	// 終了
	void Finalize();

	// 更新
	void Update();

	// 現在の敵(AI)、プレイヤーのカウント
	void CharacterCount(Character* character);

	// 初回起動
	void Play();
private:

	// 敵(AI)キャラクターのリスト
	vector<Character*> enemyList;
	// プレイヤー側のキャラクターのリスト
	vector<Character*> playerList;

	// 操作するキャラクターのインスタンス
	Character* myCharacter;

	// 移動判定
	bool isMove;
	int moveToX = 0, moveToY = 0;
	// 移動最大値格納
	int moveCost;
	// プレイヤーとの最短距離
	int minDistance = 100;
	
	// 動かすキャラクターの選択
	void MoveSelect(Character* character);

	// 移動先を選択する
	void ChoiseMovePoint(int _x, int _y);

	// プレイヤー側キャラクターとの距離を取得
	int GetDistancePlayer(Character* character, vector<Character*> playerList);

	// 移動地点の取得
	void GetMovePoint(Character* character, int _x, int _y, vector<Character*> playerList);

	// 移動地点がプレイヤーに向かえる最短距離か検索
	void CheckCanMove(Character* character, int _x, int _y, Character* playerSt);

	// 敵キャラクターのロスト(死亡処理)
	void CharacterLost(Character* character);
};