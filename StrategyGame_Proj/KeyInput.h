#pragma once
#include "CharacterManager.h"
#include "StageCreate.h"

// �L�[���̓N���X
class KeyInput : public Singleton<KeyInput>
{
public:
	KeyInput();

	// �L�[��������Ă���t���[�������擾
	int Key[256];
	// x���W, y���W
	int xPos, yPos;
	// �^���I�J�����ʒu
	VECTOR cameraPos;
	// �I�����
	bool isSelect = false;

	// �L�[���͂̍X�V
	int UpdateInput();
	// �ړ��ʌv�Z
	void InputCalc(CharacterManager* characterMgr);
};