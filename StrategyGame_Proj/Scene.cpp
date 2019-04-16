#include "DxLib.h"
#include "BaseScene.h"
#include "GameScene.h"
#include "TitleScene.h"
#include <memory>
using namespace std;

Scene::Scene()
{
	fadeState = Scene::FADEIN;
	nextScene = CURRENT_SCENE::TITLE;
	ChangeScene(nextScene);
	fadeImg = FileManager::Instance()->GetFileHandle(FADE_IMAGE);
}

Scene::~Scene()
{

}

// シーン変更 (引数　変更先のシーン)
void Scene::ChangeScene(CURRENT_SCENE scene)
{
	switch (scene) {
	case CURRENT_SCENE::TITLE:
		fadeState = Scene::FADEIN;
		c_Scene = make_unique<TitleScene>();
		break;
	case CURRENT_SCENE::GAME:
		fadeState = Scene::FADEIN;
		c_Scene = make_unique<GameScene>();
		break;
	}
}

void Scene::ChangeFadeState(FADE_STATE state, CURRENT_SCENE scene)
{
	fadeState = state;
	nextScene = scene;
}

// 更新
void Scene::Update() {
	// 現在のシーンの更新
	c_Scene->Update();
}

// 描画
void Scene::Draw() {
	// 現在のシーンの描画
	c_Scene->Draw();
}

// シーンフェード (引数　変更先のシーン)
void Scene::SceneFade(CURRENT_SCENE nextScene)
{
	// フェード用変数(透明度,フェード用画像)
	int fadeImg = FileManager::Instance()->GetFileHandle(FADE_IMAGE);

	SetDrawScreen(DX_SCREEN_BACK);

	for (int alpha = 0; alpha < 256; alpha += 3) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		ClearDrawScreen();

		// フェードイン開始
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 122);
		DrawGraph(0, 0, fadeImg, true);
		// 同時に音量もフェード
		AudioManager::Instance()->VolumeFade(alpha);
		ScreenFlip();
	}

	// 次のシーンの初期化
	ChangeScene(nextScene);

	for (int alpha = 255; alpha > -1; alpha -= 3) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		ClearDrawScreen();

		// フェードイン開始
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 122);
		DrawGraph(0, 0, fadeImg, true);
		// 同時に音量もフェード
		AudioManager::Instance()->VolumeFade(alpha);
		ScreenFlip();
	}

	// 描画のブレンドを切る
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	ScreenFlip();
}

void Scene::SceneFade(int _alpha, int fadeValue)
{
	ClearDrawScreen();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// フェードイン開始
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);
	DrawGraph(0, 0, fadeImg, true);
	// 同時に音量もフェード
	AudioManager::Instance()->VolumeFade(_alpha);

	// 目標のアルファ値に達した場合
	if (_alpha == fadeValue) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		if(_alpha != 0) fadeState = Scene::NONE;
		else ChangeScene(nextScene);
	}
}

void Scene::Finalize()
{
	c_Scene->Finalize();
}