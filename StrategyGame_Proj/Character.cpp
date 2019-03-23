#include "Character.h"
#include "KeyInput.h"
#include <time.h>

#define PI 3.141592654

// �R���X�g���N�^
Character::Character()
{
	HpBar = LoadGraph(HP_BAR);
	HpBarBox = LoadGraph(HP_BARBOX);
	MoveArea = LoadGraph(CAN_MOVE_AREA);
	AttackArea = LoadGraph(CAN_ATTACK_AREA);
	AttackDetail = LoadGraph(ATTACK_DETAIL);
	DamageDetail = LoadGraph(DAMAGE_DETAIL);
	LoadDivGraph(ARROW, 6, 6, 1, CHIP_SIZE, CHIP_SIZE, ArrowImage);


	// �C���X�^���X�쐬
	stage = StageCreate::Instance();

	for (int y = 0; y < StageCreate::Instance()->MAP_SIZEY; y++) {
		for (int x = 0; x < StageCreate::Instance()->MAP_SIZEX; x++) {
			moveToPos[y][x] = -1;
			moveArrow[y][x] = false;
			stage->StageUpdate(x, y);
		}
	}
}

 void Character::Character_Initialize(string pass, string name, string team, int posX, int posY)
{
	 //if (status == nullptr) return;

	 GetCharacterParam(pass);

	 // �X�e�[�^�X�ݒ�
	 myStatus->PosX = posX;
	 myStatus->PosY = posY;
	 myStatus->_PosX = posX;
	 myStatus->_PosY = posY;
	 myStatus->AnimHandle = 0;
	 myStatus->AttackRange = 1;
	 myStatus->myTeam = team;
	 LoadDivGraph(CHARACTER_IMG, 20, 4, 5, CHIP_SIZE, CHIP_SIZE, myStatus->Image);

	 // �p�����[�^�ݒ�
	 myStatus->myParam.NAME = name;
	 myStatus->myParam.HP = _param[(int)PLAYER_PARAM::HP];
	 myStatus->myParam.POWER = _param[(int)PLAYER_PARAM::POWER];
	 myStatus->myParam.TECHNIQUE = _param[(int)PLAYER_PARAM::TECHNIQUE];
	 myStatus->myParam.SPEED = _param[(int)PLAYER_PARAM::SPEED];
	 myStatus->myParam.LUCKY = _param[(int)PLAYER_PARAM::LUCKY];
	 myStatus->myParam.DEFENCE = _param[(int)PLAYER_PARAM::DEFENCE];
	 myStatus->myParam.MAGIC_DEFENCE = _param[(int)PLAYER_PARAM::MAGIC_DEFENCE];
	 myStatus->myParam.PHYSIQUE = _param[(int)PLAYER_PARAM::PHYSIQUE];
	 myStatus->myParam.MOVERANGE = _param[(int)PLAYER_PARAM::MOVERANGE];

	 // �K�E�� (�Z�p�����[�^ / 2)
	 myStatus->myParam.ATTACK_CLT = myStatus->myParam.TECHNIQUE / 2;
	 // ��䗦 (�����p�����[�^ * 2 + �K�^�p�����[�^)
	 myStatus->myParam.ATTACK_AVO = myStatus->myParam.SPEED * 2 + myStatus->myParam.LUCKY;
	 // ������ (���햽�� + �Z�p�����[�^ * 2.5)
	 myStatus->myParam.ATTACK_HIT = 85 + (int)(myStatus->myParam.TECHNIQUE * 2.5);
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

 // �ړ��ĊJ(�^�[���J�n���ɌĂяo��)
 void Character::TurnStart()
 {
	 myStatus->canMove = true;
 }

 // �L�����N�^�[����̕`��
 void Character::SpriteDraw(int x, int y, int img)
 {
	 DrawGraph(x, y, img, true);
 }

// �L�����N�^�[�A�j���[�V����
void Character::CharacterAnim()
{
	if (myStatus == nullptr) return;

	// �I����ԂȂ�A�j���[�V�����X�V
	if (myStatus->isSelect) {
		myStatus->AnimHandle += 0.1f;
		if (myStatus->AnimHandle > 7.0f && myStatus->canMove)
			myStatus->AnimHandle = 4.0f;
	}

	// xPos, yPos�̈ʒu�ɃL�����N�^�[��`��
	DrawGraph(myStatus->PosX, myStatus->PosY, myStatus->Image[(int)myStatus->AnimHandle], true);

	// �I�𒆂łȂ����j�b�g�̈ʒu�Ƀ`�F�b�N
	if (myStatus->isSelect == false) stage->onUnit[myStatus->PosY / CHIP_SIZE][myStatus->PosX / CHIP_SIZE] = myStatus->myTeam;
}

// �L�����N�^�[�̈ړ�
bool Character::CharacterMove(int moveX, int moveY) 
{
	if (myStatus == nullptr) return false;

	// �����̈ʒu��I��������ړ��I��
	if (myStatus->PosX == moveX && myStatus->PosY == moveY) {
		myStatus->isSelect = false;
		myStatus->canMove = false;
		myStatus->AnimHandle = 0;
		AttackCheck();
		return false;
	}

	// �ړ��s�\�G���A��I�������烊�^�[��
	if (stage->checkMove[moveY / CHIP_SIZE][moveX / CHIP_SIZE] == false
		|| stage->onUnit[moveY / CHIP_SIZE][moveX / CHIP_SIZE] != "NONE") {
		myStatus->isSelect = false;
		myStatus->AnimHandle = 0;
		return false;
	}

	myStatus->canMove = false;

	// ���݂̒n�_�̓񎟌��z��p�f�[�^
	int valueX = moveX - myStatus->_PosX;
	int valueY = moveY - myStatus->_PosY;
	// ���ڎw���n�_�̓񎟌��z��p�f�[�^
	int nextX = myStatus->_PosX / CHIP_SIZE;
	int nextY = myStatus->_PosY / CHIP_SIZE;
	
	// �����̂���n�_�̓`�F�b�N
	moveArrow[nextY][nextX] = false;

	// �E�ړ�
	if (nextX + 1 <= 14 && moveArrow[nextY][nextX + 1] == true) {
		// �摜�̐؂�ւ�
		if (myStatus->AnimHandle < 8.0f || myStatus->AnimHandle > 11.0f)
			myStatus->AnimHandle = 8.0f;

		myStatus->PosX += 6;

		// ��}�X���̈ړ�
		if (myStatus->PosX - myStatus->_PosX == CHIP_SIZE) {
			myStatus->_PosX = myStatus->PosX;
			moveArrow[nextY][nextX + 1] = false;
		}

		// �ŏI�n�_�ɓ��B������ړ��I��
		if (myStatus->PosX == moveX && myStatus->PosY == moveY) {
			myStatus->isSelect = false;
			AttackCheck();
			myStatus->AnimHandle = 0.0f;
			return false;
		}

		return true;
	}

	// ���ړ�
	if (nextX - 1 >= 0 && moveArrow[nextY][nextX - 1] == true) {
		// �摜�̐؂�ւ�
		if (myStatus->AnimHandle < 12.0f || myStatus->AnimHandle > 15.0f)
			myStatus->AnimHandle = 12.0f;

		myStatus->PosX -= 6;

		// ��}�X���̈ړ�
		if (myStatus->PosX - myStatus->_PosX == -CHIP_SIZE) {
			myStatus->_PosX = myStatus->PosX;
			moveArrow[nextY][nextX - 1] = false;
		}

		// �ŏI�n�_�ɓ��B������ړ��I��
		if (myStatus->PosX == moveX && myStatus->PosY == moveY) {
			myStatus->isSelect = false;
			AttackCheck();
			myStatus->AnimHandle = 0.0f;
			return false;
		}

		return true;
	}

	// ���ړ�
	if (nextY + 1 <= 9 && moveArrow[nextY + 1][nextX] == true) {
		// �摜�̐؂�ւ�
		if (myStatus->AnimHandle < 4.0f || myStatus->AnimHandle > 7.0f)
			myStatus->AnimHandle = 4.0f;

		myStatus->PosY += 6;

		// ��}�X���̈ړ�
		if (myStatus->PosY - myStatus->_PosY == CHIP_SIZE) {
			myStatus->_PosY = myStatus->PosY;
			moveArrow[nextY + 1][nextX] = false;
		}

		// �ŏI�n�_�ɓ��B������ړ��I��
		if (myStatus->PosX == moveX && myStatus ->PosY == moveY) {
			myStatus->isSelect = false;
			AttackCheck();
			myStatus->AnimHandle = 0.0f;
			return false;
		}

		return true;
	}

	// ��ړ�
	if (nextY - 1 >= 0 && moveArrow[nextY - 1][nextX] == true) {
		// �摜�̐؂�ւ�
		if (myStatus->AnimHandle < 16.0f || myStatus->AnimHandle > 19.0f)
			myStatus->AnimHandle = 16.0f;

		myStatus->PosY -= 6;

		// ��}�X���̈ړ�
		if (myStatus->PosY - myStatus->_PosY == -CHIP_SIZE) {
			myStatus->_PosY = myStatus->PosY;
			moveArrow[nextY - 1][nextX] = false;
		}

		// �ŏI�n�_�ɓ��B������ړ��I��
		if (myStatus->PosX == moveX && myStatus->PosY == moveY) {
			myStatus->isSelect = false;
			AttackCheck();
			myStatus->AnimHandle = 0.0f;
			return false;
		}

		return true;
	}

	return true;
}

// �ړ��͈͌v�Z
void Character::MoveRange(int x, int y, int moveCost)
{
	if (myStatus == nullptr) return;

	DrawFormatString(48, 48, GetColor(0, 255, 0), "HP [%d]", myStatus->myParam.HP);

	// �J�����Ƃ̍���
	float cameraX = KeyInput::Instance()->cameraPos.x, cameraY = KeyInput::Instance()->cameraPos.y;
	int offsetX = (int)cameraX / CHIP_SIZE, offsetY = (int)cameraY / CHIP_SIZE;

	// ���݂̒n�_�̓񎟌��z��f�[�^�A�N�Z�X�p
	int valueX = x / CHIP_SIZE, valueY = y / CHIP_SIZE;
	// �J�����̍������݂̓񎟌��z��f�[�^�A�N�Z�X�p
	int _valueX = valueX + offsetX, _valueY = valueY + offsetY;

	// ��ʊO�Ȃ烊�^�[��
	if (valueX < 0 || valueX > 14 || valueY < 0 || valueY > 9)
		return;
	// ��ʊO�Ȃ烊�^�[��
	if (_valueX < 0 || _valueX > 14 || _valueY < 0 || _valueY > 9)
		return;

	// �ړ�������i�[
	if (moveToPos[valueY][valueX] < moveCost) moveToPos[valueY][valueX] = moveCost;

	// ���������Ȃ��������͐i�߂Ȃ��ꏊ�̏ꍇ�̓��^�[��
	if (stage->stageList[_valueY][_valueX] > 0 && moveCost >= 0) {
		SpriteDraw(x, y, AttackArea); 
		moveToPos[valueY][valueX] = 0;
		return;
	}
	else if (stage->stageList[_valueY][_valueX] > 0 || moveCost == 0) return;

	// �ړ��͈͕\�����Ă��Ȃ��Ȃ�\��
	if (stage->checkMove[valueY][valueX] != true) {
		stage->checkMove[valueY][valueX] = true;
		SpriteDraw(x, y, MoveArea);
	}

	// ��֍s����Ȃ�`�F�b�N
	if (valueY - 1 >= 0 && moveToPos[valueY - 1][valueX] < moveCost) MoveRange(x, y - CHIP_SIZE, moveCost - 1);
	// �E�֍s����Ȃ�`�F�b�N
	if (valueX + 1 <= 14 && moveToPos[valueY][valueX + 1] < moveCost) MoveRange(x + CHIP_SIZE, y, moveCost - 1);
	// ���֍s����Ȃ�`�F�b�N
	if (valueX - 1 >= 0 && moveToPos[valueY][valueX - 1] < moveCost)MoveRange(x - CHIP_SIZE, y, moveCost - 1);
	// ���֍s����Ȃ�`�F�b�N
	if (valueY + 1 <= 9 && moveToPos[valueY + 1][valueX] < moveCost) MoveRange(x, y + CHIP_SIZE, moveCost - 1);
}

// �U���͈͕\��
void Character::AttackRange()
{
	for (int y = 0; y < StageCreate::Instance()->MAP_SIZEY; y++) {
		for (int x = 0; x < StageCreate::Instance()->MAP_SIZEX; x++) {
			if (moveToPos[y][x] == 0) SpriteDraw(x * CHIP_SIZE, y * CHIP_SIZE, AttackArea);
		}
	}
}

// �K�C�h���C���\��
void Character::DrawMoveArrow(int x, int y, int moveValue)
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

	// �G�̍s��
	if (myStatus->myTeam == "Enemy") {
		// ����܂ł̓��͏����N���A
		OldPosX.clear();
		OldPosY.clear();

		if (stage->checkMove[valueY][valueX] == true) {

			// ���j�b�g�̈ʒu�łȂ��Ȃ炱������t�T�m
			if (myStatus->PosX != x || myStatus->PosY != y) {
				DrawGraph(x, y, ArrowImage[moveValue], true);
				moveArrow[valueY][valueX] = true;
			}

			// ���j�b�g�Ɍ������ă��[�g���t�T�m���Ă���
			if (valueY + 1 < 9 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY + 1][valueX] && stage->checkMove[valueY + 1][valueX] == true) {
				DrawMoveArrow(x, y + CHIP_SIZE, 5);
				return;
			}
			if (valueY - 1 > 0 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY - 1][valueX] && stage->checkMove[valueY - 1][valueX] == true) {
				DrawMoveArrow(x, y - CHIP_SIZE, 5);
				return;
			}
			if (valueX + 1 < 14 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX + 1] && stage->checkMove[valueY][valueX + 1] == true) {
				DrawMoveArrow(x + CHIP_SIZE, y, 5);
				return;
			}
			if (valueX - 1 > 0 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX - 1] && stage->checkMove[valueY][valueX - 1] == true) {
				DrawMoveArrow(x - CHIP_SIZE, y, 5);
				return;
			}
		}
	}

	// ���͏��ɃK�C�h���C���\��
	if (myStatus->myParam.MOVERANGE - moveToPos[valueY][valueX] >= moveCount || OldPosX.size() == moveCount) {
		for (unsigned int num = 0; num < OldPosX.size(); num++) {
			if (stage->checkMove[OldPosY[num] / CHIP_SIZE][OldPosX[num] / CHIP_SIZE] == true) {
				DrawGraph(OldPosX[num], OldPosY[num], ArrowImage[moveValue], true);
				moveArrow[OldPosY[num] / CHIP_SIZE][OldPosX[num] / CHIP_SIZE] = true;
			}
		}
	}
	// �ŒZ�ōs���郋�[�g��\��
	else if(myStatus->myParam.MOVERANGE - moveToPos[valueY][valueX] < moveCount || OldPosX.size() != moveCount) {

		// ����܂ł̓��͏����N���A
		OldPosX.clear();
		OldPosY.clear();

		if (stage->checkMove[valueY][valueX] == true) {

			// ���j�b�g�̈ʒu�łȂ��Ȃ炱������t�T�m
			if (myStatus->PosX != x || myStatus->PosY != y) {
				DrawGraph(x, y, ArrowImage[moveValue], true);
				moveArrow[y / CHIP_SIZE][x / CHIP_SIZE] = true;
			}

			// ���j�b�g�Ɍ������ă��[�g���t�T�m���Ă���
			if (valueY + 1 < 9 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY + 1][valueX] && stage->checkMove[valueY + 1][valueX] == true) {
				DrawMoveArrow(x, y + CHIP_SIZE, 5);
				return;
			}
			if (valueY - 1 > 0 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY - 1][valueX] && stage->checkMove[valueY - 1][valueX] == true) {
				DrawMoveArrow(x, y - CHIP_SIZE, 5);
				return;
			}
			if (valueX + 1 < 14 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX + 1] && stage->checkMove[valueY][valueX + 1] == true) {
				DrawMoveArrow(x + CHIP_SIZE, y, 5);
				return;
			}
			if (valueX - 1 > 0 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX - 1] && stage->checkMove[valueY][valueX - 1] == true) {
				DrawMoveArrow(x - CHIP_SIZE, y, 5);
				return;
			}
		}
	}
}

