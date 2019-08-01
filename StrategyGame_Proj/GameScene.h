#pragma once
#include "BaseScene.h"
#include "DrawParts.h"

#define FIELD_IMG "StageMap.png"
#define CURSOR_IMG "Cursor.png"
#define PLAYERTURN_IMG "PlayerTurn.png"
#define ENEMYTURN_IMG "EnemyTurn.png"
#define CLEAR_IMG "GameClear.png"
#define GAMEOVER_IMG "GameOver.png"

class GameScene : public BaseScene
{
public:
	// �R���X�g���N�^
	GameScene();

	enum GAMESTATE {
		TITLE,
		END
	};
	GAMESTATE type;

	// ������
	void Initialize();
	// �I��
	void Finalize();
	// �t�@�C���ǂݍ���
	void LoadFile();
	// �^�[���ύX
	void TurnChange(bool playerTurn);
	//�X�V
	void Update();
	//�`��
	void Draw();


private:
	// �L�[���͏���
	void KeyEvent();
	// �Q�[���I��
	void GameEnd(bool isClear);

	// �A�j���[�V�����p�ϐ�
	float moveX = 672;
	float moveY = -48; 

	// �K�v�ȑf��
	shared_ptr<Renderer> fieldImg;
	shared_ptr<Renderer> cursorImg;
	shared_ptr<Renderer> turnImg;

	// ���݂̍��W
	int xPos = 480, yPos = 240;
};