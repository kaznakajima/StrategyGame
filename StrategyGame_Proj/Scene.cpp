#include "DxLib.h"
#include "BaseScene.h"
#include "GameScene.h"

BaseScene* Scene::c_Scene;

Scene::Scene()
{
	ChangeScene(Scene::GAME);
}

Scene::~Scene()
{
	
}

// �V�[���ύX (�����@�ύX��̃V�[��)
void Scene::ChangeScene(SCENE scene)
{
	// �Â��V�[���̍폜
	if (c_Scene != NULL) {
		delete c_Scene;
	}

	switch (scene) {
	case SCENE::TITLE:
		break;
	case SCENE::SELECT:
		break;
	case SCENE::GAME:
		c_Scene = new GameScene();
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
void Scene::SceneFade(SCENE nextScene, int stageNum)
{
	// �t�F�[�h�p�ϐ�(�����x,�t�F�[�h�p�摜)
	int alpha, fadeImg = LoadGraph(FADE_IMAGE);

	SetDrawScreen(DX_SCREEN_BACK);

	// �t�F�[�h�A�E�g
	for (alpha = 0; alpha < 255; alpha += 3)
	{
		ClearDrawScreen();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		Draw();

		// �t�F�[�h�C���J�n
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawGraph(0, 0, fadeImg, TRUE);
		// �����ɉ��ʂ��t�F�[�h
		//AudioManager::Instance()->VolumeFade(alpha);
		ScreenFlip();
	}

	// ���̃V�[���̏�����
	ChangeScene(nextScene);

	// �t�F�[�h�C��
	for (alpha = 255; alpha > 0; alpha -= 3)
	{
		ClearDrawScreen();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		Draw();

		// �t�F�[�h�A�E�g�J�n
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawGraph(0, 0, fadeImg, TRUE);
		// �����ɉ��ʂ��t�F�[�h
		//AudioManager::Instance()->VolumeFade(alpha);
		ScreenFlip();
	}

	// �`��̃u�����h��؂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	ScreenFlip();
}