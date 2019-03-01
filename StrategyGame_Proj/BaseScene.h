#pragma once

#define FADE_IMAGE "Resources\\image\\Fade.png"

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
};

// シーンを管理するクラス
class Scene
{
public:
	Scene();
	~Scene();

	// シーンタイプ
	enum SCENE {
		TITLE,
		SELECT,
		GAME
	};

	// シーン変更
	static void ChangeScene(SCENE scene);
	// シーンフェード
	static void SceneFade(SCENE nextScene,int stageNum = 0);
	// 更新
	static void Update();
	// 描画
	static void Draw();
private:
	// 現在のシーン
	static BaseScene* c_Scene;
};
