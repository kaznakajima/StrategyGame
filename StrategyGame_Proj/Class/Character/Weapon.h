#pragma once
#include <fstream>
#include <string>
#include <memory>
using namespace std;

#define IRON_SWORD "Resources\\WeaponData\\鉄の剣.dat"
#define IRON_SPEAR "Resources\\WeaponData\\鉄の槍.dat"
#define IRON_AX "Resources\\WeaponData\\鉄の斧.dat"

class Weapon
{
	// 武器の種類
	enum WEAPON_TYPE
	{
		SWORD = 0,  // 剣
		SPEAR,          // 槍
		AX,               // 斧
	};

public:
	Weapon();

	// 武器のパラメータ
	typedef struct
	{
		int WeaponType;                    // 武器の種類
		int MAX_USE;                        // 最大使用数
		int RANGE;                            // 射程
		int WEIGHT;                          // 重さ
		int POWER;                           // 威力
		int HIT;                                 // 命中
		int CLT;                                 // 必殺
	} WEAPON_PARAM;

	// 使用回数
	int use;

	// 自身のパラメータ
	WEAPON_PARAM myParam;

	// パラメータの初期化
	void ParamInitialize(string w_Name);
};