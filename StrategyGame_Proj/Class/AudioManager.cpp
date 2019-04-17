#include "AudioManager.h"
#include "FileManager.h"

AudioManager::AudioManager(void)
{
	bgm = BGM_ENEMY;
}
AudioManager::~AudioManager(void)
{

}

// サウンド関連クラスのメンバ関数の記述
// AudioManagerクラスのメンバ変数の実態
string AudioManager::bgm;

// SEの再生
void AudioManager::playSE(string pass) 
{
	// 引数に対応したSEを再生
	PlaySoundMem(FileManager::Instance()->GetFileHandle(pass), DX_PLAYTYPE_BACK);
}

// SEの停止
void AudioManager::stopSound(string pass) 
{
	// 引数に対応したSEを停止
	StopSoundMem(FileManager::Instance()->GetFileHandle(pass));
}

// BGMの再生
void AudioManager::playBGM(string pass)
{
	// 再生するサウンドを取得
	string newbgm = pass;
	// 現在のBGMと同じならそのまま
	if (bgm == newbgm) {
		return;
	}
	// BGMの停止
	StopSoundMem(FileManager::Instance()->GetFileHandle(bgm));
	bgm = newbgm;
	// BGMはループ再生
	PlaySoundMem(FileManager::Instance()->GetFileHandle(bgm), DX_PLAYTYPE_LOOP);
}

// 音量のフェード
void AudioManager::VolumeFade(int volume)
{
	// 引数によって音量調節
	ChangeVolumeSoundMem(255 - volume, FileManager::Instance()->GetFileHandle(bgm));
}