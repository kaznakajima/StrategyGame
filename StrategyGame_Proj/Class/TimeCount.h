#pragma once
#include "Singleton.h"

// fps
#define FPS (60)
// 秒数
#define SECOND(_sec) ((_sec) * FPS)

class TimeCount : public Singleton<TimeCount> 
{
public:
	// 経過時間
	float time;

	// 現在の時間を調べる
	void SetCount();

	// 経過時間を調べる
	float GetTimer(float interval);
private:

	// カウント開始するかどうか
	bool isCount = false;
};