// �U���ł��邩�`�F�b�N
void Character::AttackCheck()
{
	if (myStatus == nullptr) return;

	myStatus->canAttack = false;

	int valueX = myStatus->PosX / CHIP_SIZE, valueY = myStatus->PosY / CHIP_SIZE;
	if (valueX + 1 <= 14 && stage->onUnit[valueY][valueX + 1] != "NONE" && stage->onUnit[valueY][valueX + 1] != myStatus->myTeam) myStatus->canAttack = true;
	if (valueX - 1 >= 0 && stage->onUnit[valueY][valueX - 1] != "NONE" && stage->onUnit[valueY][valueX - 1] != myStatus->myTeam) myStatus->canAttack = true;
	if (valueY + 1 <= 9 && stage->onUnit[valueY + 1][valueX] != "NONE" && stage->onUnit[valueY + 1][valueX] != myStatus->myTeam) myStatus->canAttack = true;
	if (valueY - 1 >= 0 && stage->onUnit[valueY - 1][valueX] != "NONE" && stage->onUnit[valueY - 1][valueX] != myStatus->myTeam) myStatus->canAttack = true;
}

// �U���͈͕`��
void Character::AttackableDraw()
{
	int valueX = myStatus->PosX / CHIP_SIZE, valueY = myStatus->PosY / CHIP_SIZE;
	if (valueX + 1 <= 14 && stage->onUnit[valueY][valueX + 1] != "NONE" && stage->onUnit[valueY][valueX + 1] != myStatus->myTeam) SpriteDraw(myStatus->PosX + CHIP_SIZE, myStatus->PosY, AttackArea);
	if (valueX - 1 >= 0 && stage->onUnit[valueY][valueX - 1] != "NONE" && stage->onUnit[valueY][valueX - 1] != myStatus->myTeam) SpriteDraw(myStatus->PosX - CHIP_SIZE, myStatus->PosY, AttackArea);
	if (valueY + 1 <= 9 && stage->onUnit[valueY + 1][valueX] != "NONE" && stage->onUnit[valueY + 1][valueX] != myStatus->myTeam) SpriteDraw(myStatus->PosX, myStatus->PosY + CHIP_SIZE, AttackArea);
	if (valueY - 1 >= 0 && stage->onUnit[valueY - 1][valueX] != "NONE" && stage->onUnit[valueY - 1][valueX] != myStatus->myTeam) SpriteDraw(myStatus->PosX, myStatus->PosY - CHIP_SIZE, AttackArea);
}

