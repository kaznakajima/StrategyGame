#include "Character.h"
#include "KeyInput.h"

#define PI 3.141592654

// �R���X�g���N�^
Character::Character()
{
	itemCount = 0;

	/*shared_ptr<DrawManager> attackDetail(new DrawParts(ATTACK_DETAIL, 1));
	DrawManager::Instance()->AddDrawList(attackDetail);*/
	/*FileManager::Instance()->GetFileHandle(CAN_MOVE_AREA);
	FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA);
	FileManager::Instance()->GetFileHandle(ATTACK_DETAIL);*/
	LoadDivGraph(ARROW, 6, 6, 1, CHIP_SIZE, CHIP_SIZE, ArrowImage);

	for (size_t y = 0; y < StageCreate::Instance()->MAP_SIZEY; y++) {
		for (size_t x = 0; x < StageCreate::Instance()->MAP_SIZEX; x++) {
			moveToPos[y][x] = -1;
			moveArrow[y][x] = false;
			StageCreate::Instance()->StageUpdate((int)x, (int)y);
		}
	}
	myRen = make_shared<Renderer>(myStatus->Image[0], 3, 0);
	DrawManager::Instance()->AddDrawList(myRen);
}

 void Character::Character_Initialize(string pass, string team, int posX, int posY)
{
	 // �����i�ǉ�
	 if (team == "Player") AddItem(IRON_SWORD); // �v���C���[�͌�
	 else if (team == "Enemy") AddItem(IRON_AX); // �G�͕�

	 GetCharacterParam(FileManager::Instance()->GetDataName(pass));

	 // �X�e�[�^�X�ݒ�
	 myData = pass;
	 myStatus->NAME = FileManager::Instance()->GetFileName(pass);
	 myStatus->xPos = posX, myStatus->yPos = posY;
	 myStatus->_xPos = posX, myStatus->_yPos = posY;
	 myStatus->AnimHandle = 0;
	 myStatus->AttackRange = Item[0]->myParam.RANGE;
	 myStatus->myTeam = team;
	 if(myStatus->myTeam == "Player") LoadDivGraph(P_CHARACTER_IMG, 20, 4, 5, CHIP_SIZE, CHIP_SIZE, myStatus->Image);
	 else LoadDivGraph(E_CHARACTER_IMG, 20, 4, 5, CHIP_SIZE, CHIP_SIZE, myStatus->Image);
}

 // �p�����[�^�擾
 void Character::GetCharacterParam(string pass)
 {
	 fstream file;
	 file.open(pass, ios::in | ios::binary);
	 file.read((char*)&myStatus->myParam, sizeof(myStatus->myParam));

	 file.close();

	 // MaxHP�̐ݒ�
	 myStatus->HP = myStatus->myParam.MaxHP;

	 // �U�����x
	 int weight = Item[0]->myParam.WEIGHT - myStatus->myParam.PHYSIQUE;
	 if (weight < 0) weight = 0;
	 myStatus->myParam.ATTACK_SPEED = myStatus->myParam.SPEED - weight;
	 // �K�E�� (�Z�p�����[�^ / 2)
	 myStatus->myParam.ATTACK_CLT = myStatus->myParam.TECHNIQUE / 2 + Item[0]->myParam.CLT;
	 // ��� (�����p�����[�^ * 2 + �K�^�p�����[�^)
	 myStatus->myParam.ATTACK_AVO = myStatus->myParam.ATTACK_SPEED * 2 + myStatus->myParam.LUCKY;
	 // ������ (���햽�� + �Z�p�����[�^ * 2 + �K�^�p�����[�^ / 2)
	 myStatus->myParam.ATTACK_HIT = Item[0]->myParam.HIT + (int)(myStatus->myParam.TECHNIQUE * 2) + (int)(myStatus->myParam.LUCKY / 2);
 }

 // ���j�b�g�̏ڍ׏��̕`��
 void Character::DrawCharacterDetail()
 {
	 DrawGraph(0, 0, FileManager::Instance()->GetFileHandle(CHARACTER_DETAIL), true);

	 DrawFormatString(140, 325, GetColor(0, 0, 0), myStatus->NAME.c_str());
	 DrawFormatString(48, 380, GetColor(0, 0, 0), "Lv_%d", myStatus->myParam.LEVEL);
	 DrawFormatString(48, 400, GetColor(0, 0, 0), "HP_%d / %d", myStatus->HP, myStatus->myParam.MaxHP);
	 DrawFormatString(400, 48, GetColor(0, 0, 0), "��_%d", myStatus->myParam.POWER);
	 DrawFormatString(400, 96, GetColor(0, 0, 0), "�Z_%d", myStatus->myParam.TECHNIQUE);
	 DrawFormatString(400, 144, GetColor(0, 0, 0), "����_%d", myStatus->myParam.SPEED);
	 DrawFormatString(400, 192, GetColor(0, 0, 0), "�K�^_%d", myStatus->myParam.LUCKY);
	 DrawFormatString(400, 240, GetColor(0, 0, 0), "���_%d", myStatus->myParam.DEFENCE);
	 DrawFormatString(400, 288, GetColor(0, 0, 0), "���h_%d", myStatus->myParam.MAGIC_DEFENCE);
	 DrawFormatString(500, 48, GetColor(0, 0, 0), "�ړ�_%d", myStatus->myParam.MOVERANGE - 1);
	 DrawFormatString(500, 96, GetColor(0, 0, 0), "�̊i_%d", myStatus->myParam.PHYSIQUE);
 }

 // �ړ��ĊJ(�^�[���J�n���ɌĂяo��)
 void Character::TurnStart()
 {
	 myStatus->canMove = true;
	 myStatus->AnimHandle = 0.0f;
	 myStatus->canAttack = false;
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
	else {
		if (myStatus->canMove == false && myStatus->canAttack == false) myStatus->AnimHandle = 3.0f; 
	}

	// xPos, yPos�̈ʒu�ɃL�����N�^�[��`��
	myRen->SetImageID(myStatus->Image[(int)myStatus->AnimHandle]);
	myRen->SetPosition(myStatus->xPos, myStatus->yPos);
	//DrawGraph(myStatus->xPos, myStatus->yPos, myStatus->Image[(int)myStatus->AnimHandle], true);
}

