#include "CharacterManager.h"
#include "Character.h"

const int PlayerNum = 3;

// �L�����N�^�[�̏��
vector<Character::STATUS> statusList;
Character::STATUS status[PlayerNum];

// �L�����N�^�[�̃C���X�^���X
Character* character;
// �U�����郆�j�b�g
Character::STATUS* myStatus = nullptr;
// �U������郆�j�b�g
Character::STATUS* eStatus = nullptr;

// ������
void CharacterManager::Initialize()
{
	character = new Character();

	for (int num = 0; num < PlayerNum; num++) {
		statusList.push_back(status[num]);
	}

	character->Character_Initialize(&statusList[0], CHARACTER_DATA_1, "Player", 240, 240);
	character->Character_Initialize(&statusList[1], CHARACTER_DATA_2, "Enemy", 144, 336);
	character->Character_Initialize(&statusList[2], CHARACTER_DATA_3, "Player", 96, 384);

	StartTurn();
}

// �L�����N�^�[�X�V
void CharacterManager::Update(int x, int y)
{
	// �U���͈͌���
	GetAttackArea(x, y);

	for (unsigned int i = 0; i < statusList.size(); i++) {
		// ���S�������j�b�g�����O����
		if (statusList[i].isDeath) {
			statusList.erase(statusList.begin() + i);
			return;
		}

		character->CharacterAnim(&statusList[i]);
	}

	if (isSelect == false) CharacterMove(x, y);

	if (attack) Attack();
}

// �^�[���J�n
void CharacterManager::StartTurn()
{
	playerTurn = !playerTurn;
	moveableUnit = 0;

	// �ړ��\�ȃ��j�b�g�̃J�E���g
	for (int num = 0; num < statusList.size(); num++) {
		// �v���C���[�^�[��
		if (playerTurn && statusList[num].myTeam == "Player") moveableUnit++;
		// �G�^�[��
		else if (playerTurn == false && statusList[num].myTeam == "Enemy") moveableUnit++;
	}
}

// �`�悷�邩�`�F�b�N
void CharacterManager::DrawCheck(int x, int y) 
{
	// �U�����̓��^�[��
	if (attack == true) {
		ChoiseAttack(x, y);
		return;
	}

	for (unsigned int i = 0; i < statusList.size(); i++)
	{
		// �J�[�\���������Ă��郆�j�b�g�̂ݕ\��
		if (statusList[i].PosX == x && statusList[i].PosY == y) {
			if (statusList[i].myTeam == "Player" && statusList[i].canMove) {
				statusList[i].isSelect = true;
				statusList[i].AnimHandle = 4.0f;
				isSelect = true;
				attack = false;
			}
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
			isMove = character->CharacterMove(&statusList[i], x, y);
			if (isMove == false && statusList[i].canMove == false && statusList[i].canAttack == false) moveableUnit--;
		}
	}

	if (moveableUnit <= 0) StartTurn();
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

// �U���̑Ώۂ�I��
void CharacterManager::ChoiseAttack(int x, int y)
{
	// ���ݍU���\�ȃ��j�b�g�̃��t�@�����X
	for (unsigned int num = 0; num < statusList.size(); num++) {
		if(statusList[num].canAttack) myStatus = &statusList[num];
	}

	// �I�������ʒu�ɓG��������U���Ώۂ̃��t�@�����X���쐬
	for (unsigned int num = 0; num < statusList.size(); num++) {
		if (myStatus != &statusList[num]) {
			if (statusList[num].PosX == x && statusList[num].PosY == y) {
				eStatus = &statusList[num];
				myStatus->isAttack = true;
			}
		}
	}

	// �U���\�ȃ��j�b�g�����Ȃ�������I��
	if(myStatus != nullptr) myStatus->canAttack = false;
	// �I�������ʒu�ɓG�����Ȃ��Ȃ�I��
	if (eStatus == nullptr) {
		attack = false;
		moveableUnit--;
		if (moveableUnit == 0) StartTurn();
	}
}

// �U���̃A�j���[�V����
void CharacterManager::Attack()
{
	// 1��ڂ̍U��
	if (myStatus != nullptr && eStatus != nullptr && attackCount < 2) {
		if (myStatus->isAttack) attack = character->AttackAnimation(myStatus, eStatus, 1);
		if (eStatus->isAttack) attack = character->AttackAnimation(eStatus, myStatus, 2);

		// �A�j���[�V�������I����Ă��Ȃ��Ȃ烊�^�[��
		if (attack) return;
	}

	// �A�j���[�V�������I����Ă���Ȃ�U���̉񐔂��L�^
	if(attack == false) attackCount = 3;

	// 2��ڂ̍U��
	if (myStatus != nullptr && eStatus != nullptr && attackCount > 2) {
		if (myStatus->isAttack) attack = character->AttackAnimation(myStatus, eStatus, 3);
		if (eStatus->isAttack) attack = character->AttackAnimation(eStatus, myStatus, 4);
		
		// �A�j���[�V�������I����Ă��Ȃ��Ȃ烊�^�[��
		if (attack) return;
	}

	// �A�j���[�V�������I������Ȃ�U���I��
	if (attack == false) {
		attackCount = 0;
		myStatus = nullptr;
		eStatus = nullptr;
		moveableUnit--;
		if (moveableUnit <= 0) StartTurn();
	}
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