// �U���̏ڍו\��
void Character::GetAttackDetail(Character* eStatus)
{
	// �З� (�U�����̗� - ������̎����)
	int mySTR = myStatus->myParam.POWER - eStatus->myStatus->myParam.DEFENCE;
	if (mySTR < 0) mySTR = 0;
	int eSTR = eStatus->myStatus->myParam.POWER - myStatus->myParam.DEFENCE;
	if (eSTR < 0) eSTR = 0;

	// ��������(4�ȏ�傫�����2��U��)
	int mySPD = myStatus->myParam.SPEED - eStatus->myStatus->myParam.SPEED;

	// ������ (�U�����̖����� - ������̉��)
	int myHitness = myStatus->myParam.ATTACK_HIT - eStatus->myStatus->myParam.ATTACK_AVO;
	int eHitness = eStatus->myStatus->myParam.ATTACK_HIT - eStatus->myStatus->myParam.ATTACK_AVO;

	SpriteDraw(0, 0, AttackDetail);
	DrawFormatString(80, 100, GetColor(255, 255, 0), "HP");
	DrawFormatString(80, 125, GetColor(255, 255, 0), "�З�");
	DrawFormatString(80, 150, GetColor(255, 255, 0), "����");

	DrawFormatString(120, 100, GetColor(0, 0, 255), "%d", myStatus->myParam.HP);
	DrawFormatString(120, 125, GetColor(0, 0, 255), "%d", mySTR);
	if (mySPD >= 4) DrawFormatString(130, 130, GetColor(255, 255, 255), "�~2");
	DrawFormatString(120, 150, GetColor(0, 0, 255), "%d", myHitness);

	DrawFormatString(45, 100, GetColor(0, 0, 255), "%d", eStatus->myStatus->myParam.HP);
	DrawFormatString(45, 125, GetColor(0, 0, 255), "%d", eSTR);
	if (mySPD <= -4) DrawFormatString(55, 130, GetColor(255, 255, 255), "�~2");
	DrawFormatString(45, 150, GetColor(0, 0, 255), "%d", eHitness);
}