// �L�����N�^�[�̈ړ�
bool Character::CharacterMove(int moveX, int moveY) 
{
	if (myStatus == nullptr) return false;

	// �����̈ʒu��I��������ړ��I��
	if (myStatus->xPos == moveX && myStatus->yPos == moveY) {
		myStatus->isSelect = false;
		myStatus->canMove = false;
		myStatus->AnimHandle = 0;
		AttackCheck();
		return false;
	}

	// �ړ��s�\�G���A��I�������烊�^�[��
	if (StageCreate::Instance()->checkMove[moveY / CHIP_SIZE][moveX / CHIP_SIZE] == false
		|| StageCreate::Instance()->onUnit[moveY / CHIP_SIZE][moveX / CHIP_SIZE] != "NONE") {
		myStatus->isSelect = false;
		myStatus->AnimHandle = 0;
		return false;
	}

	myStatus->canMove = false;

	// ���݂̒n�_�̓񎟌��z��p�f�[�^
	int valueX = moveX - myStatus->_xPos;
	int valueY = moveY - myStatus->_yPos;
	// ���ڎw���n�_�̓񎟌��z��p�f�[�^
	int nextX = myStatus->_xPos / CHIP_SIZE;
	int nextY = myStatus->_yPos / CHIP_SIZE;
	
	// �����̂���n�_�̓`�F�b�N
	moveArrow[nextY][nextX] = false;

	// �E�ړ�
	if (nextX + 1 <= 14 && moveArrow[nextY][nextX + 1] == true) {
		// �摜�̐؂�ւ�
		if (myStatus->AnimHandle < 8.0f || myStatus->AnimHandle > 11.0f)
			myStatus->AnimHandle = 8.0f;

		myStatus->xPos += 6;

		// ��}�X���̈ړ�
		if (myStatus->xPos - myStatus->_xPos == CHIP_SIZE) {
			myStatus->_xPos = myStatus->xPos;
			moveArrow[nextY][nextX + 1] = false;
		}

		// �ŏI�n�_�ɓ��B������ړ��I��
		if (myStatus->xPos == moveX && myStatus->yPos == moveY) {
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

		myStatus->xPos -= 6;

		// ��}�X���̈ړ�
		if (myStatus->xPos - myStatus->_xPos == -CHIP_SIZE) {
			myStatus->_xPos = myStatus->xPos;
			moveArrow[nextY][nextX - 1] = false;
		}

		// �ŏI�n�_�ɓ��B������ړ��I��
		if (myStatus->xPos == moveX && myStatus->yPos == moveY) {
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

		myStatus->yPos += 6;

		// ��}�X���̈ړ�
		if (myStatus->yPos - myStatus->_yPos == CHIP_SIZE) {
			myStatus->_yPos = myStatus->yPos;
			moveArrow[nextY + 1][nextX] = false;
		}

		// �ŏI�n�_�ɓ��B������ړ��I��
		if (myStatus->xPos == moveX && myStatus ->yPos == moveY) {
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

		myStatus->yPos -= 6;

		// ��}�X���̈ړ�
		if (myStatus->yPos - myStatus->_yPos == -CHIP_SIZE) {
			myStatus->_yPos = myStatus->yPos;
			moveArrow[nextY - 1][nextX] = false;
		}

		// �ŏI�n�_�ɓ��B������ړ��I��
		if (myStatus->xPos == moveX && myStatus->yPos == moveY) {
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

	// ���������Ȃ��������͐i�߂Ȃ��ꏊ�̏ꍇ�A�G�w�c������ꍇ�̓��^�[��
	if (StageCreate::Instance()->stageList[_valueY][_valueX] > 0 && moveCost >= 0 || 
		StageCreate::Instance()->onUnit[valueY][valueX] != "NONE" && StageCreate::Instance()->onUnit[valueY][valueX] != myStatus->myTeam) 
	{
		if(moveCost == 0) StageCreate::Instance()->checkMove[valueY][valueX] = true;
		moveToPos[valueY][valueX] = 0;
		if (DrawManager::Instance()->GetDrawParts(FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA), x, y) != nullptr)
			DrawManager::Instance()->GetDrawParts(FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA), x, y)->SetVisible(true);
		return;
	}
	else if (StageCreate::Instance()->stageList[_valueY][_valueX] > 0 || moveCost == 0) return;

	// �ړ��͈͕\�����Ă��Ȃ��Ȃ�\��
	if (StageCreate::Instance()->checkMove[valueY][valueX] != true) {
		StageCreate::Instance()->checkMove[valueY][valueX] = true; 
		if (DrawManager::Instance()->GetDrawParts(FileManager::Instance()->GetFileHandle(CAN_MOVE_AREA), x, y) != nullptr)
			DrawManager::Instance()->GetDrawParts(FileManager::Instance()->GetFileHandle(CAN_MOVE_AREA), x, y)->SetVisible(true);
	}

	// ��֍s���邩�`�F�b�N
	if (valueY - 1 >= 0 && moveToPos[valueY - 1][valueX] < moveCost) MoveRange(x, y - CHIP_SIZE, moveCost - 1);
	// �E�֍s���邩�`�F�b�N
	if (valueX + 1 <= 14 && moveToPos[valueY][valueX + 1] < moveCost) MoveRange(x + CHIP_SIZE, y, moveCost - 1);
	// ���֍s���邩�`�F�b�N
	if (valueX - 1 >= 0 && moveToPos[valueY][valueX - 1] < moveCost)MoveRange(x - CHIP_SIZE, y, moveCost - 1);
	// ���֍s���邩�`�F�b�N
	if (valueY + 1 <= 9 && moveToPos[valueY + 1][valueX] < moveCost) MoveRange(x, y + CHIP_SIZE, moveCost - 1);
}

// �U���͈͕\��
void Character::AttackRange()
{
	// ���g�̍U���͈͕\��
	for (int y = 0; y < StageCreate::Instance()->MAP_SIZEY; y++) {
		for (int x = 0; x < StageCreate::Instance()->MAP_SIZEX; x++) {
			if (moveToPos[y][x] == 0) {
				int posX = x * CHIP_SIZE, posY = y * CHIP_SIZE;
				if (DrawManager::Instance()->GetDrawParts(FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA), posX, posY) != nullptr)
					DrawManager::Instance()->GetDrawParts(FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA), posX, posY)->SetVisible(true);
			}
		}
	}
}

// �K�C�h���C���\��
void Character::DrawMoveArrow(int x, int y, int moveValue)
{
	// ���݂̒n�_�̓񎟌��z��p�f�[�^
	int valueX = x / CHIP_SIZE, valueY = y / CHIP_SIZE;

	// �ړ��͈͂łȂ��Ȃ�\�����Ȃ�
	if (StageCreate::Instance()->checkMove[valueY][valueX] == false) {
		// ���͍��W�̃��Z�b�g
		OldPosX.clear(); 
		OldPosY.clear();
		// �ړ��l�̓J�E���g
		moveCount++;
		moveArrow[valueY][valueX] = false;
	}

	// �G�̍s��
	if (myStatus->myTeam == "Enemy") {

		if (StageCreate::Instance()->checkMove[valueY][valueX] == true) {

			// ���j�b�g�̈ʒu�łȂ��Ȃ炱������t�T�m
			if (myStatus->xPos != x || myStatus->yPos != y) {
				DrawGraph(x, y, ArrowImage[moveValue], true);
				moveArrow[valueY][valueX] = true;
			}

			// ���j�b�g�Ɍ������ă��[�g���t�T�m���Ă���
			// ��
			if (valueY + 1 < 9 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY + 1][valueX] && StageCreate::Instance()->checkMove[valueY + 1][valueX] == true) {
				DrawMoveArrow(x, y + CHIP_SIZE, 5);
				return;
			}
			// ���j�b�g�Ɍ������ă��[�g���t�T�m���Ă���
			// ��
			if (valueY - 1 > 0 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY - 1][valueX] && StageCreate::Instance()->checkMove[valueY - 1][valueX] == true) {
				DrawMoveArrow(x, y - CHIP_SIZE, 5);
				return;
			}
			// ���j�b�g�Ɍ������ă��[�g���t�T�m���Ă���
			// �E
			if (valueX + 1 < 14 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX + 1] && StageCreate::Instance()->checkMove[valueY][valueX + 1] == true) {
				DrawMoveArrow(x + CHIP_SIZE, y, 5);
				return;
			}
			// ���j�b�g�Ɍ������ă��[�g���t�T�m���Ă���
			// ��
			if (valueX - 1 > 0 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX - 1] && StageCreate::Instance()->checkMove[valueY][valueX - 1] == true) {
				DrawMoveArrow(x - CHIP_SIZE, y, 5);
				return;
			}
		}
	}

	// �ȉ�����̓v���C���[�^�[���̂ݎ��s

	// ���͏��ɃK�C�h���C���\��
	if (myStatus->myParam.MOVERANGE - moveToPos[valueY][valueX] >= moveCount || OldPosX.size() == moveCount) {
		for (unsigned int num = 0; num < OldPosX.size(); num++) {
			if (StageCreate::Instance()->checkMove[OldPosY[num] / CHIP_SIZE][OldPosX[num] / CHIP_SIZE] == true) {
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

		if (StageCreate::Instance()->checkMove[valueY][valueX] == true) {

			// ���j�b�g�̈ʒu�łȂ��Ȃ炱������t�T�m
			if (myStatus->xPos != x || myStatus->yPos != y) {
				DrawGraph(x, y, ArrowImage[moveValue], true);
				moveArrow[y / CHIP_SIZE][x / CHIP_SIZE] = true;
			}

			// ���j�b�g�Ɍ������ă��[�g���t�T�m���Ă���
			// ��
			if (valueY + 1 < 9 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY + 1][valueX] && StageCreate::Instance()->checkMove[valueY + 1][valueX] == true) {
				DrawMoveArrow(x, y + CHIP_SIZE, 5);
				return;
			}
			// ���j�b�g�Ɍ������ă��[�g���t�T�m���Ă���
			// ��
			if (valueY - 1 > 0 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY - 1][valueX] && StageCreate::Instance()->checkMove[valueY - 1][valueX] == true) {
				DrawMoveArrow(x, y - CHIP_SIZE, 5);
				return;
			}
			// ���j�b�g�Ɍ������ă��[�g���t�T�m���Ă���
			// �E
			if (valueX + 1 < 14 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX + 1] && StageCreate::Instance()->checkMove[valueY][valueX + 1] == true) {
				DrawMoveArrow(x + CHIP_SIZE, y, 5);
				return;
			}
			// ���j�b�g�Ɍ������ă��[�g���t�T�m���Ă���
			// ��
			if (valueX - 1 > 0 && moveToPos[valueY][valueX] + 1 == moveToPos[valueY][valueX - 1] && StageCreate::Instance()->checkMove[valueY][valueX - 1] == true) {
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

	int valueX = myStatus->xPos / CHIP_SIZE, valueY = myStatus->yPos / CHIP_SIZE;
	// ���g�̎l���ɓG������Ȃ�U���\�Ƃ���
	if (valueX + 1 <= 14 && StageCreate::Instance()->onUnit[valueY][valueX + 1] != "NONE" && StageCreate::Instance()->onUnit[valueY][valueX + 1] != myStatus->myTeam) myStatus->canAttack = true;
	if (valueX - 1 >= 0 && StageCreate::Instance()->onUnit[valueY][valueX - 1] != "NONE" && StageCreate::Instance()->onUnit[valueY][valueX - 1] != myStatus->myTeam) myStatus->canAttack = true;
	if (valueY + 1 <= 9 && StageCreate::Instance()->onUnit[valueY + 1][valueX] != "NONE" && StageCreate::Instance()->onUnit[valueY + 1][valueX] != myStatus->myTeam) myStatus->canAttack = true;
	if (valueY - 1 >= 0 && StageCreate::Instance()->onUnit[valueY - 1][valueX] != "NONE" && StageCreate::Instance()->onUnit[valueY - 1][valueX] != myStatus->myTeam) myStatus->canAttack = true;
}

// �U���͈͕`��
void Character::AttackableDraw()
{
	if (myStatus->canMove) return;

	int valueX = myStatus->xPos / CHIP_SIZE, valueY = myStatus->yPos / CHIP_SIZE;
	// �U���\�Ȉʒu�̕`��
	if (valueX + 1 <= 14 && StageCreate::Instance()->onUnit[valueY][valueX + 1] != "NONE" && StageCreate::Instance()->onUnit[valueY][valueX + 1] != myStatus->myTeam) {
		//SpriteDraw(myStatus->xPos + CHIP_SIZE, myStatus->yPos, FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA));
	}
	if (valueX - 1 >= 0 && StageCreate::Instance()->onUnit[valueY][valueX - 1] != "NONE" && StageCreate::Instance()->onUnit[valueY][valueX - 1] != myStatus->myTeam) {
		//SpriteDraw(myStatus->xPos - CHIP_SIZE, myStatus->yPos, FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA));
	}
	if (valueY + 1 <= 9 && StageCreate::Instance()->onUnit[valueY + 1][valueX] != "NONE" && StageCreate::Instance()->onUnit[valueY + 1][valueX] != myStatus->myTeam) {
		//SpriteDraw(myStatus->xPos, myStatus->yPos + CHIP_SIZE, FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA));
	}
	if (valueY - 1 >= 0 && StageCreate::Instance()->onUnit[valueY - 1][valueX] != "NONE" && StageCreate::Instance()->onUnit[valueY - 1][valueX] != myStatus->myTeam) {
		//SpriteDraw(myStatus->xPos, myStatus->yPos - CHIP_SIZE, FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA));
	}
}

// �U���̏ڍו\��
void Character::GetAttackDetail(shared_ptr<Character> const &eCharacter)
{
	// �З� (�U�����̗� - ������̎����)
	if (Item.empty() == false) myStatus->myParam.ATTACK_STR = myStatus->myParam.POWER + Item[0]->myParam.POWER;
	else myStatus->myParam.ATTACK_STR = 0;

	if (eCharacter->Item.empty() == false) eCharacter->myStatus->myParam.ATTACK_STR = eCharacter->myStatus->myParam.POWER + eCharacter->Item[0]->myParam.POWER;
	else eCharacter->myStatus->myParam.ATTACK_STR = 0;

	// �U���͂̌v�Z (�З� - �G�̎����(�n�`�␳��))
	int mySTR = myStatus->myParam.ATTACK_STR - (eCharacter->myStatus->myParam.DEFENCE +
		StageCreate::Instance()->GetTerrainParam(eCharacter->myStatus->xPos / CHIP_SIZE, eCharacter->myStatus->yPos / CHIP_SIZE, "DEF"));
	if (mySTR < 0) mySTR = 0;
	int eSTR = eCharacter->myStatus->myParam.ATTACK_STR - (myStatus->myParam.DEFENCE +
		StageCreate::Instance()->GetTerrainParam(myStatus->xPos / CHIP_SIZE, myStatus->yPos / CHIP_SIZE, "DEF"));;
	if (eSTR < 0) eSTR = 0;

	// ��������(4�ȏ�傫�����2��U��)
	int mySPD = myStatus->myParam.SPEED - eCharacter->myStatus->myParam.SPEED;

	// ������ (�U�����̖����� - ������̉��(�n�`�␳����))
	int myHitness = myStatus->myParam.ATTACK_HIT - (eCharacter->myStatus->myParam.ATTACK_AVO +
		StageCreate::Instance()->GetTerrainParam(eCharacter->myStatus->xPos / CHIP_SIZE, eCharacter->myStatus->yPos / CHIP_SIZE, "AVO"));
	if (myHitness > 100) myHitness = 100;
	int eHitness = eCharacter->myStatus->myParam.ATTACK_HIT - (myStatus->myParam.ATTACK_AVO +
		StageCreate::Instance()->GetTerrainParam(myStatus->xPos / CHIP_SIZE, myStatus->yPos / CHIP_SIZE, "AVO"));
	if (eHitness > 100) eHitness = 100;

	// �`��ʒu�̕␳�p�ϐ�
	int drawOffset = 0;

	// ���g�̈ʒu���āA�`��ʒu�ɕ␳��������
	if (myStatus->xPos > STAGE1_WIDTH / 2) {
		SpriteDraw(drawOffset, 0, FileManager::Instance()->GetFileHandle(ATTACK_DETAIL));
	}
	else if (myStatus->xPos < STAGE1_WIDTH / 2) {
		drawOffset = 480;
		SpriteDraw(drawOffset, 0, FileManager::Instance()->GetFileHandle(ATTACK_DETAIL));
	}

	// �p�����[�^�\��
	DrawFormatString(80 + drawOffset, 100, GetColor(255, 255, 0), "HP");
	DrawFormatString(80 + drawOffset, 125, GetColor(255, 255, 0), "�З�");
	DrawFormatString(80 + drawOffset, 150, GetColor(255, 255, 0), "����");

	DrawFormatString(120 + drawOffset, 100, GetColor(0, 0, 255), "%d", myStatus->HP);
	DrawFormatString(45 + drawOffset, 30, GetColor(0, 0, 0), myStatus->NAME.c_str());
	DrawFormatString(120 + drawOffset, 125, GetColor(0, 0, 255), "%d", mySTR);
	if (mySPD >= 4) DrawFormatString(130 + drawOffset, 130, GetColor(255, 255, 255), "�~2");
	DrawFormatString(120 + drawOffset, 150, GetColor(0, 0, 255), "%d", myHitness);

	DrawFormatString(45 + drawOffset, 100, GetColor(0, 0, 255), "%d", eCharacter->myStatus->HP);
	DrawFormatString(100 + drawOffset, 300, GetColor(255, 255, 255), eCharacter->myStatus->NAME.c_str());
	DrawFormatString(45 + drawOffset, 125, GetColor(0, 0, 255), "%d", eSTR);
	if (mySPD <= -4) DrawFormatString(55 + drawOffset, 130, GetColor(255, 255, 255), "�~2");
	DrawFormatString(45 + drawOffset, 150, GetColor(0, 0, 255), "%d", eHitness);
}

// �U���A�j���[�V����
bool Character::AttackAnimation(shared_ptr<Character> const &eCharacter, int count)
{

	// �U���������擾
	int moveX = eCharacter->myStatus->xPos - myStatus->_xPos;
	int moveY = eCharacter->myStatus->yPos - myStatus->_yPos;

	if (moveX > 0) {
		// �U���A�j���[�V����
		if (myStatus->animReset) myStatus->xPos -= 4;
		else myStatus->xPos += 4;

		if (myStatus->xPos - myStatus->_xPos == CHIP_SIZE) myStatus->animReset = true;
		if (myStatus->xPos == myStatus->_xPos) {
			CharacterAttack(eCharacter, count);
			myStatus->animReset = false;
			return false;
		}
	}
	else if (moveX < 0) {
		// �U���A�j���[�V����
		if (myStatus->animReset) myStatus->xPos += 4;
		else myStatus->xPos -= 4;

		if (myStatus->xPos - myStatus->_xPos == -CHIP_SIZE) myStatus->animReset = true;
		if (myStatus->xPos == myStatus->_xPos) {
			CharacterAttack(eCharacter, count);
			myStatus->animReset = false;
			return false;
		}
	}
	if (moveY > 0) {
		// �U���A�j���[�V����
		if (myStatus->animReset) myStatus->yPos -= 4;
		else myStatus->yPos += 4;

		if (myStatus->yPos - myStatus->_yPos == CHIP_SIZE) myStatus->animReset = true;
		if (myStatus->yPos == myStatus->_yPos) {
			CharacterAttack(eCharacter, count);
			myStatus->animReset = false;
			return false;
		}
	}
	else if (moveY < 0) {
		// �U���A�j���[�V����
		if (myStatus->animReset) myStatus->yPos += 4;
		else myStatus->yPos -= 4;

		if (myStatus->yPos - myStatus->_yPos == -CHIP_SIZE) myStatus->animReset = true;
		if (myStatus->yPos == myStatus->_yPos) {
			CharacterAttack(eCharacter, count);
			myStatus->animReset = false;
			return false;
		}
	}

	return true;
}

// �L�����N�^�[�̍U��
void Character::CharacterAttack(shared_ptr<Character> const &eCharacter, int count)
{
	// �З� (�U�����̗� - ������̎����)
	if (Item.empty() == false) myStatus->myParam.ATTACK_STR = myStatus->myParam.POWER + Item[0]->myParam.POWER;
	else myStatus->myParam.ATTACK_STR = myStatus->myParam.POWER;

	if (eCharacter->Item.empty() == false) eCharacter->myStatus->myParam.ATTACK_STR = eCharacter->myStatus->myParam.POWER + eCharacter->Item[0]->myParam.POWER;
	else eCharacter->myStatus->myParam.ATTACK_STR = eCharacter->myStatus->myParam.POWER;

	// �_���[�W�v�Z
	int damage = myStatus->myParam.ATTACK_STR - (eCharacter->myStatus->myParam.DEFENCE + 
		StageCreate::Instance()->GetTerrainParam(eCharacter->myStatus->xPos / CHIP_SIZE, eCharacter->myStatus->yPos / CHIP_SIZE, "DEF"));
	// �}�C�i�X��0
	if (damage < 0) damage = 0;

	// ��������(4�ȏ�傫�����2��U��)
	myStatus->myParam.ATTACK_SPEED = myStatus->myParam.SPEED - eCharacter->myStatus->myParam.SPEED;

	// ������ (�U�����̖����� - ������̉��)
	int myHitness = myStatus->myParam.ATTACK_HIT - (eCharacter->myStatus->myParam.ATTACK_AVO + 
		StageCreate::Instance()->GetTerrainParam(eCharacter->myStatus->xPos / CHIP_SIZE, eCharacter->myStatus->yPos / CHIP_SIZE, "AVO"));
	if (myHitness > 100) myHitness = 100;

	// �m���i�����́j
	int probability = myHitness;

	// �����̏�����
	srand((unsigned)time(NULL));
	// �U������
	if (GetRand(100) <= probability) {
		int _damage = eCharacter->myStatus->HP - damage;
		ApplyDamage(eCharacter, _damage);
		if (damage > 0) AudioManager::Instance()->playSE(SE_DAMAGE);
		else AudioManager::Instance()->playSE(SE_NODAMAGE);
		// �G���|�����炻�̎��_�Ő퓬�I��
		if (eCharacter->myStatus->isDeath) {
			myStatus->isAttack = false;
			return;
		}
	}
	else AudioManager::Instance()->playSE(SE_MISS);

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
}

// �U���̏���
void Character::ApplyDamage(shared_ptr<Character> const &eCharacter, int damage)
{
	if (eCharacter->myStatus->HP == damage && damage > 0)  return;

	eCharacter->myStatus->HP--;

	if (eCharacter->myStatus->HP <= 0) {
		eCharacter->myStatus->isDeath = true;
		return;
	}

	ApplyDamage(eCharacter, damage);
}

// �^���I�J�����Ƃ̃I�t�Z�b�g�̌v�Z
void Character::SetCameraOffset(int dir, bool horizontal)
{
	// ���E�v�Z
	if (horizontal) {
		myStatus->xPos += CHIP_SIZE * dir;
		myStatus->_xPos = myStatus->xPos;
	}
	// �㉺�v�Z
	else {
		myStatus->yPos += CHIP_SIZE * dir;
		myStatus->_yPos = myStatus->yPos;
	}
}

void Character::ResetArea(int x, int y)
{
	moveToPos[y][x] = -1;
	moveArrow[y][x] = false;
}

// �A�C�e���̒ǉ�
void Character::AddItem(string itemName)
{
	if (itemCount == 5) return;

	Item.push_back(make_unique<Weapon>());
	Item[itemCount]->ParamInitialize(itemName);
	itemCount++;
}

// ���j�b�g�̃��x���A�b�v
void Character::LevelUp()
{
	// �t�@�C�����J��
	fstream file;
	file.open(FileManager::Instance()->GetDataName(myData), ios::binary | ios::out);
	file.write((char*)&myStatus->myParam, sizeof(myStatus->myParam));

	file.close();
}
