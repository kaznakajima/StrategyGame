#include "Weapon.h"

Weapon::Weapon()
{

}

// パラメータの初期化
void Weapon::ParamInitialize(string w_Name)
{
	fstream file;
	file.open(w_Name, ios::in | ios::binary);
	file.read((char*)&myParam, sizeof(myParam));

	//use = myParam->MAX_USE;

	file.close();
}