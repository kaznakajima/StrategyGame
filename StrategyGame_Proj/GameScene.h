#pragma once
#include "BaseScene.h"

// �X�e�[�W�摜�ƃJ�[�\���摜
#define FIELD_IMG "Resources\\image\\Field.png"
#define CURSOR_IMG "Resources\\image\\Cursor.png"

class GameScene : public BaseScene
{
public:
	// �R���X�g���N�^
	GameScene();

	// ������
	void Initialize();
	// �t�@�C���ǂݍ���
	void LoadFile();
	// �ǂݍ��񂾃t�@�C���̔j��
	void UnLoadFile();
	//�X�V
	void Update();
	//�`��
	void Draw();

private:
	// �I�����
	bool isSelect = false;
	// �X�e�[�W�摜
	int stageImg;
	// �J�[�\���摜
	int cursorImg;

	// ���݂̍��W
	int xPos = 480, yPos = 240;
};