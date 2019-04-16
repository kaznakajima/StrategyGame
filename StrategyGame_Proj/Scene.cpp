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

// �V�[���ύX (�����@�ύX��̃V�[��)
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
	c_Scene->Initialize();
}

void Scene::ChangeFadeState(FADE_STATE state, CURRENT_SCENE scene)
{
	fadeState = state;
	nextScene = scene;
}

// �X�V
void Scene::Update() {
	// ���݂̃V�[���̍X�V
	c_Scene->Update();
}

// �`��
void Scene::Draw() {
	// ���݂̃V�[���̕`��
	c_Scene->Draw();
}

// �V�[���t�F�[�h
void Scene::SceneFade(int _alpha, int fadeValue)
{
	ClearDrawScreen();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �t�F�[�h�C���J�n
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);
	DrawGraph(0, 0, fadeImg, true);
	// �����ɉ��ʂ��t�F�[�h
	AudioManager::Instance()->VolumeFade(_alpha);

	// �ڕW�̃A���t�@�l�ɒB�����ꍇ
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