#pragma once
#include "AudioManager.h"
#include "KeyInput.h"
#include "FileManager.h"
#include "TimeCount.h"

#define FADE_IMAGE "Fade.png"

// �V�[���̂��ƂƂȂ�N���X(�p��������)
class BaseScene
{
public:
	// �ǂݍ���
	virtual void LoadFile() = 0;
	// �ǂݍ��񂾃t�@�C���̔j��
	virtual void UnLoadFile() = 0;
	// ������
	virtual void Initialize() = 0; 
	// �X�V
	virtual void Update() = 0;
	// �`��
	virtual void Draw() = 0;
	// �I��
	virtual void Finalize() = 0;
};

// �V�[�����Ǘ�����N���X
class Scene : public Singleton<Scene>
{
public:
	Scene();
	~Scene();

	// �V�[���^�C�v
	enum CURRENT_SCENE {
		TITLE,
		GAME
	};
	// �t�F�[�h��Ԃ̃X�e�[�g
	enum FADE_STATE {
		FADEIN,
		FADEOUT,
		NONE
	};
	FADE_STATE fadeState;

	// ���݂̃V�[��
	unique_ptr<BaseScene> c_Scene;
	// ���̃V�[��
	CURRENT_SCENE nextScene;
	// �t�F�[�h�p�ϐ�(�����x,�t�F�[�h�p�摜)
	int fadeImg;
	// �V�[���ύX
	void ChangeScene(CURRENT_SCENE scene);
	// �t�F�[�h�X�e�[�g�̕ύX
	void ChangeFadeState(FADE_STATE state, CURRENT_SCENE scene);
	// �V�[���t�F�[�h(���݂̃A���t�@�l, �ڕW�̃A���t�@�l)
	void SceneFade(int _alpha, int fadeValue);
	// �X�V
	void Update();
	// �`��
	void Draw();
	// �I��
	void Finalize();
};
