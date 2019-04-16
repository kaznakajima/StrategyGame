#include "KeyInput.h"
#include "StageCreate.h"
#include "BaseScene.h"
#include "FileManager.h"

// ���̏�����
void Init()
{
	FileManager::Instance()->Initialize();
}

// �A���t�@�l��Ԃ�
int FadeAlpha(int _alpha)
{
	switch (Scene::Instance()->fadeState) {
	case Scene::FADEIN:
		_alpha += 3;
		if (_alpha > 255) _alpha = 255;
		Scene::Instance()->SceneFade(_alpha, 255);
		break;
	case Scene::FADEOUT:
		_alpha -= 3;
		if (_alpha < 0) _alpha = 0;
		Scene::Instance()->SceneFade(_alpha, 0);
		break;
	default:
		break;
	}

	return _alpha;
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

	// �t�F�[�h�p�A���t�@�l
	int alpha = 0;

	while (ProcessMessage() == 0 && ClearDrawScreen() == 0 && KeyInput::Instance()->UpdateInput() == 0)
	{
		// �t�F�[�h��̃A���t�@�l�̎擾
		if(Scene::Instance()->fadeState != Scene::NONE) alpha = FadeAlpha(alpha);

		// �d�Ȃ�h�~
		if (alpha != 0) {
			Scene::Instance()->Draw();

			Scene::Instance()->Update();
		}
		ScreenFlip();
	}

	Scene::Instance()->Finalize();

	// �I��
	DxLib_End();

	return 0;
}