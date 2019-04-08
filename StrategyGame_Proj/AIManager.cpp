#include "AIManager.h"
#include "StageCreate.h"

AIManager::AIManager()
{
	isMove = false;
}

AIManager::~AIManager() { }

// ������
void AIManager::Initialize()
{
	_myCharacter = nullptr;
	// ���X�g�̏�����
	_playerList.clear();
	_enemyList.clear();
}

// �X�V
void AIManager::Update()
{
	if (_myCharacter != nullptr) {
		if (_myCharacter->myStatus->canAttack) CharacterManager::Instance()->DrawCheck(xPos, yPos);
	}
}

// ���݂̓G(AI)�A�v���C���[�̃J�E���g
void AIManager::CharacterCount(shared_ptr<Character> const &character)
{
	if (character->myStatus->myTeam == "Player") _playerList.push_back(character);
	else if (character->myStatus->myTeam == "Enemy") _enemyList.push_back(character);
}

// ����N��
void AIManager::Play()
{
	isMove = false;
	int _minDistance = 100;

	// �v���C���[�Ɉ�ԋ߂����j�b�g���s��������
	for (size_t num = 0; num < _enemyList.size(); ++num) {
		// ��ԋ߂��L�����N�^�[��I��
		if (_minDistance > GetDistancePlayer(_enemyList[num], _playerList) && _enemyList[num]->myStatus->canMove) {
			_minDistance = GetDistancePlayer(_enemyList[num], _playerList);
			_myCharacter = _enemyList[num];
		}
	}

	// �ړ���̑I��
	if (_myCharacter != nullptr) MoveSelect(_myCharacter);
}

// �ړ���̑I��
void AIManager::MoveSelect(shared_ptr<Character> const &character)
{
	x = character->myStatus->PosX, y = character->myStatus->PosY;

	CharacterManager::Instance()->DrawCheck(x, y);

	character->MoveAreaClear(CharacterManager::Instance()->_character);

	character->MoveRange(character->myStatus->PosX, character->myStatus->PosY, character->myStatus->myParam.MOVERANGE);

	// �v���C���[���j�b�g�̎��͂�����
	for (size_t num = 0; num < _playerList.size(); ++num) {
		int moveX = _playerList[num]->myStatus->PosX, moveY = _playerList[num]->myStatus->PosY;
		// ���j�b�g�̎��͂��ړ��\�ȏꍇ�Ȃ�ړ���ɓo�^
		// �E
		if (moveY / CHIP_SIZE < 9 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE + character->myStatus->AttackRange][moveX / CHIP_SIZE] == true) {
			ChoiseMovePoint(moveX, moveY + CHIP_SIZE);
			if (isMove) {
				xPos = moveX, yPos = moveY;
				break;
			}
		}
		// ���j�b�g�̎��͂��ړ��\�ȏꍇ�Ȃ�ړ���ɓo�^
		// ��
		if (moveY / CHIP_SIZE > 0 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE - character->myStatus->AttackRange][moveX / CHIP_SIZE] == true) {
			ChoiseMovePoint(moveX, moveY - CHIP_SIZE);
			if (isMove) {
				xPos = moveX, yPos = moveY;
				break;
			}
		}
		// ���j�b�g�̎��͂��ړ��\�ȏꍇ�Ȃ�ړ���ɓo�^
		// ��
		if (moveX / CHIP_SIZE < 14 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE][moveX / CHIP_SIZE + character->myStatus->AttackRange] == true) {
			ChoiseMovePoint(moveX + CHIP_SIZE, moveY);
			if (isMove) {
				xPos = moveX, yPos = moveY;
				break;
			}
		}
		// ���j�b�g�̎��͂��ړ��\�ȏꍇ�Ȃ�ړ���ɓo�^
		// ��
		if (moveX / CHIP_SIZE > 0 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE][moveX / CHIP_SIZE - character->myStatus->AttackRange] == true) {
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
				GetMovePoint(character, stageX, stageY, _playerList);
			}
		}
	}
	CharacterManager::Instance()->GetMoveArrow(x, y);
	CharacterManager::Instance()->KeyCheck(x, y);
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
int AIManager::GetDistancePlayer(shared_ptr<Character> const &character, vector<shared_ptr<Character>> const &playerList)
{
	// �v���C���[���̃L�����N�^�[�Ƃ̋���
	int offsetX = 0, offsetY = 0, offsetTotal = 0;
	// �ŒZ����
	int _minDistance = 100;

	// �v���C���[����̋����̌v�Z
	for (size_t num = 0; num < _playerList.size(); ++num) {
		offsetTotal = 0;
		offsetX = abs(character->myStatus->PosX - _playerList[num]->myStatus->PosX) / CHIP_SIZE;
		offsetY = abs(character->myStatus->PosY - _playerList[num]->myStatus->PosY) / CHIP_SIZE;
		offsetTotal = offsetX + offsetY;

		// �ŒZ�����̍X�V
		if (_minDistance > offsetTotal) {
			_minDistance = offsetTotal;
		}
	}

	return _minDistance;
}