// �U���A�j���[�V����
bool Character::AttackAnimation(Character* eCharacter, int count)
{
	DrawGraph(0, 0, DamageDetail, true);

	// �U���������擾
	int moveX = eCharacter->myStatus->PosX - myStatus->_PosX;
	int moveY = eCharacter->myStatus->PosY - myStatus->_PosY;

	if (moveX > 0) {
		// �U���A�j���[�V����
		if(myStatus->animReset) myStatus->PosX -= 6;
		else myStatus->PosX += 6;
		
		if (myStatus->PosX - myStatus->_PosX == CHIP_SIZE) myStatus->animReset = true;
		if (myStatus->PosX == myStatus->_PosX) {
			CharacterAttack(eCharacter, count);
			myStatus->animReset = false;
			return false;
		}
	}
	else if (moveX < 0) {
		// �U���A�j���[�V����
		if (myStatus->animReset) myStatus->PosX += 6;
		else myStatus->PosX -= 6;

		if (myStatus->PosX - myStatus->_PosX == -CHIP_SIZE) myStatus->animReset = true;
		if (myStatus->PosX == myStatus->_PosX) {
			CharacterAttack(eCharacter, count);
			myStatus->animReset = false;
			return false;
		}
	}
	if (moveY > 0) {
		// �U���A�j���[�V����
		if (myStatus->animReset) myStatus->PosY -= 6;
		else myStatus->PosY += 6;

		if (myStatus->PosY - myStatus->_PosY == CHIP_SIZE) myStatus->animReset = true;
		if (myStatus->PosY == myStatus->_PosY) {
			CharacterAttack(eCharacter, count);
			myStatus->animReset = false;
			return false;
		}
	}
	else if (moveY < 0) {
		// �U���A�j���[�V����
		if (myStatus->animReset) myStatus->PosY += 6;
		else myStatus->PosY -= 6;

		if (myStatus->PosY - myStatus->_PosY == -CHIP_SIZE) myStatus->animReset = true;
		if (myStatus->PosY == myStatus->_PosY) {
			CharacterAttack(eCharacter, count);
			myStatus->animReset = false;
			return false;
		}
	}

	return true;
}

