#pragma once
#include "Singleton.h"

// fps
#define FPS (60)
// �b��
#define SECOND(_sec) ((_sec) * FPS)

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