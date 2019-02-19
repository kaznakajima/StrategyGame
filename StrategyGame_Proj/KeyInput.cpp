#include "DxLib.h"
#include "KeyInput.h"

KeyInput::KeyInput()
{
	// �^���I�J�����̏�����
	xPos = 480; yPos = 240;
	cameraPos.x = 0; cameraPos.y = 0; cameraPos.z = -10.0f;
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

// �ړ��ʌv�Z
void KeyInput::InputCalc(CharacterManager* characterMgr)
{
	if (characterMgr->isMove) return;

	// �����ꂽ�L�[���͂ɂ���ď��������s
	// �E�L�[�������ꂽ��
	if (Key[KEY_INPUT_RIGHT] == 1) {

		// �E�Ɉړ�
		xPos += CHIP_SIZE;

		// ��ʒ[����o�Ȃ��悤�ɐ���
		if (xPos >= SCREEN_WIDTH - CHIP_SIZE) xPos = SCREEN_WIDTH - CHIP_SIZE;

		if (xPos > CHIP_SIZE * 10 && cameraPos.x + SCREEN_WIDTH < STAGE1_WIDTH) {
			cameraPos.x += CHIP_SIZE;
			characterMgr->SetCameraOffset(-1, true);
		}

		if (isSelect) characterMgr->GetMoveCount(xPos, yPos);
	}
	// ���L�[�������ꂽ��
	else if (Key[KEY_INPUT_DOWN] == 1) {

		// ���Ɉړ�
		yPos += CHIP_SIZE;

		if (yPos >= SCREEN_HEIGHT - CHIP_SIZE) yPos = SCREEN_HEIGHT - CHIP_SIZE;

		if (isSelect) characterMgr->GetMoveCount(xPos, yPos);
	}
	// ���L�[�������ꂽ��
	else if (Key[KEY_INPUT_LEFT] == 1) {

		// ���Ɉړ�
		xPos -= CHIP_SIZE;

		if (xPos <= 0) xPos = 0;

		if (xPos < CHIP_SIZE * 3 && cameraPos.x > 0) {
			cameraPos.x -= CHIP_SIZE;
			characterMgr->SetCameraOffset(1, true);
		}

		if (isSelect) characterMgr->GetMoveCount(xPos, yPos);
	}
	// ��L�[�������ꂽ��
	else if (Key[KEY_INPUT_UP] == 1) {

		// ��Ɉړ�
		yPos -= CHIP_SIZE;

		if (yPos <= 0) yPos = 0;

		if (isSelect) characterMgr->GetMoveCount(xPos, yPos);
	}
}