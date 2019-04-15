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
	enum SCENE {
		TITLE,
		GAME
	};

	// �V�[���ύX
	void ChangeScene(SCENE scene);
	// �V�[���t�F�[�h
	void SceneFade(SCENE nextScene);
	// �X�V
	void Update();
	// �`��
	void Draw();
	// �I��
	void Finalize();
};
