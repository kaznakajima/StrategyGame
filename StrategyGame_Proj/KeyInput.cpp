#include "DxLib.h"
#include "KeyInput.h"

KeyInput::KeyInput()
{
	Initialize();
}

void KeyInput::Initialize()
{
	xPos = 480; yPos = 240;
	// �^���I�J�����̏�����
	cameraPos.x = 48; cameraPos.y = 0; cameraPos.z = -10.0f;
}

// �L�[���͂̍X�V
int KeyInput::UpdateInput()
{
	// ���݂̃L�[���͂��i�[
	char tmpKey[256];

	// �S�ẴL�[���͏�Ԃ𓾂�
	GetHitKeyStateAll(tmpKey);

	for (int i = 0; i < 256; i++)
	{
		// �L�[�������ꂽ��
		if (tmpKey[i] != 0)
		{
			// ���Z
			Key[i]++;
		}
		else
		{
			// 0�ɂ���
			Key[i] = 0;
		}
	}
	return 0;
}

// ���͂��Ă���L�[��Ԃ�
int KeyInput::Keyboard_Get(int KeyCode)
{
	return Key[KeyCode];
}