#pragma once
#include "AudioManager.h"
#include "KeyInput.h"
#include "FileManager.h"
#include "TimeCount.h"

#define FADE_IMAGE "Fade.png"

// シーンのもととなるクラス(継承させる)
class BaseScene
{
public:
	// 読み込み
	virtual void LoadFile() = 0;
	// 読み込んだファイルの破棄
	virtual void UnLoadFile() = 0;
	// 初期化
	virtual void Initialize() = 0; 
	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Draw() = 0;
	// 終了
	virtual void Finalize() = 0;
};

// シーンを管理するクラス
class Scene : public Singleton<Scene>
{
public:
	Scene();
	~Scene();

	// シーンタイプ
	enum CURRENT_SCENE {
		TITLE,
		GAME
	};
	// フェード状態のステート
	enum FADE_STATE {
		FADEIN,
		FADEOUT,
		NONE
	};
	FADE_STATE fadeState;

	// 現在のシーン
	unique_ptr<BaseScene> c_Scene;
	// 次のシーン
	CURRENT_SCENE nextScene;
	// フェード用変数(透明度,フェード用画像)
	int fadeImg;
	// シーン変更
	void ChangeScene(CURRENT_SCENE scene);
	// フェードステートの変更
	void ChangeFadeState(FADE_STATE state, CURRENT_SCENE scene);
	// シーンフェード(現在のアルファ値, 目標のアルファ値)
	void SceneFade(int _alpha, int fadeValue);
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了
	void Finalize();
};
