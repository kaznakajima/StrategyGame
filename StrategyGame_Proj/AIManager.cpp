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
	if(_myCharacter != nullptr) _myCharacter.reset();
	// ���X�g�̏�����
	_playerList.clear(); _playerList = CharacterManager::Instance()->_playerList;
	_enemyList.clear(); _enemyList = CharacterManager::Instance()->_enemyList;
}

// �X�V
void AIManager::Update()
{
	if (_myCharacter != nullptr) {
		if (_myCharacter->myStatus->canAttack) CharacterManager::Instance()->DrawCheck(xPos, yPos);
	}
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
	// �G�̈ړ���̌������o
	x = character->myStatus->xPos, y = character->myStatus->yPos;
	CharacterManager::Instance()->DrawCheck(x, y);
	character->MoveAreaClear(CharacterManager::Instance()->_character);
	character->MoveRange(character->myStatus->xPos, character->myStatus->yPos, character->myStatus->myParam.MOVERANGE);

	// �v���C���[���j�b�g�̎��͂�����
	for (size_t num = 0; num < _playerList.size(); ++num) {
		int moveX = _playerList[num]->myStatus->xPos, moveY = _playerList[num]->myStatus->yPos;
		// ���j�b�g�̎��͂��ړ��\�ȏꍇ�Ȃ�ړ���ɓo�^
		// ��
		if (moveY / CHIP_SIZE < 9 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE + character->myStatus->AttackRange][moveX / CHIP_SIZE] == true) {
			isMove = ChoiseMovePoint(moveX, moveY + CHIP_SIZE);
			if (isMove) { xPos = moveX, yPos = moveY; break;} // �U����̍��W���擾
			else if(moveX == x && moveY + CHIP_SIZE == y) { xPos = moveX, yPos = moveY; break; } // ���̏�ōU���ł���Ȃ�U������擾
		}
		// ���j�b�g�̎��͂��ړ��\�ȏꍇ�Ȃ�ړ���ɓo�^
		// ��
		if (moveY / CHIP_SIZE > 0 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE - character->myStatus->AttackRange][moveX / CHIP_SIZE] == true) {
			isMove = ChoiseMovePoint(moveX, moveY - CHIP_SIZE);
			if (isMove) { xPos = moveX, yPos = moveY; break; } // �U����̍��W���擾
			else if (moveX == x && moveY - CHIP_SIZE == y) { xPos = moveX, yPos = moveY; break; } // ���̏�ōU���ł���Ȃ�U������擾
		}
		// ���j�b�g�̎��͂��ړ��\�ȏꍇ�Ȃ�ړ���ɓo�^
		// �E
		if (moveX / CHIP_SIZE < 14 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE][moveX / CHIP_SIZE + character->myStatus->AttackRange] == true) {
			isMove = ChoiseMovePoint(moveX + CHIP_SIZE, moveY);
			if (isMove) { xPos = moveX, yPos = moveY; break; } // �U����̍��W���擾
			else if (moveX + CHIP_SIZE == x && moveY == y) { xPos = moveX, yPos = moveY; break; } // ���̏�ōU���ł���Ȃ�U������擾
		}
		// ���j�b�g�̎��͂��ړ��\�ȏꍇ�Ȃ�ړ���ɓo�^
		// ��
		if (moveX / CHIP_SIZE > 0 && StageCreate::Instance()->checkMove[moveY / CHIP_SIZE][moveX / CHIP_SIZE - character->myStatus->AttackRange] == true) {
			isMove = ChoiseMovePoint(moveX - CHIP_SIZE, moveY);
			if (isMove) { xPos = moveX, yPos = moveY; break; } // �U����̍��W���擾
			else if (moveX - CHIP_SIZE == x && moveY == y) { xPos = moveX, yPos = moveY; break; } // ���̏�ōU���ł���Ȃ�U������擾
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

	// ���ۂɈړ�������
	CharacterManager::Instance()->GetMoveArrow(x, y);
	CharacterManager::Instance()->KeyCheck(x, y);
}

