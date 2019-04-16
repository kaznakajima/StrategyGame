#pragma once
#include "BaseScene.h"
#define FIELD_IMG "StageMap.png"
#define START_TEXT "GameStart.png"
#define CLOSE_TEXT "GameClose.png"
#define CHOISE_IMG "Choise.png"

class TitleScene : public BaseScene
{
public:
	// ������
	void Initialize();
	// �I��
	void Finalize();
	// �t�@�C���ǂݍ���
	void LoadFile();
	// �ǂݍ��񂾃t�@�C���̔j��
	void UnLoadFile();
	//�X�V
	void Update();
	//�`��
	void Draw();
private:
	// �v���C���[�^�C�v
	enum PLAYERTYPE {
		PLAYER = 0,
		ENEMY
	};
	PLAYERTYPE type;
	int _type = 0;

	// ���W
	int xPos;

	// �v���C���[���ƓG�̐�
	int playerCount = 2, enemyCount = 2;
};
