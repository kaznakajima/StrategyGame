#include "DxLib.h"
#include "TimeCount.h"

// ���݂̎��Ԃ��Z�b�g
void TimeCount::SetCount()
{
	isCount = true;
	// ���݂̎��Ԃ��擾
	time = GetNowCount();
}

// �o�ߎ��Ԃ̎擾
float TimeCount::GetTimer(float interval)
{
	if (isCount == false) return 0;

	// �o�ߎ��Ԃ��擾
	float timer = GetNowCount() - time;

	if (timer >= interval) {
		isCount = false;
		return timer;
	}
	return 0;
}