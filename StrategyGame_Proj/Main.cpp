#include "KeyInput.h"
#include "StageCreate.h"

// �X�e�[�W�摜
int Handle;
// �J�[�\���摜
int Cursor;

CharacterManager* characterMgr;
KeyInput* _keyInput;

// ���j�b�g�̑I�����
bool isSelect;

// ���W
int xPos = 480, yPos = 240;

// ���̏�����
void Init()
{
	// �L�����N�^�[�}�l�[�W���[�̃C���X�^���X
	characterMgr = new CharacterManager();

	_keyInput = KeyInput::Instance();

	Handle = LoadGraph("Field.png");
	Cursor = LoadGraph("Cursor.png");
}

// �`�惁�\�b�h
void gpDraw()
{
	// ���W�X�V
	xPos = _keyInput->xPos;
	yPos = _keyInput->yPos;

	// �`��
	DrawGraph(0 - _keyInput->cameraPos.x, 0 - _keyInput->cameraPos.y, Handle, true);

	if (_keyInput->Key[KEY_INPUT_SPACE] == 1) {
		characterMgr->KeyCheck(xPos, yPos);

		if (isSelect == false) characterMgr->DrawCheck(xPos, yPos);

		isSelect = characterMgr->isSelect;
		_keyInput->isSelect = isSelect;
	}

	if (isSelect == true && characterMgr->attack == false) {
		characterMgr->Draw();
		characterMgr->GetMoveArrow(xPos, yPos);
	}

	DrawGraph(xPos, yPos, Cursor, true);
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

	characterMgr->Initialize();

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && _keyInput->UpdateInput() == 0)
	{
		// �ړ��ʌv�Z
		_keyInput->InputCalc(characterMgr);

		// �`��
		gpDraw();

		characterMgr->GetAttackArea(xPos, yPos);

		characterMgr->Update();

		if (isSelect == false) characterMgr->CharacterMove(xPos, yPos);

		if (characterMgr->attack) characterMgr->Attack();
	}

	// �I��
	DxLib_End();

	return 0;
}