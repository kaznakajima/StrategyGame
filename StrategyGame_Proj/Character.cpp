#include "Character.h"
#include "KeyInput.h"
#include <time.h>

#define PI 3.141592654

// �ړ��͈�
int MoveArea;
// �U���͈�
int AttackArea;
// �U�����
int AttackDetail;
// �ړ��o�H
int ArrowImage[6];

// �ړ��ł���G���A
int moveToPos[MAP_SIZEY][MAP_SIZEX];

// �ړ���̓��؂��ǂ���
bool moveArrow[MAP_SIZEY][MAP_SIZEX];

// �X�e�[�W�f�[�^��ǂݍ��ރN���X�̃C���X�^���X
StageCreate* stage;

// �p�����[�^
int _param[9];

KeyInput* keyInput;

Character::Character()
{
	MoveArea = LoadGraph("CanMove.png");
	AttackArea = LoadGraph("CanAttack.png");
	AttackDetail = LoadGraph("AttackGraph.png");
	LoadDivGraph("Arrow.png", 6, 6, 1, CHIP_SIZE, CHIP_SIZE, ArrowImage);

	// �C���X�^���X�쐬
	stage = StageCreate::Instance();
	keyInput = KeyInput::Instance();

	for (int y = 0; y < MAP_SIZEY; y++) {
		for (int x = 0; x < MAP_SIZEX; x++) {
			moveToPos[y][x] = -1;
			moveArrow[y][x] = false;
			stage->StageUpdate(x, y);
		}
	}
}

 void Character::Character_Initialize(STATUS* status, string pass, int posX, int posY)
{
	 GetCharacterParam(pass);

	 // �X�e�[�^�X�ݒ�
	 status->PosX = posX;
	 status->PosY = posY;
	 status->_PosX = posX;
	 status->_PosY = posY;
	 status->AnimHandle = 0;
	 status->AttackRange = 1;
	 LoadDivGraph("Character_40�~40.png", 20, 4, 5, CHIP_SIZE, CHIP_SIZE, status->Image);

	 // �p�����[�^�ݒ�
	 status->myParam.HP = _param[(int)PLAYER_PARAM::HP];
	 status->myParam.POWER = _param[(int)PLAYER_PARAM::POWER];
	 status->myParam.TECHNIQUE = _param[(int)PLAYER_PARAM::TECHNIQUE];
	 status->myParam.SPEED = _param[(int)PLAYER_PARAM::SPEED];
	 status->myParam.LUCKY = _param[(int)PLAYER_PARAM::LUCKY];
	 status->myParam.DEFENCE = _param[(int)PLAYER_PARAM::DEFENCE];
	 status->myParam.MAGIC_DEFENCE = _param[(int)PLAYER_PARAM::MAGIC_DEFENCE];
	 status->myParam.PHYSIQUE = _param[(int)PLAYER_PARAM::PHYSIQUE];
	 status->myParam.MOVERANGE = _param[(int)PLAYER_PARAM::MOVERANGE];

	 // �K�E�� (�Z�p�����[�^ / 2)
	 status->myParam.ATTACK_CLT = status->myParam.TECHNIQUE / 2;
	 // ��䗦 (�����p�����[�^ * 2 + �K�^�p�����[�^)
	 status->myParam.ATTACK_AVO = status->myParam.SPEED * 2 + status->myParam.LUCKY;
	 // ������ (���햽�� + �Z�p�����[�^ * 2.5)
	 status->myParam.ATTACK_HIT = 85 + (int)(status->myParam.TECHNIQUE * 2.5);
}

 // �p�����[�^�擾
 void Character::GetCharacterParam(string pass)
 {
	 int num = 0;

	 ifstream ifs(pass);
	 string str = "";
	 // �t�@�C���̒��g����s���ǂݎ��
	while (getline(ifs, str)) {
		string tmp = "";
		istringstream stream(str);
		while (getline(stream, tmp, ',')) {
			_param[num] = atoi(tmp.c_str());
			num++;
		}
	}
 }

 // �L�����N�^�[����̕`��
 void Character::SpriteDraw(int x, int y, int img)
 {
	 DrawGraph(x, y, img, true);
 }

