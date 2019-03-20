#pragma once
#include "BaseScene.h"

// �X�e�[�W�摜�ƃJ�[�\���摜
#define FIELD_IMG "Resources\\image\\Field.png"
#define CURSOR_IMG "Resources\\image\\Cursor.png"
#define PLAYERTURN_IMG "Resources\\image\\TurnStart_P.png"
#define ENEMYTURN_IMG "Resources\\image\\TurnStart_E.png"

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
	// �^�[���ύX
	void TurnChange(bool playerTurn);
	//�X�V
	void Update();
	//�`��
	void Draw();

private:
	// �X�e�[�W�摜
	int stageImg;
	// �J�[�\���摜
	int cursorImg;
	// �^�[���ύX�p�摜
	int turnChangeImg = 0;
	// �A�j���[�V�����p�ϐ�
	float moveX = 720;

	// ���݂̍��W
	int xPos = 480, yPos = 240;
};