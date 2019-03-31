#pragma once

#define FADE_IMAGE "Resources\\image\\Fade.png"

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
class Scene
{
public:
	Scene();
	~Scene();

	// �V�[���^�C�v
	enum SCENE {
		TITLE,
		SELECT,
		GAME
	};

	// �V�[���ύX
	void ChangeScene(SCENE scene);
	// �V�[���t�F�[�h
	void SceneFade(SCENE nextScene,int stageNum = 0);
	// �X�V
	void Update();
	// �`��
	void Draw();
	// �I��
	void Finalize();
private:
};