// �L�����N�^�[�A�j���[�V����
void Character::CharacterAnim(STATUS* status)
{
	// �I����ԂȂ�A�j���[�V�����X�V
	if (status->isSelect) {
		status->AnimHandle += 0.1f;
		if (status->AnimHandle > 7.0f && status->isMove == false)
			status->AnimHandle = 4.0f;
	}

	// xPos, yPos�̈ʒu�ɃL�����N�^�[��`��
	if (status->isAttack) {
		DrawGraph(status->PosX + (sin(PI * 2 / 120 * Count) + 1) / 2 * CHIP_SIZE, status->PosY, status->Image[(int)status->AnimHandle], true);
		Count++;
		if (Count == 60) {
			Count = 0;
			status->isAttack = false;
		}
	}
	else {
		DrawGraph(status->PosX, status->PosY, status->Image[(int)status->AnimHandle], true);
	}

	// �I�𒆂łȂ����j�b�g�̈ʒu�Ƀ`�F�b�N
	if (status->isSelect == false) stage->onUnit[status->PosY / CHIP_SIZE][status->PosX / CHIP_SIZE] = true;
}

// �L�����N�^�[�̈ړ�
bool Character::CharacterMove(STATUS* status, int moveX, int moveY) 
{
	// �ړ��s�\�G���A��I�������烊�^�[��
	if (stage->checkMove[moveY / CHIP_SIZE][moveX / CHIP_SIZE] == false 
		|| stage->onUnit[moveY / CHIP_SIZE][moveX / CHIP_SIZE] == true 
		|| status->PosX == moveX && status->PosY == moveY) {
		status->isSelect = false;
		status->isMove = false;
		status->AnimHandle = 0;
		AttackCheck(status);
		return false;
	}

	// ���݂̒n�_�̓񎟌��z��p�f�[�^
	int valueX = moveX - status->_PosX;
	int valueY = moveY - status->_PosY;
	// ���ڎw���n�_�̓񎟌��z��p�f�[�^
	int nextX = status->_PosX / CHIP_SIZE;
	int nextY = status->_PosY / CHIP_SIZE;
	
	// �����̂���n�_�̓`�F�b�N
	moveArrow[nextY][nextX] = false;

	// �E�ړ�
	if (moveArrow[nextY][nextX + 1] == true) {
		// �摜�̐؂�ւ�
		if (status->AnimHandle < 8.0f || status->AnimHandle > 11.0f)
			status->AnimHandle = 8.0f;

		status->PosX += 6;

		// ��}�X���̈ړ�
		if (status->PosX - status->_PosX == CHIP_SIZE) {
			status->_PosX = status->PosX;
			moveArrow[nextY][nextX + 1] = false;
		}

		// �ŏI�n�_�ɓ��B������ړ��I��
		if (status->PosX == moveX && status->PosY == moveY) {
			status->isSelect = false;
			status->isMove = false;
			AttackCheck(status);
			status->AnimHandle = 0.0f;
			return false;
		}

		return true;
	}

	// ���ړ�
	if (moveArrow[nextY][nextX - 1] == true) {
		// �摜�̐؂�ւ�
		if (status->AnimHandle < 12.0f || status->AnimHandle > 15.0f)
			status->AnimHandle = 12.0f;

		status->PosX -= 6;

		// ��}�X���̈ړ�
		if (status->PosX - status->_PosX == -CHIP_SIZE) {
			status->_PosX = status->PosX;
			moveArrow[nextY][nextX - 1] = false;
		}

		// �ŏI�n�_�ɓ��B������ړ��I��
		if (status->PosX == moveX && status->PosY == moveY) {
			status->isSelect = false;
			status->isMove = false;
			AttackCheck(status);
			status->AnimHandle = 0.0f;
			return false;
		}

		return true;
	}

	// ���ړ�
	if (moveArrow[nextY + 1][nextX] == true) {
		// �摜�̐؂�ւ�
		if (status->AnimHandle < 4.0f || status->AnimHandle > 7.0f)
			status->AnimHandle = 4.0f;

		status->PosY += 6;

		// ��}�X���̈ړ�
		if (status->PosY - status->_PosY == CHIP_SIZE) {
			status->_PosY = status->PosY;
			moveArrow[nextY + 1][nextX] = false;
		}

		// �ŏI�n�_�ɓ��B������ړ��I��
		if (status->PosX == moveX && status->PosY == moveY) {
			status->isSelect = false;
			status->isMove = false;
			AttackCheck(status);
			status->AnimHandle = 0.0f;
			return false;
		}

		return true;
	}

	// ��ړ�
	if (moveArrow[nextY - 1][nextX] == true) {
		// �摜�̐؂�ւ�
		if (status->AnimHandle < 16.0f || status->AnimHandle > 19.0f)
			status->AnimHandle = 16.0f;

		status->PosY -= 6;

		// ��}�X���̈ړ�
		if (status->PosY - status->_PosY == -CHIP_SIZE) {
			status->_PosY = status->PosY;
			moveArrow[nextY - 1][nextX] = false;
		}

		// �ŏI�n�_�ɓ��B������ړ��I��
		if (status->PosX == moveX && status->PosY == moveY) {
			status->isSelect = false;
			status->isMove = false;
			AttackCheck(status);
			status->AnimHandle = 0.0f;
			return false;
		}

		return true;
	}

	return true;
}

