#pragma once

#include "DxLib.h"
#include "Singleton.h"
#include <vector>
using namespace std;

#define BGM_TITLE "BGM\\Title.mp3"
#define BGM_PLAYER "BGM\\PlayerBGM.mp3"
#define BGM_ENEMY "BGM\\EnemyBGM.mp3"
#define SE_TURNSTART "SE\\TurnStart.mp3"
#define SE_DAMAGE "SE\\Damage.mp3"
#define SE_NODAMAGE "SE\\NoDamage.mp3"
#define SE_MISS "SE\\Miss.mp3"

class AudioManager : public Singleton <AudioManager>
{
public:
	AudioManager();
	~AudioManager();

	// 現在のBGM
	static string bgm;
	// 再生
	void playSE(string typeNum);
	// 停止
	void stopSound(string sNum);
	// BGMの再生
	void playBGM(string typeNum);
	// 音量のフェード
    void VolumeFade(int volume);
};