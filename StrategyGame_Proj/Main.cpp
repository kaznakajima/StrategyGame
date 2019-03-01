#include "KeyInput.h"
#include "StageCreate.h"
#include "BaseScene.h"

// �V�[���Ǘ��N���X�̃C���X�^���X
Scene* c_Scene;

KeyInput* _keyInput;

// ���̏�����
void Init()
{
	_keyInput = KeyInput::Instance();

	c_Scene = new Scene();
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// �E�C���h�E���[�h�ɕύX
	ChangeWindowMode(TRUE);

	// ������
	DxLib_Init();
	SetGraphMode(672, 480, 32);
	SetWindowSize(672, 480);
	SetDrawScreen(DX_SCREEN_BACK);

	Init();

	StageCreate::Instance()->Open(STAGE_FILE_1);

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && _keyInput->UpdateInput() == 0)
	{
		c_Scene->Update();
	}

	// �I��
	DxLib_End();

	return 0;
}