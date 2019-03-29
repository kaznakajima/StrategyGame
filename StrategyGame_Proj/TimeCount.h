#pragma once
#include "Singleton.h"

class TimeCount : public Singleton<TimeCount> 
{
public:
	// �o�ߎ���
	float time;

	// ���݂̎��Ԃ𒲂ׂ�
	void SetCount();

	// �o�ߎ��Ԃ𒲂ׂ�
	float GetTimer(float interval);
private:

	// �J�E���g�J�n���邩�ǂ���
	bool isCount = false;
};