// �ړ��͈͌v�Z
void Character::MoveRange(STATUS* status, int x, int y, int moveCost)
{

	DrawFormatString(48, 48, GetColor(0, 255, 0), "HP [%d]", status->myParam.HP);

	// �J�����Ƃ̍���
	float cameraX = keyInput->cameraPos.x, cameraY = keyInput->cameraPos.y;
	int offsetX = (int)cameraX / CHIP_SIZE, offsetY = (int)cameraY / CHIP_SIZE;

	// ���݂̒n�_�̓񎟌��z��f�[�^�A�N�Z�X�p
	int valueX = x / CHIP_SIZE, valueY = y / CHIP_SIZE;
	// �J�����̍������݂̓񎟌��z��f�[�^�A�N�Z�X�p
	int _valueX = valueX + offsetX, _valueY = valueY + offsetY;

	// ��ʊO�Ȃ烊�^�[��
	if (valueX < 0 || valueX > 14 || valueY < 0 || valueY > 9)
		return;

	// �ړ�������i�[
	moveToPos[valueY][valueX] = moveCost;

	// ���������Ȃ��������͐i�߂Ȃ��ꏊ�̏ꍇ�̓��^�[��
	if (stage->stageList[_valueY][_valueX] > 0 && moveCost >= 0) {
		SpriteDraw(x, y, AttackArea);
		return;
	}
	else if (stage->stageList[_valueY][_valueX] > 0 || moveCost == 0) return;

	// �ړ��͈͕\�����Ă��Ȃ��Ȃ�\��
	if (stage->checkMove[valueY][valueX] != true) {
		stage->checkMove[valueY][valueX] = true;
		SpriteDraw(x, y, MoveArea);
	}

	// ��֍s����Ȃ�`�F�b�N
	if (moveToPos[valueY - 1][valueX] < moveCost) MoveRange(status, x, y - CHIP_SIZE, moveCost - 1);
	// �E�֍s����Ȃ�`�F�b�N
	if (moveToPos[valueY][valueX + 1] < moveCost) MoveRange(status, x + CHIP_SIZE, y, moveCost - 1);
	// ���֍s����Ȃ�`�F�b�N
	if (moveToPos[valueY][valueX - 1] < moveCost)MoveRange(status, x - CHIP_SIZE, y, moveCost - 1);
	// ���֍s����Ȃ�`�F�b�N
	if (moveToPos[valueY + 1][valueX] < moveCost) MoveRange(status, x, y + CHIP_SIZE, moveCost - 1);
}

// �U���͈͕\��
void Character::AttackRange()
{
	for (int y = 0; y < MAP_SIZEY; y++) {
		for (int x = 0; x < MAP_SIZEX; x++) {
			if (moveToPos[y][x] == 0) SpriteDraw(x * CHIP_SIZE, y * CHIP_SIZE, AttackArea);
		}
	}
}

