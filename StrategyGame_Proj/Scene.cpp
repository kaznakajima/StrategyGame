#include "DxLib.h"
#include "BaseScene.h"
#include "GameScene.h"
#include "TitleScene.h"
#include <memory>
using namespace std;

// ���݂̃V�[��
unique_ptr<BaseScene> c_Scene;

Scene::Scene()
{
	ChangeScene(Scene::TITLE);
}

Scene::~Scene()
{

}

// �V�[���ύX (�����@�ύX��̃V�[��)
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

// �V�[���t�F�[�h (�����@�ύX��̃V�[��)
void Scene::SceneFade(SCENE nextScene)
{
	// �t�F�[�h�p�ϐ�(�����x,�t�F�[�h�p�摜)
	int fadeImg = FileManager::Instance()->GetFileHandle(FADE_IMAGE);

	SetDrawScreen(DX_SCREEN_BACK);

	for (int alpha = 0; alpha < 256; alpha += 3) {
		ClearDrawScreen();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �t�F�[�h�C���J�n
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawGraph(0, 0, fadeImg, TRUE);
		// �����ɉ��ʂ��t�F�[�h
		AudioManager::Instance()->VolumeFade(alpha);
		ScreenFlip();
	}

	// ���̃V�[���̏�����
	ChangeScene(nextScene);

	for (int alpha = 255; alpha > -1; alpha -= 3) {
		ClearDrawScreen();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �t�F�[�h�C���J�n
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawGraph(0, 0, fadeImg, TRUE);
		// �����ɉ��ʂ��t�F�[�h
		AudioManager::Instance()->VolumeFade(alpha);
		ScreenFlip();
	}

	// �`��̃u�����h��؂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	ScreenFlip();
}

void Scene::Finalize()
{
	c_Scene->Finalize();
}