// �L�����N�^�[�̍U��
void Character::CharacterAttack(Character* eCharacter, int count)
{
	// �_���[�W�v�Z
	int damage = myStatus->myParam.POWER - eCharacter->myStatus->myParam.DEFENCE;
	// �}�C�i�X��0
	if (damage < 0) damage = 0;

	// ��������(4�ȏ�傫�����2��U��)
	myStatus->myParam.ATTACK_SPEED = myStatus->myParam.SPEED - eCharacter->myStatus->myParam.SPEED;

	// ������ (�U�����̖����� - ������̉��)
	int myHitness = myStatus->myParam.ATTACK_HIT - eCharacter->myStatus->myParam.ATTACK_AVO;

	// �m���i�����́j
	double probability = myHitness;

	// �����̏�����
	srand((unsigned)time(NULL));
	if (GetRand(100) <= probability) {
		CharacterDamage(eCharacter, damage);
		// �G���|�����炻�̎��_�Ő퓬�I��
		if (eCharacter->myStatus->isDeath) {
			myStatus->isAttack = false;
			return;
		}
	}

	// �U���I��
	myStatus->isAttack = false;

	// �G�̔���
	if (count < 2 && myStatus->AttackRange == eCharacter->myStatus->AttackRange) {
		eCharacter->myStatus->isAttack = true;
		return;
	}
	else if (count >= 2) {
		//	��������̒ǌ�
		if (myStatus->myParam.ATTACK_SPEED >= 4) {
			myStatus->isAttack = true;
			return;
		}
		// �G����̒ǌ�
		else if (eCharacter->myStatus->myParam.ATTACK_SPEED >= 4 && myStatus->AttackRange == eCharacter->myStatus->AttackRange) {
			eCharacter->myStatus->isAttack = true;
			return;
		}
	}

	// �ǌ����o�Ȃ��Ȃ�퓬�I��
	if (myStatus->myParam.ATTACK_SPEED < 4 || eCharacter->myStatus->myParam.ATTACK_SPEED < 4 || count < 2) myStatus->canAttack = false;
}

