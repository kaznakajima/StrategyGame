#pragma once
#include "BaseScene.h"

#define FIELD_IMG "StageMap.png"
#define START_IMG "GameStart.png"
#define TITLE_LOGO "TitleLogo.png"

class TitleScene : public BaseScene
{
public:
	// ������
	void Initialize();
	// �I��
	void Finalize();
	// �t�@�C���ǂݍ���
	void LoadFile();
	//�X�V
	void Update();
	//�`��
	void Draw();
private:
	// �v���C���[�^�C�v
	enum TITLESTATE {
		START,
		END
	};
	TITLESTATE type;
	int _type = 0;

	// ���W
	int xPos = 60;
};