// �ړ��n�_������
void AIManager::GetMovePoint(shared_ptr<Character> const &character, int _x, int _y, vector<shared_ptr<Character>> const &playerList)
{
	if (StageCreate::Instance()->checkMove[_y][_x] == false || StageCreate::Instance()->onUnit[_y][_x] != "NONE") return;

	// �v���C���[���̃L�����N�^�[�Ƃ̋���
	int offsetX = 0, offsetY = 0, offsetTotal = 0;
	// �v���C���[���̃L�����N�^�[�Ƃ̋���
	int _offsetX = 0, _offsetY = 0, _offsetTotal = 0;

	if (character->moveToPos[_y][_x] > 0 && moveCost >= character->moveToPos[_y][_x]) {
		moveCost = character->moveToPos[_y][_x];
		// �v���C���[����̋����̌v�Z
		for (size_t num = 0; num < _playerList.size(); ++num) {
			offsetTotal = 0, _offsetTotal = 0;
			// �����̌��o
			offsetX = abs(_x * CHIP_SIZE - _playerList[num]->myStatus->PosX) / CHIP_SIZE;
			offsetY = abs(_y * CHIP_SIZE - _playerList[num]->myStatus->PosY) / CHIP_SIZE;
			offsetTotal = offsetX + offsetY;
			// ���݂̍ŒZ�����̌��o
			_offsetX = abs(x - _playerList[num]->myStatus->PosX) / CHIP_SIZE;
			_offsetY = abs(y - _playerList[num]->myStatus->PosY) / CHIP_SIZE;
			_offsetTotal = _offsetX + _offsetY;

			// �ŒZ�����̍X�V
			if (offsetTotal <= _offsetTotal) {
				// �ړ��\���`�F�b�N
				CheckCanMove(character, _playerList[num]->myStatus->PosX / CHIP_SIZE, _playerList[num]->myStatus->PosY / CHIP_SIZE, _playerList[num]);
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

// �ړ��\�Ȓn�_���`�F�b�N����
void AIManager::CheckCanMove(shared_ptr<Character> const &character, int _x, int _y, shared_ptr<Character> const &playerSt)
{
	// �ړ��s�\�Ȓn�_�Ȃ烊�^�[��
	if (StageCreate::Instance()->stageList[_y][_x] > 0) return;

	// �ŒZ�����̒n�_���i�[�����^�[��
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

	// �ŒZ�����̃��j�b�g�Ƃ̋����𑪂�
	int moveX = (character->myStatus->PosX - _x * CHIP_SIZE) / CHIP_SIZE, moveY = (character->myStatus->PosY - _y * CHIP_SIZE) / CHIP_SIZE;

	if (moveX > character->myStatus->PosX / CHIP_SIZE || moveY > character->myStatus->PosY / CHIP_SIZE) {
		// ���j�b�g�Ɍ������ă��[�g���t�T�m���Ă���
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

void AIManager::Finalize()
{
	if (_playerList.empty() == false) _playerList.clear();
	if (_enemyList.empty() == false) _enemyList.clear();
}