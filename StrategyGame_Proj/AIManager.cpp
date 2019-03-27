#include "AIManager.h"
#include "StageCreate.h"

AIManager::AIManager()
{
	characterMgr = CharacterManager::Instance();

	isMove = false;
}

// ������
void AIManager::Initialize()
{
	// ���X�g�̏�����
	playerList.clear();
	enemyList.clear();
}

// �X�V
void AIManager::Update()
{
	DrawFormatString(0, 48, GetColor(0, 0, 0), "%d", playerList.size());
	if (myCharacter != nullptr) {
		if (characterMgr->isSelect) {
			//characterMgr->Draw();
		}
		if (myCharacter->myStatus->canAttack) characterMgr->DrawCheck(xPos, yPos);
	}
}

// ���݂̓G(AI)�A�v���C���[�̃J�E���g
void AIManager::CharacterCount(Character* character)
{
	if (character->myStatus->myTeam == "Player") {
		playerList.push_back(character);
	}
	else if (character->myStatus->myTeam == "Enemy") {
		enemyList.push_back(character);
	} 
}

// ����N��
void AIManager::Play()
{
	isMove = false;
	myCharacter = nullptr;
	int _minDistance = 100;

	// �v���C���[�Ɉ�ԋ߂����j�b�g���s��������
	for (Character* character : enemyList) {
		// ��ԋ߂��L�����N�^�[��I��
		if (_minDistance > GetDistancePlayer(character, playerList) && character->myStatus->canMove) {
			_minDistance = GetDistancePlayer(character, playerList);
			myCharacter = character;
		}
	}

	if (myCharacter != nullptr) MoveSelect(myCharacter);
}

// �������L�����N�^�[�̑I��
void AIManager::MoveSelect(Character* character)
{
	x = character->myStatus->PosX, y = character->myStatus->PosY;

	characterMgr->DrawCheck(x, y);

	character->MoveAreaClear(characterMgr->character);

	character->MoveRange(character->myStatus->PosX, character->myStatus->PosY, character->myStatus->myParam.MOVERANGE);

	for (Character* _character : playerList) {
		int moveX  = _character->myStatus->PosX, moveY = _character->myStatus->PosY;
		if (moveY / CHIP_SIZE < 9 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE + 1][moveX / CHIP_SIZE] == true) {
			ChoiseMovePoint(moveX, moveY + CHIP_SIZE);
			if (isMove) {
				xPos = moveX, yPos = moveY;
				break;
			}
		}
		if (moveY / CHIP_SIZE > 0 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE - 1][moveX / CHIP_SIZE] == true) {
			ChoiseMovePoint(moveX, moveY - CHIP_SIZE);
			if (isMove) {
				xPos = moveX, yPos = moveY;
				break;
			}
		}
		if (moveX / CHIP_SIZE < 14 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE][moveX / CHIP_SIZE + 1] == true) {
			ChoiseMovePoint(moveX + CHIP_SIZE, moveY);
			if (isMove) {
				xPos = moveX, yPos = moveY;
				break;
			}
		}
		if (moveX / CHIP_SIZE > 0 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE][moveX / CHIP_SIZE - 1] == true) {
			ChoiseMovePoint(moveX - CHIP_SIZE, moveY);
			if (isMove) {
				xPos = moveX, yPos = moveY;
				break;
			}
		}
	}

	// �܂��ړ��悪���܂��Ă��Ȃ��Ȃ�
	if (isMove == false) {
		minDistance = 100;
		moveCost = character->myStatus->myParam.MOVERANGE;
		// �v���C���[�����Ɍ������Ĉړ�
		for (int stageY = 0; stageY < StageCreate::Instance()->MAP_SIZEY; stageY++) {
			for (int stageX = 0; stageX < StageCreate::Instance()->MAP_SIZEX; stageX++) {
				GetMovePoint(character, stageX, stageY, playerList);
			}
		}
	}
	characterMgr->GetMoveArrow(x, y);
	characterMgr->KeyCheck(x, y);
}

// �ړ���̑I��
void AIManager::ChoiseMovePoint(int _x, int _y)
{
	if (StageCreate::Instance()->onUnit[_y / CHIP_SIZE][_x / CHIP_SIZE] != "NONE") {
		isMove = false; 
		return;
	}

	x = _x, y = _y;

	isMove = true;
}

// �v���C���[���̃L�����N�^�[�̎擾
int AIManager::GetDistancePlayer(Character* character, vector<Character*> playerList)
{
	// �v���C���[���̃L�����N�^�[�Ƃ̋���
	int offsetX = 0, offsetY = 0, offsetTotal = 0;
	// �ŒZ����
	int _minDistance = 100;

	// �v���C���[����̋����̌v�Z
	for (Character* playerSt : playerList) {
		offsetTotal = 0;
		offsetX = abs(character->myStatus->PosX - playerSt->myStatus->PosX) / CHIP_SIZE;
		offsetY = abs(character->myStatus->PosY - playerSt->myStatus->PosY) / CHIP_SIZE;
		offsetTotal = offsetX + offsetY;

		// �ŒZ�����̍X�V
		if (_minDistance > offsetTotal) {
			_minDistance = offsetTotal;
		}
	}

	return _minDistance;
}

