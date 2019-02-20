#include "DxLib.h"
#include "BaseScene.h"

BaseScene* Scene::c_Scene;

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
	// 古いシーンの削除
	if (c_Scene != NULL) {
		delete c_Scene;
	}

	switch (scene) {
	case SCENE::TITLE:
		break;
	case SCENE::SELECT:
		break;
	case SCENE::GAME:
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
void Scene::SceneFade(SCENE nextScene, int stageNum)
{
	// フェード用変数(透明度,フェード用画像)
	int alpha;

	SetDrawScreen(DX_SCREEN_BACK);

	// フェードアウト
	for (alpha = 0; alpha < 255; alpha += 3)
	{
		ClearDrawScreen();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		Draw();

		// フェードイン開始
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		//DrawGraph(0, 0, FileManager::Instance()->GetFileHandle(FADE_IMAGE), TRUE);
		// 同時に音量もフェード
		//AudioManager::Instance()->VolumeFade(alpha);
		ScreenFlip();
	}

	// 次のシーンの初期化
	c_Scene->UnLoadFile();
	ChangeScene(nextScene);

	// GAMEシーンならマップ読み込み
	switch (nextScene)
	{
	case SCENE::GAME:
		switch (stageNum)
		{
			// ステージ1を読み込む
		case 0:
			break;
			// ステージ2を読み込む
		case 1:
			break;
		}
		break;
	default:
		break;
	}

	// フェードイン
	for (alpha = 255; alpha > 0; alpha -= 3)
	{
		ClearDrawScreen();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		Draw();

		// フェードアウト開始
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		//DrawGraph(0, 0, FileManager::Instance()->GetFileHandle(FADE_IMAGE), TRUE);
		// 同時に音量もフェード
		//AudioManager::Instance()->VolumeFade(alpha);
		ScreenFlip();
	}

	// 描画のブレンドを切る
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	ScreenFlip();
}