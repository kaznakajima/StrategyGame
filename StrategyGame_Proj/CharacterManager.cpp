#include "CharacterManager.h"
#include "Character.h"

const int PlayerNum = 3;

// �L�����N�^�[�̏��
vector<Character::STATUS> statusList;
Character::STATUS status[PlayerNum];

// �L�����N�^�[�̃C���X�^���X
Character* character;

// ������
void CharacterManager::Initialize()
{
	character = new Character();

	for (int num = 0; num < PlayerNum; num++) {
		statusList.push_back(status[num]);
	}

	character->Character_Initialize(&statusList[0], CHARACTER_DATA_1, 240, 240);
	character->Character_Initialize(&statusList[1], CHARACTER_DATA_2, 144, 336);
	character->Character_Initialize(&statusList[2], CHARACTER_DATA_3, 96, 384);
}

// �A�j���[�V�����X�V
void CharacterManager::Update()
{
	for (unsigned int i = 0; i < statusList.size(); i++) {
		// ���S�������j�b�g�����O����
		if (statusList[i].isDeath) {
			statusList.erase(statusList.begin() + i);
			return;
		}

		character->CharacterAnim(&statusList[i]);
	}
}

// �`�悷�邩�`�F�b�N
void CharacterManager::DrawCheck(int x, int y) 
{
	// �U�����̓��^�[��
	if (attack == true) {
		Attack(x, y);
		return;
	}

	for (unsigned int i = 0; i < statusList.size(); i++)
	{
		// �J�[�\���������Ă��郆�j�b�g�̂ݕ\��
		if (statusList[i].PosX == x && statusList[i].PosY == y) {
			statusList[i].isSelect = true;
			statusList[i].AnimHandle = 4.0f;
			isSelect = true;
			attack = false;
		}
	}
}

// �`�揈��
void CharacterManager::Draw()
{
	for (unsigned int i = 0; i < statusList.size(); i++) {

		character->MoveAreaClear();

		if (statusList[i].isSelect) {
			character->OldPosX.push_back(statusList[i].PosX);
			character->OldPosY.push_back(statusList[i].PosY);
			character->MoveRange(&statusList[i], statusList[i].PosX, statusList[i].PosY, statusList[i].myParam.MOVERANGE);
			character->AttackRange();
			return;
		}
	}
}

void CharacterManager::CharacterMove(int x, int y) 
{
	for (unsigned int i = 0; i < statusList.size(); i++) {
		if (statusList[i].isSelect) {
			statusList[i].isMove = true;
			isMove = character->CharacterMove(&statusList[i], x, y);
		}
	}
}

// �ړ��l�̎擾
void CharacterManager::GetMoveCount(int x, int y) {
	character->OldPosX.push_back(x);
	character->OldPosY.push_back(y);
	character->moveCount++;
}

void CharacterManager::GetMoveArrow(int x, int y)
{
	for (unsigned int i = 0; i < statusList.size(); i++) {
		if (statusList[i].isSelect) {
			character->DrawMoveArrow(statusList[i], x, y, 5);

			if (x == statusList[i].PosX && y == statusList[i].PosY) {
				character->OldPosX.clear();
				character->OldPosY.clear();
				character->moveCount = 0;
			}
		}
	}
}

// �U���͈͕\��
void CharacterManager::GetAttackArea(int x, int y)
{
	// �U�����郆�j�b�g
	Character::STATUS* myStatus = nullptr;

	for (unsigned int num = 0; num < statusList.size(); num++) {
		if (statusList[num].canAttack) {
			myStatus = &statusList[num];
			character->AttackableDraw(&statusList[num]);
			attack = true;
		}
	}

	for (unsigned int num = 0; num < statusList.size(); num++) {
		if (myStatus != nullptr && myStatus != &statusList[num]) {
			if (statusList[num].PosX == x && statusList[num].PosY == y)
				character->GetAttackDetail(myStatus, &statusList[num]);
		}
	}
}

// �U��
void CharacterManager::Attack(int x, int y)
{
	// �U�����郆�j�b�g
	Character::STATUS* myStatus = nullptr;

	for (unsigned int num = 0; num < statusList.size(); num++) {
		if(statusList[num].canAttack)
			myStatus = &statusList[num];
	}

	for (unsigned int num = 0; num < statusList.size(); num++) {
		if (myStatus != &statusList[num]) {
			if (statusList[num].PosX == x && statusList[num].PosY == y)
				character->CharacterAttack(myStatus, &statusList[num], 1);
		}
	}

	if(myStatus != nullptr) myStatus->canAttack = false;
	attack = false;
}

// �J�����Ƃ̃I�t�Z�b�g�̌v�Z
void CharacterManager::SetCameraOffset(int dir, bool horizontal)
{
	for (unsigned int num = 0; num < statusList.size(); num++) {
		character->SetCameraOffset(&statusList[num], dir, horizontal);
	}
}

void CharacterManager::KeyCheck(int x, int y)
{
	for (unsigned int i = 0; i < statusList.size(); i++) {
		// �J�[�\���������Ă��郆�j�b�g�̂ݕ\��
		if (statusList[i].PosX != x || statusList[i].PosY != y) {
			character->OldPosX.clear();
			character->OldPosY.clear();
			character->moveCount = 0;
			isSelect = false;
		}
	}
}

void CharacterManager::Finalize() {

}