// �K�C�h���C���\��
void Character::DrawMoveArrow(STATUS status, int x, int y, int moveValue)
{
	// ���݂̒n�_�̓񎟌��z��p�f�[�^
	int valueX = x / CHIP_SIZE, valueY = y / CHIP_SIZE;

	// �ړ��͈͂łȂ��Ȃ�\�����Ȃ�
	if (stage->checkMove[valueY][valueX] == false) {
		OldPosX.clear();
		OldPosY.clear();
		moveCount++;
		moveArrow[valueY][valueX] = false;
	}

	// ���͏��ɃK�C�h���C���\��
	if (status.myParam.MOVERANGE - moveToPos[valueY][valueX] >= moveCount || OldPosX.size() == moveCount) {
		for (unsigned int num = 0; num < OldPosX.size(); num++) {
			if (stage->checkMove[OldPosY[num] / CHIP_SIZE][OldPosX[num] / CHIP_SIZE] == true) {
				DrawGraph(OldPosX[num], OldPosY[num], ArrowImage[moveValue], true);
				moveArrow[OldPosY[num] / CHIP_SIZE][OldPosX[num] / CHIP_SIZE] = true;
			}
		}
	}
	// �ŒZ�ōs���郋�[�g��\��
	else if(status.myParam.MOVERANGE - moveToPos[valueY][valueX] < moveCount || OldPosX.size() != moveCount) {

		// ����܂ł̓��͏����N���A
		OldPosX.clear();
		OldPosY.clear();

		if (stage->checkMove[valueY][valueX] == true) {

			// ���j�b�g�̈ʒu�łȂ��Ȃ炱������t�T�m
			if (status.PosX != x || status.PosY != y) {
				DrawGraph(x, y, ArrowImage[moveValue], true);
				moveArrow[y / CHIP_SIZE][x / CHIP_SIZE] = true;
			}

			// ���j�b�g�Ɍ������ă��[�g���t�T�m���Ă���
			if (moveToPos[valueY][valueX] + 1 == moveToPos[valueY + 1][valueX] && stage->checkMove[valueY + 1][valueX] == true) {
				DrawMoveArrow(status, x, y + CHIP_SIZE, 5);
				return;
			}
			if (moveToPos[valueY][valueX] + 1 == moveToPos[valueY - 1][valueX] && stage->checkMove[valueY - 1][valueX] == true) {
				DrawMoveArrow(status, x, y - CHIP_SIZE, 5);
				return;
			}
			if (moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX + 1] && stage->checkMove[valueY][valueX + 1] == true) {
				DrawMoveArrow(status, x + CHIP_SIZE, y, 5);
				return;
			}
			if (moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX - 1] && stage->checkMove[valueY][valueX - 1] == true) {
				DrawMoveArrow(status, x - CHIP_SIZE, y, 5);
				return;
			}
		}
	}
}

// �U���ł��邩�`�F�b�N
void Character::AttackCheck(STATUS* status)
{
	status->canAttack = false;

	int valueX = status->PosX / CHIP_SIZE, valueY = status->PosY / CHIP_SIZE;
	if (stage->onUnit[valueY][valueX + 1] == true) status->canAttack = true;
	if (stage->onUnit[valueY][valueX - 1] == true) status->canAttack = true;
	if (stage->onUnit[valueY + 1][valueX] == true) status->canAttack = true;
	if (stage->onUnit[valueY - 1][valueX] == true) status->canAttack = true;
}

// �U���͈͕`��
void Character::AttackableDraw(STATUS* status)
{
	int valueX = status->PosX / CHIP_SIZE, valueY = status->PosY / CHIP_SIZE;
	if (stage->onUnit[valueY][valueX + 1] == true) SpriteDraw(status->PosX + CHIP_SIZE, status->PosY, AttackArea);
	if (stage->onUnit[valueY][valueX - 1] == true) SpriteDraw(status->PosX - CHIP_SIZE, status->PosY, AttackArea);
	if (stage->onUnit[valueY + 1][valueX] == true) SpriteDraw(status->PosX, status->PosY + CHIP_SIZE, AttackArea);
	if (stage->onUnit[valueY - 1][valueX] == true) SpriteDraw(status->PosX, status->PosY - CHIP_SIZE, AttackArea);
}

