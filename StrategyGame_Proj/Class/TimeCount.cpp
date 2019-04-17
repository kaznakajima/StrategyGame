#include "DxLib.h"
#include "TimeCount.h"

// 現在の時間をセット
void TimeCount::SetCount()
{
	isCount = true;
	// 現在の時間を取得
	time = GetNowCount();
}

// 経過時間の取得
float TimeCount::GetTimer(float interval)
{
	if (isCount == false) return 0;

	// 経過時間を取得
	float timer = GetNowCount() - time;

	if (timer >= interval) {
		isCount = false;
		return timer;
	}
	return 0;
}