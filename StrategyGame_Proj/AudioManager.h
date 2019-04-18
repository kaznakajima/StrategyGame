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

	// ���݂�BGM
	static string bgm;
	// �Đ�
	void playSE(string typeNum);
	// ��~
	void stopSound(string sNum);
	// BGM�̍Đ�
	void playBGM(string typeNum);
	// ���ʂ̃t�F�[�h
    void VolumeFade(int volume);
};