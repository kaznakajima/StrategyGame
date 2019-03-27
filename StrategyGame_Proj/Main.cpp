#include "KeyInput.h"
#include "StageCreate.h"
#include "BaseScene.h"

// ���̏�����
void Init()
{

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

	// �V�[���Ǘ��N���X�̃C���X�^���X
	unique_ptr<Scene> c_Scene = make_unique<Scene>();

	StageCreate::Instance()->Open(STAGE_FILE_1);

	while (ProcessMessage() == 0 && ClearDrawScreen() == 0 && KeyInput::Instance()->UpdateInput() == 0)
	{
		c_Scene->Update(); 
		
		ScreenFlip();
	}

	// �I��
	DxLib_End();

	return 0;
}