// �U���̏���
void Character::CharacterDamage(Character* eCharacter, int damage)
{
	eCharacter->myStatus->myParam.HP -= damage;

	if (eCharacter->myStatus->myParam.HP < 0) {
		eCharacter->myStatus->isDeath = true;
	}
	myStatus->canAttack = false;
}

// �^���I�J�����Ƃ̃I�t�Z�b�g�̌v�Z
void Character::SetCameraOffset(int dir, bool horizontal)
{
	// ���E�v�Z
	if (horizontal) {
		myStatus->PosX += CHIP_SIZE * dir;
		myStatus->_PosX = myStatus->PosX;
	}
	// �㉺�v�Z
	else {
		myStatus->PosY += CHIP_SIZE * dir;
		myStatus->_PosY = myStatus->PosY;
	}
}

void Character::MoveAreaClear(vector<Character*> _character)
{
	for (int y = 0; y < StageCreate::Instance()->MAP_SIZEY; y++) {
		for (int x = 0; x < StageCreate::Instance()->MAP_SIZEX; x++) {
			for (Character* character : _character) {
				moveToPos[y][x] = -1;
				moveArrow[y][x] = false;
				if(character->myStatus->PosX != x, character->myStatus->PosY != y) stage->StageUpdate(x, y);
			}
		}
	}
}

