#pragma once
#include <fstream>
#include <string>
#include <memory>
using namespace std;

#define IRON_SWORD "Resources\\WeaponData\\�S�̌�.dat"
#define IRON_SPEAR "Resources\\WeaponData\\�S�̑�.dat"
#define IRON_AX "Resources\\WeaponData\\�S�̕�.dat"

class Weapon
{
	// ����̎��
	enum WEAPON_TYPE
	{
		SWORD = 0,  // ��
		SPEAR,          // ��
		AX,               // ��
	};

public:
	Weapon();

	// ����̃p�����[�^
	typedef struct
	{
		int WeaponType;                    // ����̎��
		int MAX_USE;                        // �ő�g�p��
		int RANGE;                            // �˒�
		int WEIGHT;                          // �d��
		int POWER;                           // �З�
		int HIT;                                 // ����
		int CLT;                                 // �K�E
	} WEAPON_PARAM;

	// �g�p��
	int use;

	// ���g�̃p�����[�^
	WEAPON_PARAM myParam;

	// �p�����[�^�̏�����
	void ParamInitialize(string w_Name);
};