// �ړ��n�_������
void AIManager::GetMovePoint(Character* character, int _x, int _y, vector<Character*> playerList)
{
	if (StageCreate::Instance()->checkMove[_y][_x] == false || StageCreate::Instance()->onUnit[_y][_x] != "NONE") return;

	// �v���C���[���̃L�����N�^�[�Ƃ̋���
	int offsetX = 0, offsetY = 0, offsetTotal = 0;
	// �v���C���[���̃L�����N�^�[�Ƃ̋���
	int _offsetX = 0, _offsetY = 0, _offsetTotal = 0;

	if (character->moveToPos[_y][_x] > 0 && moveCost >= character->moveToPos[_y][_x]) {
		moveCost = character->moveToPos[_y][_x];
		// �v���C���[����̋����̌v�Z
		for (Character* playerSt : playerList) {
			offsetTotal = 0, _offsetTotal = 0;
			offsetX = abs(_x * CHIP_SIZE - playerSt->myStatus->PosX) / CHIP_SIZE;
			offsetY = abs(_y * CHIP_SIZE - playerSt->myStatus->PosY) / CHIP_SIZE;
			offsetTotal = offsetX + offsetY;
			_offsetX = abs(x - playerSt->myStatus->PosX) / CHIP_SIZE;
			_offsetY = abs(y - playerSt->myStatus->PosY) / CHIP_SIZE;
			_offsetTotal = _offsetX + _offsetY;

			// �ŒZ�����̍X�V
			if (offsetTotal <= _offsetTotal) {
				CheckCanMove(character, playerSt->myStatus->PosX / CHIP_SIZE, playerSt->myStatus->PosY / CHIP_SIZE, playerSt);
				if (isMove == false) {
					x = moveToX;
					y = moveToY;
				}
				else {
					moveToX = _x * CHIP_SIZE;
					moveToY = _y * CHIP_SIZE;
				}
			}
		}
	}
}

void AIManager::CheckCanMove(Character* character, int _x, int _y, Character* playerSt)
{
	if (StageCreate::Instance()->stageList[_y][_x] > 0) return;

	if (character->moveToPos[_y][_x] == moveCost && StageCreate::Instance()->onUnit[_y][_x] == "NONE") {
		isMove = true;
		int disX = abs(_x * CHIP_SIZE - playerSt->myStatus->PosX) / CHIP_SIZE, disY = abs(_y * CHIP_SIZE - playerSt->myStatus->PosY) / CHIP_SIZE;
		int offset = disX + disY;
		if (minDistance > offset) {
			minDistance = offset;
			x = _x * CHIP_SIZE;
			y = _y * CHIP_SIZE;
		}
		return;
	}

	int moveX = (character->myStatus->PosX - _x * CHIP_SIZE) / CHIP_SIZE, moveY = (character->myStatus->PosY - _y * CHIP_SIZE) / CHIP_SIZE;
	if (moveX > character->myStatus->PosX / CHIP_SIZE || moveY > character->myStatus->PosY / CHIP_SIZE) {
		if (_x + 1 < 14 && StageCreate::Instance()->stageList[_y][_x + 1] <= 0) {
			CheckCanMove(character, _x + 1, _y, playerSt);
		}
		if (_y + 1 < 9 && StageCreate::Instance()->stageList[_y + 1][_x] <= 0) {
			CheckCanMove(character, _x, _y + 1, playerSt);
		}
		return;
	}
	else if (moveX > character->myStatus->PosX / CHIP_SIZE || moveY < character->myStatus->PosY / CHIP_SIZE) {
		// ���j�b�g�Ɍ������ă��[�g���t�T�m���Ă���
		if (_x + 1 < 14 && StageCreate::Instance()->stageList[_y][_x + 1] <= 0) {
			CheckCanMove(character, _x + 1, _y, playerSt);
		}
		if (_y - 1 > 0 && StageCreate::Instance()->stageList[_y - 1][_x] <= 0) {
			CheckCanMove(character, _x, _y - 1, playerSt);
		}
		return;
	}
	else if (moveX <  character->myStatus->PosX / CHIP_SIZE || moveY >  character->myStatus->PosY / CHIP_SIZE) {
		// ���j�b�g�Ɍ������ă��[�g���t�T�m���Ă���
		if (_x - 1 > 0 && StageCreate::Instance()->stageList[_y][_x - 1] <= 0) {
			CheckCanMove(character, _x - 1, _y, playerSt);
		}
		if (_y + 1 < 9 && StageCreate::Instance()->stageList[_y + 1][_x] <= 0) {
			CheckCanMove(character, _x, _y + 1, playerSt);
		}
		return;
	}
	else if (moveX < character->myStatus->PosX / CHIP_SIZE || moveY < character->myStatus->PosY / CHIP_SIZE) {
		// ���j�b�g�Ɍ������ă��[�g���t�T�m���Ă���
		if (_x - 1 > 0 && StageCreate::Instance()->stageList[_y][_x - 1] <= 0) {
			CheckCanMove(character, _x - 1, _y, playerSt);
		}
		if (_y - 1 > 0 && StageCreate::Instance()->stageList[_y - 1][_x] <= 0) {
			CheckCanMove(character, _x, _y - 1, playerSt);
		}
		return;
	}
}

// �G�L�����N�^�[�̃��X�g(���S����)
void AIManager::CharacterLost(Character* character)
{

}