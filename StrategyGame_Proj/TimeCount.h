#pragma once
#include "Singleton.h"

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