#include "DxLib.h"
#include "BaseScene.h"
#include "GameScene.h"
#include "TitleScene.h"
#include <memory>
using namespace std;

// 現在のシーン
unique_ptr<BaseScene> c_Scene;

Scene::Scene()
{
	ChangeScene(Scene::TITLE);
}

Scene::~Scene()
{

}

// シーン変更 (引数　変更先のシーン)
void Scene::ChangeScene(SCENE scene)
{
	switch (scene) {
	case SCENE::TITLE:
		c_Scene = make_unique<TitleScene>();
		break;
	case SCENE::GAME:
		c_Scene = make_unique<GameScene>();
		break;
	}
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
void Scene::SceneFade(SCENE nextScene)
{
	// フェード用変数(透明度,フェード用画像)
	int fadeImg = FileManager::Instance()->GetFileHandle(FADE_IMAGE);

	SetDrawScreen(DX_SCREEN_BACK);

	for (int alpha = 0; alpha < 256; alpha += 3) {
		ClearDrawScreen();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// フェードイン開始
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawGraph(0, 0, fadeImg, TRUE);
		// 同時に音量もフェード
		AudioManager::Instance()->VolumeFade(alpha);
		ScreenFlip();
	}

	// 次のシーンの初期化
	ChangeScene(nextScene);

	for (int alpha = 255; alpha > -1; alpha -= 3) {
		ClearDrawScreen();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// フェードイン開始
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawGraph(0, 0, fadeImg, TRUE);
		// 同時に音量もフェード
		AudioManager::Instance()->VolumeFade(alpha);
		ScreenFlip();
	}

	// 描画のブレンドを切る
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	ScreenFlip();
}

void Scene::Finalize()
{
	c_Scene->Finalize();
}