// �ړ���̑I��
bool AIManager::ChoiseMovePoint(int _x, int _y)
{
	// ���̃��j�b�g�����̏�ɂ���Ȃ烊�^�[��
	if (StageCreate::Instance()->onUnit[_y / CHIP_SIZE][_x / CHIP_SIZE] != "NONE") return false;

	// �ړ���̑��
	x = _x, y = _y;

	return true;
}

// �v���C���[���̃��j�b�g���擾
int AIManager::GetDistancePlayer(shared_ptr<Character> const &character, vector<shared_ptr<Character>> const &playerList)
{
	// �v���C���[���̃L�����N�^�[�Ƃ̋���
	int offsetX = 0, offsetY = 0, offsetTotal = 0;
	// �ŒZ����
	int _minDistance = 100;

	// �v���C���[����̋����̌v�Z
	for (size_t num = 0; num < _playerList.size(); ++num) {
		offsetTotal = 0;
		offsetX = abs(character->myStatus->xPos - _playerList[num]->myStatus->xPos) / CHIP_SIZE;
		offsetY = abs(character->myStatus->yPos - _playerList[num]->myStatus->yPos) / CHIP_SIZE;
		offsetTotal = offsetX + offsetY;

		// �ŒZ�����̍X�V
		if (_minDistance > offsetTotal) {
			_minDistance = offsetTotal;
		}
	}

	// �ŒZ������Ԃ�
	return _minDistance;
}

// �ړ��n�_������
void AIManager::GetMovePoint(shared_ptr<Character> const &character, int _x, int _y, vector<shared_ptr<Character>> const &playerList)
{
	// ���g�̈ړ��͈͊O�A���̃��j�b�g������ʒu�Ȃ烊�^�[��
	if (StageCreate::Instance()->checkMove[_y][_x] == false || StageCreate::Instance()->onUnit[_y][_x] != "NONE") return;

	// �v���C���[���̃L�����N�^�[�Ƃ̋���
	int offsetX = 0, offsetY = 0, offsetTotal = 0;
	// �v���C���[���̃L�����N�^�[�Ƃ̋���
	int _offsetX = 0, _offsetY = 0, _offsetTotal = 0;

	// �ړ�����܂ł̍ł������ʒu�Ȃ���s
	if (character->moveToPos[_y][_x] > 0 && moveCost >= character->moveToPos[_y][_x]) {
		moveCost = character->moveToPos[_y][_x];
		// �v���C���[����̋����̌v�Z
		for (size_t num = 0; num < _playerList.size(); ++num) {
			offsetTotal = 0, _offsetTotal = 0;
			// �����̌��o
			offsetX = abs(_x * CHIP_SIZE - _playerList[num]->myStatus->xPos) / CHIP_SIZE;
			offsetY = abs(_y * CHIP_SIZE - _playerList[num]->myStatus->yPos) / CHIP_SIZE;
			offsetTotal = offsetX + offsetY;
			// ���݂̍ŒZ�����̌��o
			_offsetX = abs(x - _playerList[num]->myStatus->xPos) / CHIP_SIZE;
			_offsetY = abs(y - _playerList[num]->myStatus->yPos) / CHIP_SIZE;
			_offsetTotal = _offsetX + _offsetY;

			// �ŒZ�����̍X�V
			if (offsetTotal < _offsetTotal) {
				CheckCanMove(character, _x, _y, _playerList[num]);
			}
		}
	}
}

// �ړ��\�Ȓn�_���`�F�b�N����
void AIManager::CheckCanMove(shared_ptr<Character> const &character, int _x, int _y, shared_ptr<Character> const &playerSt)
{
	// �ړ��s�\�Ȓn�_�Ȃ烊�^�[��
	if (StageCreate::Instance()->stageList[_y][_x] > 0) return;

	// �ړ�����i�[
	isMove = true;
	x = _x * CHIP_SIZE;
	y = _y * CHIP_SIZE;
}

// �I������
void AIManager::Finalize()
{
	if (_playerList.empty() == false) _playerList.clear();
	if (_enemyList.empty() == false) _enemyList.clear();
	if (_myCharacter != nullptr) _myCharacter.reset();
}