// �U���̏ڍו\��
void Character::GetAttackDetail(STATUS* myStatus, STATUS* eStatus)
{
	// �З� (�U�����̗� - ������̎����)
	int mySTR = myStatus->myParam.POWER - eStatus->myParam.DEFENCE;
	if (mySTR < 0) mySTR = 0;
	int eSTR = eStatus->myParam.POWER - myStatus->myParam.DEFENCE;
	if (eSTR < 0) eSTR = 0;

	// ��������(4�ȏ�傫�����2��U��)
	int mySPD = myStatus->myParam.SPEED - eStatus->myParam.SPEED;

	// ������ (�U�����̖����� - ������̉��)
	int myHitness = myStatus->myParam.ATTACK_HIT - eStatus->myParam.ATTACK_AVO;
	int eHitness = eStatus->myParam.ATTACK_HIT - myStatus->myParam.ATTACK_AVO;

	SpriteDraw(0, 0, AttackDetail);
	DrawFormatString(80, 100, GetColor(255, 255, 0), "HP");
	DrawFormatString(80, 125, GetColor(255, 255, 0), "�З�");
	DrawFormatString(80, 150, GetColor(255, 255, 0), "����");

	DrawFormatString(120, 100, GetColor(0, 0, 255), "%d", myStatus->myParam.HP);
	DrawFormatString(120, 125, GetColor(0, 0, 255), "%d", mySTR);
	if (mySPD >= 4) DrawFormatString(130, 130, GetColor(255, 255, 255), "�~2");
	DrawFormatString(120, 150, GetColor(0, 0, 255), "%d", myHitness);

	DrawFormatString(45, 100, GetColor(0, 0, 255), "%d", eStatus->myParam.HP);
	DrawFormatString(45, 125, GetColor(0, 0, 255), "%d", eSTR);
	if (mySPD <= -4) DrawFormatString(55, 130, GetColor(255, 255, 255), "�~2");
	DrawFormatString(45, 150, GetColor(0, 0, 255), "%d", eHitness);
}

// �L�����N�^�[�̍U��
void Character::CharacterAttack(STATUS* myStatus, STATUS* eStatus, int count) 
{
	myStatus->isAttack = true;

	// �_���[�W�v�Z
	int damage = myStatus->myParam.POWER - eStatus->myParam.DEFENCE;
	// �}�C�i�X��0
	if (damage < 0) damage = 0;

	// ��������(4�ȏ�傫�����2��U��)
	int mySPD = myStatus->myParam.SPEED - eStatus->myParam.SPEED;

	// ������ (�U�����̖����� - ������̉��)
	int myHitness = myStatus->myParam.ATTACK_HIT - eStatus->myParam.ATTACK_AVO;

	// �m���i�����́j
	double probability = myHitness;

	// �����̏�����
	srand((unsigned)time(NULL));
	if (GetRand(100) <= probability) {
		CharacterDamage(myStatus, eStatus, probability, damage);
		// �U�����������|������퓬�I��
		if (eStatus->isDeath == true) {
			myStatus->canAttack = false;
			myStatus->isAttack = false;
			return;
		}
	}

	// ����
	if(count < 2 && myStatus->AttackRange == eStatus->AttackRange) CharacterAttack(eStatus, myStatus, 2);
	else if (count <= 2) {
		//	��������̒ǌ�
		if (mySPD >= 4) {
			CharacterAttack(myStatus, eStatus, 3);
			return;
		}
		// �G����̒ǌ�
		else if (mySPD <= -4 && myStatus->AttackRange == eStatus->AttackRange) {
			CharacterAttack(eStatus, myStatus, 3);
			return;
		}
	}

	// �ǌ����o�Ȃ��Ȃ�퓬�I��
	if (mySPD < 4 || mySPD > -4 || count < 2) myStatus->canAttack = false;
}

// �U���̏���
void Character::CharacterDamage(STATUS* myStatus, STATUS* eStatus, double probability, int damage)
{
	eStatus->myParam.HP -= damage;

	if (eStatus->myParam.HP < 0) {
		eStatus->isDeath = true;
	}
}

// �^���I�J�����Ƃ̃I�t�Z�b�g�̌v�Z
void Character::SetCameraOffset(STATUS* status, int dir, bool horizontal)
{
	// ���E�v�Z
	if (horizontal) {
		status->PosX += CHIP_SIZE * dir;
		status->_PosX = status->PosX;
	}
	// �㉺�v�Z
	else {
		status->PosY += CHIP_SIZE * dir;
		status->_PosY = status->PosY;
	}
}

void Character::MoveAreaClear()
{
	for (int y = 0; y < MAP_SIZEY; y++) {
		for (int x = 0; x < MAP_SIZEX; x++) {
			moveToPos[y][x] = -1;
			moveArrow[y][x] = false;
			stage->StageUpdate(x, y);
		}
	}
}

